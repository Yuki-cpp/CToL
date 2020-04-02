#include <iostream>

#include "ctol/ctol.hpp"

using grid = ctol::cell_list<ctol::dead_cell, // Row 0
                             ctol::dead_cell,
                             ctol::dead_cell,
                             ctol::dead_cell,
                             ctol::dead_cell,
                             ctol::dead_cell, // Row 1
                             ctol::dead_cell,
                             ctol::living_cell,
                             ctol::dead_cell,
                             ctol::dead_cell,
                             ctol::dead_cell, // Row 2
                             ctol::dead_cell,
                             ctol::dead_cell,
                             ctol::living_cell,
                             ctol::dead_cell,
                             ctol::dead_cell, // Row 3
                             ctol::living_cell,
                             ctol::living_cell,
                             ctol::living_cell,
                             ctol::dead_cell,
                             ctol::dead_cell, // Row 4
                             ctol::dead_cell,
                             ctol::dead_cell,
                             ctol::dead_cell,
                             ctol::dead_cell>;
using size = ctol::point<5, 5>;
using game_grid = ctol::conway_grid<grid, size>;

template<std::size_t Iterations>
using evolved_grid = typename ctol::conway_game<game_grid, Iterations>::type;

int main()
{
    ctol::printer<evolved_grid<1>>::print();
    std::cout << std::endl;
    ctol::printer<evolved_grid<2>>::print();
    std::cout << std::endl;
    ctol::printer<evolved_grid<3>>::print();
    std::cout << std::endl;
    ctol::printer<evolved_grid<4>>::print();
    std::cout << std::endl;
    ctol::printer<evolved_grid<5>>::print();
    std::cout << std::endl;
    ctol::printer<evolved_grid<6>>::print();
    std::cout << std::endl;
    ctol::printer<evolved_grid<7>>::print();
    std::cout << std::endl;
    ctol::printer<evolved_grid<8>>::print();
    std::cout << std::endl;
    ctol::printer<evolved_grid<9>>::print();
    std::cout << std::endl;
    ctol::printer<evolved_grid<10>>::print();
    std::cout << std::endl;
}
