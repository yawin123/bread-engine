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

            template <typename T>
            struct is_type { using type = T; };

        //Type comparison
            template <typename T, typename U>
            struct is_same : is_false {};

            template <typename T>
            struct is_same<T,T> : is_true{};

            template <typename T, typename U>
            constexpr bool is_same_type = is_same<T,U>::value;

        //From list
            template<std::size_t N, typename... Ts>
            struct type_from_typename_list { static_assert(sizeof...(Ts) > 0); };

            template<std::size_t N, typename... Ts>
            using type_from_list = typename type_from_typename_list<N, Ts...>::type;

            template<typename T, typename... Ts>
            struct type_from_typename_list<0, T, Ts...> : is_type<T> {};

            template<std::size_t N, typename T, typename... Ts>
            struct type_from_typename_list<N, T, Ts...> : is_type<type_from_list<N-1, Ts...>>{};


        //Typelist
            template <typename... Ts>
            struct Typelist {
                consteval static std::size_t size() noexcept { return sizeof...(Ts); }

                template <typename T>
                consteval static bool contains() noexcept { return ( false || ... || is_same_type<T,Ts> ); }

                template <typename T>
                consteval static std::size_t position() noexcept { return 1; }
            };

        //Typelist traits
            template <typename TYPELIST>
            struct TypeTraits {
                consteval static std::size_t size() noexcept { return TYPELIST::size(); }

                template<typename T>
                consteval static std::size_t id() noexcept
                {
                    static_assert(TYPELIST::template contains<T>(), "Type T must be in TYPELIST");
                    return 1;

                }

                template<typename T>
                consteval static std::size_t mask() noexcept { return 1 << id<T>(); }
            };
    };
  };
};
