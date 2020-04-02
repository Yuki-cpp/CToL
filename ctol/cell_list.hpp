#pragma once

#include <type_traits>
#include <utility>
namespace ctol
{
template<typename... Ts>
struct cell_list
{
    using type = cell_list;
    constexpr static std::size_t size = sizeof...(Ts);
};

template<>
struct cell_list<>
{
    constexpr static std::size_t size = 0;
};

template<typename CellListA, typename CellListB>
struct concatenate;
template<typename... As, typename... Bs>
struct concatenate<cell_list<As...>, cell_list<Bs...>>
{
    using type = cell_list<As..., Bs...>;
};

template<typename CellList, typename Cell>
struct append;
template<typename... Ts, typename C>
struct append<cell_list<Ts...>, C>
{
    using type = cell_list<Ts..., C>;
};

template<typename CellList, std::size_t N>
struct get_n_first_cells;
template<typename T, typename... Ts>
struct get_n_first_cells<cell_list<T, Ts...>, 1>
{
    using type = cell_list<T>;
};
template<std::size_t N>
struct get_n_first_cells<cell_list<>, N>
{
    using type = cell_list<>;
};
template<typename T, typename... Ts, std::size_t N>
struct get_n_first_cells<cell_list<T, Ts...>, N>
{
    using type =
        typename concatenate<cell_list<T>,
                             typename get_n_first_cells<cell_list<Ts...>, N - 1>::type>::type;
};

template<typename CellList, std::size_t N>
struct get_n_last_cells;
template<typename T, typename... Ts, std::size_t N>
struct get_n_last_cells<cell_list<T, Ts...>, N>
{
    using type =
        typename std::conditional<N == sizeof...(Ts) + 1,
                                  cell_list<T, Ts...>,
                                  typename get_n_last_cells<cell_list<Ts...>, N>::type>::type;
};
template<std::size_t N>
struct get_n_last_cells<cell_list<>, N>
{
    using type = cell_list<>;
};

template<typename CellList, std::size_t Index>
struct get
{
    using type = void;
};
template<typename T, typename... Ts>
struct get<cell_list<T, Ts...>, 0>
{
    using type = T;
};
template<typename T, typename... Ts, std::size_t Index>
struct get<cell_list<T, Ts...>, Index>
{
    using type = typename get<cell_list<Ts...>, Index - 1>::type;
};

template<typename CellList, std::size_t Index, typename T>
struct set
{
    private:
    using front_cells = get_n_first_cells<CellList, Index>::type;
    using back_cells = get_n_last_cells<CellList, CellList::size - (Index + 1)>::type;

    public:
    using type = concatenate<typename append<front_cells, T>::type, back_cells>::type;
};

} // namespace ctol