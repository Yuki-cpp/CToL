#pragma once
#include <type_traits>
#include <utility>

template<typename... Ts>
struct type_list
{
    using type = type_list;
    constexpr static std::size_t size = sizeof...(Ts);
};
template<>
struct type_list<>
{
    constexpr static std::size_t size = 0;
};

template<typename TypeListA, typename TypeListB>
struct extend;
template<typename... As, typename... Bs>
struct extend<type_list<As...>, type_list<Bs...>>
{
    using type = type_list<As..., Bs...>;
};

template<typename TypeList, typename Type>
struct append;
template<typename... Ts, typename Type>
struct append<type_list<Ts...>, Type>
{
    using type = type_list<Ts..., Type>;
};

template<typename TypeList, std::size_t n>
struct n_front;
template<typename T, typename... Ts>
struct n_front<type_list<T, Ts...>, 1>
{
    using type = type_list<T>;
};
template<std::size_t n>
struct n_front<type_list<>, n>
{
    using type = type_list<>;
};
template<typename T, typename... Ts, std::size_t n>
struct n_front<type_list<T, Ts...>, n>
{
    using type =
        typename extend<type_list<T>, typename n_front<type_list<Ts...>, n - 1>::type>::type;
};

template<typename TypeList, std::size_t n>
struct n_back_helper;
template<typename T, typename... Ts, std::size_t n>
struct n_back_helper<type_list<T, Ts...>, n>
{
    using type = typename std::conditional<n == sizeof...(Ts) + 1,
                                           type_list<T, Ts...>,
                                           typename n_back_helper<type_list<Ts...>, n>::type>::type;
};
template<std::size_t n>
struct n_back_helper<type_list<>, n>
{
    using type = type_list<>;
};
template<typename TypeList, std::size_t n>
struct n_back
{
    using type = typename n_back_helper<TypeList, n>::type;
};
template<std::size_t n>
struct n_back<type_list<>, n>
{
    using type = type_list<>;
};

template<typename TypeList, std::size_t index>
struct get
{
    using type = std::nullptr_t;
};
template<typename T, typename... Ts>
struct get<type_list<T, Ts...>, 0>
{
    using type = T;
};
template<typename T, typename... Ts, std::size_t index>
struct get<type_list<T, Ts...>, index>
{
    using type = typename get<type_list<Ts...>, index - 1>::type;
};

template<typename TypeList, std::size_t index, typename T>
struct set
{
    using type =
        typename extend<typename append<typename n_front<TypeList, index>::type, T>::type,
                        typename n_back<TypeList, TypeList::size - (index + 1)>::type>::type;
};