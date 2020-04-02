#pragma once

#include <utility>

namespace ctol
{

template<bool Alive>
struct cell
{
    constexpr static bool value = Alive;
};
using living_cell = cell<true>;
using dead_cell = cell<false>;

template<std::size_t X, std::size_t Y>
struct point
{
    constexpr static std::size_t x = X;
    constexpr static std::size_t y = Y;
};

template<typename Grid, typename Size>
struct conway_grid
{
    using grid = Grid;
    using size = Size;
};

} // namespace ctol