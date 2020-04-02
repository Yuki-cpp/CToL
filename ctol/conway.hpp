#pragma once
#include <type_traits>

#include "cell_list.hpp"
#include "neighbors_exploration.hpp"
#include "types.hpp"

namespace ctol
{
template<typename T>
struct living_cell_counter
{
    constexpr static unsigned value = 0;
};
template<typename T, typename... Ts>
struct living_cell_counter<ctol::cell_list<T, Ts...>>
{
    constexpr static unsigned value = ctol::living_cell_counter<T>::value +
                                      ctol::living_cell_counter<ctol::cell_list<Ts...>>::value;
};
template<>
struct living_cell_counter<ctol::living_cell>
{
    constexpr static unsigned value = 1;
};

template<typename Cell, typename Neighbors>
struct get_next_state;
template<typename Neighbors>
struct get_next_state<ctol::living_cell, Neighbors>
{
    using type = typename std::conditional<ctol::living_cell_counter<Neighbors>::value == 2 ||
                                               ctol::living_cell_counter<Neighbors>::value == 3,
                                           ctol::living_cell,
                                           ctol::dead_cell>::type;
};
template<typename Neighbors>
struct get_next_state<ctol::dead_cell, Neighbors>
{
    using type = typename std::conditional<ctol::living_cell_counter<Neighbors>::value == 3,
                                           ctol::living_cell,
                                           ctol::dead_cell>::type;
};

template<std::size_t current, typename ConwayGrid, typename BaseGrid>
struct next_generation_helper
{
    using position = typename ctol::get_position_of<typename BaseGrid::size, current>::type;
    using neighbors = typename ctol::
        get_neighbors<typename BaseGrid::size, position, typename BaseGrid::grid>::type;

    using new_state = typename ctol::get_next_state<
        typename ctol::get<typename BaseGrid::grid,
                           ctol::get_index_of<typename BaseGrid::size, position>::value>::type,
        neighbors>::type;

    using new_grid = typename ctol::set<typename ConwayGrid::grid, current, new_state>::type;
    using new_conway_grid = ctol::conway_grid<new_grid, typename ConwayGrid::size>;

    using type = std::conditional_t<
        current == 1,
        new_conway_grid,
        typename ctol::next_generation_helper<current - 1, new_conway_grid, BaseGrid>::type>;
};
template<typename ConwayGrid, typename BaseGrid>
struct next_generation_helper<0, ConwayGrid, BaseGrid>
{
    using type = ConwayGrid;
};

template<typename ConwayGrid>
struct next_generation
{
    constexpr static std::size_t size = (ConwayGrid::size::x * ConwayGrid::size::y) - 1;
    using type = ctol::next_generation_helper<size, ConwayGrid, ConwayGrid>::type;
};

template<typename ConwayGrid, std::size_t generations>
struct conway_game
{
    using type = typename conway_game<typename ctol::next_generation<ConwayGrid>::type,
                                      generations - 1>::type;
};
template<typename ConwayGrid>
struct conway_game<ConwayGrid, 0>
{
    using type = ConwayGrid;
};

} // namespace ctol