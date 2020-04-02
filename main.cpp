#include "type_list.hpp"
#include <iostream>

template<bool alive>
struct cell
{
    constexpr static bool value = alive;
};
using living_cell = cell<true>;
using dead_cell = cell<false>;

template<typename TypeList>
struct living_cell_counter
{
    constexpr static unsigned value = 0;
};
template<typename T, typename... Ts>
struct living_cell_counter<type_list<T, Ts...>>
{
    constexpr static unsigned value =
        living_cell_counter<T>::value + living_cell_counter<type_list<Ts...>>::value;
};
template<>
struct living_cell_counter<living_cell>
{
    constexpr static unsigned value = 1;
};

template<typename Cell, typename Neighbors>
struct get_next_state;
template<typename Neighbors>
struct get_next_state<living_cell, Neighbors>
{
    using type = typename std::conditional<living_cell_counter<Neighbors>::value == 2 ||
                                               living_cell_counter<Neighbors>::value == 3,
                                           living_cell,
                                           dead_cell>::type;
};
template<typename Neighbors>
struct get_next_state<dead_cell, Neighbors>
{
    using type = typename std::
        conditional<living_cell_counter<Neighbors>::value == 3, living_cell, dead_cell>::type;
};

template<std::size_t X, std::size_t Y>
struct point
{
    constexpr static std::size_t x = X;
    constexpr static std::size_t y = Y;
};

template<typename Size, typename Position>
struct get_index_of
{
    static constexpr std::size_t value = Position::x + (Size::x * Position::y);
};
template<typename Size, std::size_t index>
struct get_position_of
{
    using type = point<index % Size::x, index / Size::x>;
};

template<typename Size, typename Position, typename Grid>
struct up
{
    using type = std::conditional_t<
        Position::y == 0,
        typename get<Grid, get_index_of<Size, point<Position::x, Size::y - 1>>::value>::type,
        typename get<Grid, get_index_of<Size, point<Position::x, Position::y - 1>>::value>::type>;
};

template<typename Size, typename Position, typename Grid>
struct up_right
{
    constexpr static std::size_t X =
        std::conditional_t<Position::x == Size::x - 1,
                           std::integral_constant<std::size_t, 0>,
                           std::integral_constant<std::size_t, Position::x + 1>>::value;

    constexpr static std::size_t Y =
        std::conditional_t<Position::y == 0,
                           std::integral_constant<std::size_t, Size::y - 1>,
                           std::integral_constant<std::size_t, Position::y - 1>>::value;

    using type = typename get<Grid, get_index_of<Size, point<X, Y>>::value>::type;
};

template<typename Size, typename Position, typename Grid>
struct up_left
{
    constexpr static std::size_t X =
        std::conditional_t<Position::x == 0,
                           std::integral_constant<std::size_t, Size::x - 1>,
                           std::integral_constant<std::size_t, Position::x - 1>>::value;

    constexpr static std::size_t Y =
        std::conditional_t<Position::y == 0,
                           std::integral_constant<std::size_t, Size::y - 1>,
                           std::integral_constant<std::size_t, Position::y - 1>>::value;

    using type = typename get<Grid, get_index_of<Size, point<X, Y>>::value>::type;
};

template<typename Size, typename Position, typename Grid>
struct down_right
{
    constexpr static std::size_t X =
        std::conditional_t<Position::x == Size::x - 1,
                           std::integral_constant<std::size_t, 0>,
                           std::integral_constant<std::size_t, Position::x + 1>>::value;

    constexpr static std::size_t Y =
        std::conditional_t<Position::y == Size::y - 1,
                           std::integral_constant<std::size_t, 0>,
                           std::integral_constant<std::size_t, Position::y + 1>>::value;

    using type = typename get<Grid, get_index_of<Size, point<X, Y>>::value>::type;
};

template<typename Size, typename Position, typename Grid>
struct down_left
{
    constexpr static std::size_t X =
        std::conditional_t<Position::x == 0,
                           std::integral_constant<std::size_t, Size::x - 1>,
                           std::integral_constant<std::size_t, Position::x - 1>>::value;

    constexpr static std::size_t Y =
        std::conditional_t<Position::y == Size::y - 1,
                           std::integral_constant<std::size_t, 0>,
                           std::integral_constant<std::size_t, Position::y + 1>>::value;

    using type = typename get<Grid, get_index_of<Size, point<X, Y>>::value>::type;
};

template<typename Size, typename Position, typename Grid>
struct down
{
    using type = std::conditional_t<
        Position::y == Size::y - 1,
        typename get<Grid, get_index_of<Size, point<Position::x, 0>>::value>::type,
        typename get<Grid, get_index_of<Size, point<Position::x, Position::y + 1>>::value>::type>;
};

template<typename Size, typename Position, typename Grid>
struct left
{
    using type = std::conditional_t<
        Position::x == 0,
        typename get<Grid, get_index_of<Size, point<Size::x - 1, Position::y>>::value>::type,
        typename get<Grid, get_index_of<Size, point<Position::x - 1, Position::y>>::value>::type>;
};

template<typename Size, typename Position, typename Grid>
struct right
{
    using type = std::conditional_t<
        Position::x == Size::x - 1,
        typename get<Grid, get_index_of<Size, point<0, Position::y>>::value>::type,
        typename get<Grid, get_index_of<Size, point<Position::x + 1, Position::y>>::value>::type>;
};

template<typename Size, typename Position, typename Grid>
struct get_neighbors
{
    using type = type_list<typename up<Size, Position, Grid>::type,
                           typename up_right<Size, Position, Grid>::type,
                           typename right<Size, Position, Grid>::type,
                           typename down_right<Size, Position, Grid>::type,
                           typename down<Size, Position, Grid>::type,
                           typename down_left<Size, Position, Grid>::type,
                           typename left<Size, Position, Grid>::type,
                           typename up_left<Size, Position, Grid>::type>;
};

template<typename Size, typename Grid>
struct conway_grid
{
    using grid = Grid;
    using size = Size;
};

template<std::size_t current, typename ConwayGrid, typename BaseGrid>
struct next_generation_helper
{
    using position = typename get_position_of<typename BaseGrid::size, current>::type;
    using neighbors =
        typename get_neighbors<typename BaseGrid::size, position, typename BaseGrid::grid>::type;

    using new_state = typename get_next_state<
        typename get<typename BaseGrid::grid,
                     get_index_of<typename BaseGrid::size, position>::value>::type,
        neighbors>::type;

    using new_grid = typename set<typename ConwayGrid::grid, current, new_state>::type;
    using new_conway_grid = conway_grid<typename ConwayGrid::size, new_grid>;

    using type = std::conditional_t<
        current == 1,
        new_conway_grid,
        typename next_generation_helper<current - 1, new_conway_grid, BaseGrid>::type>;
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
    using type = typename next_generation_helper<size, ConwayGrid, ConwayGrid>::type;
};

template<typename ConwayGrid, std::size_t generations>
struct conway_game
{
    using type =
        typename conway_game<typename next_generation<ConwayGrid>::type, generations - 1>::type;
};
template<typename ConwayGrid>
struct conway_game<ConwayGrid, 0>
{
    using type = ConwayGrid;
};

template<typename T>
struct print_helper;
template<>
struct print_helper<living_cell>
{
    static void print()
    {
        std::cout << 'X';
    }
};
template<>
struct print_helper<dead_cell>
{
    static void print()
    {
        std::cout << '.';
    }
};
template<>
struct print_helper<type_list<>>
{
    static void print()
    {
        std::cout << std::endl;
    }
};
template<typename T, typename... Ts>
struct print_helper<type_list<T, Ts...>>
{
    static void print()
    {
        print_helper<T>::print();
        print_helper<type_list<Ts...>>::print();
    }
};

template<typename ConwayGrid>
struct printer;

template<std::size_t X, std::size_t Y, typename Grid>
struct printer<conway_grid<point<X, Y>, Grid>>
{
    using first_row = n_front<Grid, X>::type;
    constexpr static std::size_t left_over_count = X * (Y - 1);
    using left_over = conway_grid<point<X, Y - 1>, typename n_back<Grid, left_over_count>::type>;

    static void print()
    {
        print_helper<first_row>::print();
        printer<left_over>::print();
    }
};

template<std::size_t X>
struct printer<conway_grid<point<X, 0>, type_list<>>>
{
    static void print() {}
};

using my_grid = type_list<dead_cell, // Row 0
                          dead_cell,
                          dead_cell,
                          dead_cell,
                          dead_cell,
                          dead_cell, // Row 1
                          dead_cell,
                          living_cell,
                          dead_cell,
                          dead_cell,
                          dead_cell, // Row 2
                          dead_cell,
                          dead_cell,
                          living_cell,
                          dead_cell,
                          dead_cell, // Row 3
                          living_cell,
                          living_cell,
                          living_cell,
                          dead_cell,
                          dead_cell, // Row 4
                          dead_cell,
                          dead_cell,
                          dead_cell,
                          dead_cell>;
using my_size = point<5, 5>;
using the_grid = conway_grid<my_size, my_grid>;

template<std::size_t x>
using new_grid = typename conway_game<the_grid, x>::type;

int main()
{
    printer<new_grid<1>>::print();
    std::cout << std::endl;
    printer<new_grid<2>>::print();
    std::cout << std::endl;
    printer<new_grid<3>>::print();
    std::cout << std::endl;
    printer<new_grid<4>>::print();
    std::cout << std::endl;
    printer<new_grid<5>>::print();
    std::cout << std::endl;
    printer<new_grid<6>>::print();
    std::cout << std::endl;
    printer<new_grid<7>>::print();
    std::cout << std::endl;
    printer<new_grid<8>>::print();
    std::cout << std::endl;
    printer<new_grid<9>>::print();
    std::cout << std::endl;
    printer<new_grid<10>>::print();
    std::cout << std::endl;
}
