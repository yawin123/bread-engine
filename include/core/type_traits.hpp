#pragma once
#include <cstdint>
#include <type_traits>
#include <utility>

namespace brd
{
  namespace core
  {
    namespace type_traits
    {
        //Constants
            template <typename T, T VAL>
            struct constant { static constexpr T value { VAL }; };

            struct is_false : constant<bool, false>{};
            struct is_true : constant<bool, true>{};

            template<size_t P>
            struct is_position : constant<size_t, P>{};

            template <typename T>
            struct is_type { using type = T; };

        //Type comparison
            template <typename T, typename U>
            struct is_same : is_false {};

            template <typename T>
            struct is_same<T,T> : is_true{};

            template <typename T, typename U>
            constexpr bool is_same_type = is_same<T,U>::value;

        //Tipos condicionales
            template<bool C, typename T, typename F>
            struct get_conditional_type : is_type<F> {};

            template<typename T, typename F>
            struct get_conditional_type<true, T, F> : is_type<T> {};

            template<bool C, typename T, typename F>
            using conditional_type = typename get_conditional_type<C,T,F>::type;

        //Manipulación de listas de tipos
            //Dada una posición quiero el tipo
                template<std::size_t N, typename... Ts>
                struct type_from_typename_list { static_assert(sizeof...(Ts) > 0, "Out of range"); };

                template<std::size_t N, typename... Ts>
                using type_from_list = typename type_from_typename_list<N, Ts...>::type;

                template<typename T, typename... Ts>
                struct type_from_typename_list<0, T, Ts...> : is_type<T> {};

                template<std::size_t N, typename T, typename... Ts>
                struct type_from_typename_list<N, T, Ts...> : is_type<type_from_list<N-1, Ts...>>{};

            //Dado un tipo, quiero la posición
                template<typename T, typename... Ts>
                struct position_from_typename_list { static_assert(sizeof...(Ts) != 0, "Type must be in typelist"); };

                template<typename T, typename... Ts>
                constexpr size_t type_position = position_from_typename_list<T, Ts...>::value;

                template<typename T, typename... Ts>
                struct position_from_typename_list<T, T, Ts...> : is_position<0> {};

                template<typename T, typename U, typename... Ts>
                struct position_from_typename_list<T, U, Ts...> : is_position< 1 + type_position<T, Ts...> >{};

        //type_list
            template <typename... Ts>
            struct type_list {
                consteval static std::size_t size() noexcept { return sizeof...(Ts); }

                template <typename T>
                consteval static bool contains() noexcept { return ( false || ... || is_same_type<T,Ts> ); }

                template <typename T>
                consteval static std::size_t position() noexcept
                {
                    static_assert(contains<T>(), "type_list must contain T");
                    return type_position<T, Ts...>;
                }

                template<typename... Others>
                constexpr type_list<Ts..., Others...> operator+(type_list<Others...>) { return {}; }
            };

        //type_list head
            template <typename... Ts>
            struct extract_head;
            template <typename H, typename... Tail>
            struct extract_head<type_list<H,Tail...>> {
                using type = H;
            };
            template <typename TL>
            using head = typename extract_head<TL>::type;

        //type_list push
            template <typename E, typename TL>
            struct front_push;
            template <typename E, typename... Ts>
            struct front_push<E, type_list<Ts...>> {
                using type = type_list<E, Ts...>;
            };
            template <typename E, typename TL>
            using push_front = typename front_push<E, TL>::type;

            template <typename E, typename TL>
            struct back_push;
            template <typename E, typename... Ts>
            struct back_push<E, type_list<Ts...>> {
                using type = type_list<Ts...,E>;
            };
            template <typename E, typename TL>
            using push_back= typename back_push<E, TL>::type;

        //type_list pop
            template <typename... Ts>
            struct front_pop;
            template <typename H, typename... Tail>
            struct front_pop<type_list<H,Tail...>> {
                using type = type_list<Tail...>;
            };
            template <typename TL>
            using pop = typename front_pop<TL>::type;

        //type_list traits
            template <typename TYPELIST>
            struct traits {
                using mask_type = conditional_type<(TYPELIST::size() <= 8),  uint8_t,
                                  conditional_type<(TYPELIST::size() <= 16), uint16_t,
                                  conditional_type<(TYPELIST::size() <= 32), uint32_t,
                                                                             uint64_t
                                  >>>;

                consteval static uint8_t size() noexcept { return TYPELIST::size(); }

                template<typename T>
                consteval static uint8_t id() noexcept
                {
                    static_assert(TYPELIST::template contains<T>(), "Type T must be in TYPELIST");
                    return TYPELIST::template position<T>();
                }

                template<typename T>
                consteval static mask_type mask() noexcept { return 1 << id<T>(); }
            };

        //Template replace
            template <template <typename...> class N, typename L>
            struct replace_template {};
            template <template <typename...> class N, typename... Ts>
            struct replace_template<N, type_list<Ts...>> {
                using type = N<Ts...>;
            };
            template <template <typename...> class N, typename L>
            using make_container = typename replace_template<N,L>::type;

        //Apply foreach
            template<template <typename...> class N, typename L>
            struct apply_foreach {};

            template<template <typename...> class N, typename L>
            using foreach_make_container = typename apply_foreach< N, L>::type;

            template <template <typename...> class N, typename T>
            struct apply_foreach<N, type_list<T>> {
                using type = type_list<N<T>>;
            };

            template <template <typename...> class N, typename T, typename... Ts>
            struct apply_foreach<N, type_list<T, Ts...>> {
                using type = push_front< N<T>, foreach_make_container< N, type_list<Ts...> > >;
              };

        //Apply foreach
            template<typename N, typename L>
            struct replace_foreach {};

            template<typename N, typename L>
            using foreach_make_replace = typename replace_foreach< N, L>::type;

            template <typename N, typename T>
            struct replace_foreach<N, type_list<T>> {
              using type = type_list<N>;
            };

            template <typename N, typename T, typename... Ts>
            struct replace_foreach<N, type_list<T, Ts...>> {
              using type = push_front< N, foreach_make_replace< N, type_list<Ts...> > >;
            };
    };
  };
};
