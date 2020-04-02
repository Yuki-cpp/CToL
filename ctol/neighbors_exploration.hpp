#pragma once

#include "cell_list.hpp"
#include "types.hpp"

namespace ctol
{
template<typename Size, typename Position>
struct y_increase
{
    constexpr static std::size_t Y =
        std::conditional_t<Position::y == 0,
                           std::integral_constant<std::size_t, Size::y - 1>,
                           std::integral_constant<std::size_t, Position::y - 1>>::value;
};

template<typename Position>
struct y_keep
{
    constexpr static std::size_t Y = Position::y;
};

template<typename Size, typename Position>
struct y_decrease
{
    constexpr static std::size_t Y =
        std::conditional_t<Position::y == Size::y - 1,
                           std::integral_constant<std::size_t, 0>,
                           std::integral_constant<std::size_t, Position::y + 1>>::value;
};

template<typename Size, typename Position>
struct x_increase
{
    constexpr static std::size_t X =
        std::conditional_t<Position::x == Size::x - 1,
                           std::integral_constant<std::size_t, 0>,
                           std::integral_constant<std::size_t, Position::x + 1>>::value;
};

template<typename Position>
struct x_keep
{
    constexpr static std::size_t X = Position::x;
};

template<typename Size, typename Position>
struct x_decrease
{
    constexpr static std::size_t X =
        std::conditional_t<Position::x == 0,
                           std::integral_constant<std::size_t, Size::x - 1>,
                           std::integral_constant<std::size_t, Position::x - 1>>::value;
};

template<typename Size, typename Position>
struct get_index_of
{
    static constexpr std::size_t value = Position::x + (Size::x * Position::y);
};
template<typename Size, std::size_t index>
struct get_position_of
{
    using type = ctol::point<index % Size::x, index / Size::x>;
};

template<typename Size, typename Position, typename Grid>
struct up: ctol::x_keep<Position>, ctol::y_increase<Size, Position>
{
    using type = ctol::get<Grid, ctol::get_index_of<Size, ctol::point<up::X, up::Y>>::value>::type;
};

template<typename Size, typename Position, typename Grid>
struct up_right: ctol::x_increase<Size, Position>, ctol::y_increase<Size, Position>
{
    using type =
        get<Grid, ctol::get_index_of<Size, ctol::point<up_right::X, up_right::Y>>::value>::type;
};

template<typename Size, typename Position, typename Grid>
struct right: ctol::x_increase<Size, Position>, ctol::y_keep<Position>
{
    using type =
        ctol::get<Grid, ctol::get_index_of<Size, ctol::point<right::X, right::Y>>::value>::type;
};

template<typename Size, typename Position, typename Grid>
struct down_right: x_increase<Size, Position>, y_decrease<Size, Position>
{
    using type =
        ctol::get<Grid,
                  ctol::get_index_of<Size, ctol::point<down_right::X, down_right::Y>>::value>::type;
};

template<typename Size, typename Position, typename Grid>
struct down: ctol::x_keep<Position>, ctol::y_decrease<Size, Position>
{
    using type =
        ctol::get<Grid, ctol::get_index_of<Size, ctol::point<down::X, down::Y>>::value>::type;
};

template<typename Size, typename Position, typename Grid>
struct down_left: ctol::x_decrease<Size, Position>, ctol::y_decrease<Size, Position>
{
    using type =
        ctol::get<Grid,
                  ctol::get_index_of<Size, ctol::point<down_left::X, down_left::Y>>::value>::type;
};

template<typename Size, typename Position, typename Grid>
struct left: ctol::x_decrease<Size, Position>, ctol::y_keep<Position>
{
    using type = ctol::get<Grid, ctol::get_index_of<Size, point<left::X, left::Y>>::value>::type;
};

template<typename Size, typename Position, typename Grid>
struct up_left: ctol::x_decrease<Size, Position>, ctol::y_increase<Size, Position>
{
    using type =
        ctol::get<Grid, ctol::get_index_of<Size, point<up_left::X, up_left::Y>>::value>::type;
};

template<typename Size, typename Position, typename Grid>
struct get_neighbors
{
    using type = ctol::cell_list<typename ctol::up<Size, Position, Grid>::type,
                                 typename ctol::up_right<Size, Position, Grid>::type,
                                 typename ctol::right<Size, Position, Grid>::type,
                                 typename ctol::down_right<Size, Position, Grid>::type,
                                 typename ctol::down<Size, Position, Grid>::type,
                                 typename ctol::down_left<Size, Position, Grid>::type,
                                 typename ctol::left<Size, Position, Grid>::type,
                                 typename ctol::up_left<Size, Position, Grid>::type>;
};

} // namespace ctol