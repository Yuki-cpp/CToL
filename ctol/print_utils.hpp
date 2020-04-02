#pragma once
#include <iostream>

#include "cell_list.hpp"
#include "types.hpp"
namespace ctol
{

template<typename T>
struct print_helper;
template<>
struct print_helper<ctol::living_cell>
{
    static void print()
    {
        std::cout << 'X';
    }
};
template<>
struct print_helper<ctol::dead_cell>
{
    static void print()
    {
        std::cout << '.';
    }
};
template<>
struct print_helper<ctol::cell_list<>>
{
    static void print()
    {
        std::cout << std::endl;
    }
};
template<typename T, typename... Ts>
struct print_helper<ctol::cell_list<T, Ts...>>
{
    static void print()
    {
        print_helper<T>::print();
        print_helper<ctol::cell_list<Ts...>>::print();
    }
};

template<typename ConwayGrid>
struct printer;

template<std::size_t X, std::size_t Y, typename Grid>
struct printer<ctol::conway_grid<ctol::point<X, Y>, Grid>>
{
    using first_row = ctol::get_n_first_cells<Grid, X>::type;
    constexpr static std::size_t left_over_count = X * (Y - 1);
    using left_over =
        conway_grid<point<X, Y - 1>, typename ctol::get_n_last_cells<Grid, left_over_count>::type>;

    static void print()
    {
        ctol::print_helper<first_row>::print();
        ctol::printer<left_over>::print();
    }
};

template<std::size_t X>
struct printer<conway_grid<point<X, 0>, ctol::cell_list<>>>
{
    static void print() {}
};
} // namespace ctol