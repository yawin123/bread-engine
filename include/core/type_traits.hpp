#include <cstdint>
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
                struct type_from_typename_list { static_assert(sizeof...(Ts) > 0), "Out of range"; };

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

        //Typelist
            template <typename... Ts>
            struct Typelist {
                consteval static std::size_t size() noexcept { return sizeof...(Ts); }

                template <typename T>
                consteval static bool contains() noexcept { return ( false || ... || is_same_type<T,Ts> ); }

                template <typename T>
                consteval static std::size_t position() noexcept
                {
                    static_assert(contains<T>());
                    return type_position<T, Ts...>;
                }
            };

        //Typelist traits
            template <typename TYPELIST>
            struct TypeTraits {
                using mask_type = conditional_type<(TYPELIST::size() <= 8),  uint8_t,
                                  conditional_type<(TYPELIST::size() <= 16), uint16_t,
                                                                             uint32_t
                                  >>;

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
            struct replace_template<N, Typelist<Ts...>> {
                using type = N<Ts...>;
            };
            template <template <typename...> class N, typename L>
            using replace = typename replace_template<N,L>::type;
    };
  };
};
