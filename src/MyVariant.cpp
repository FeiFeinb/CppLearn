//
// Created by 12784 on 2021/10/12.
//
#include <utility>
#include "Utilities/AllHeader.h"
#include "Utilities/ClassesForTest.h"

template<auto Left, auto Right>
inline constexpr bool is_left_greater_than_right = Left > Right;

template<typename Left, typename Right>
struct binary_type_compare_max : std::conditional<is_left_greater_than_right<sizeof(Left), sizeof(Right)>, Left, Right> {};

template<typename Left, typename Right>
using binary_type_compare_max_t = typename binary_type_compare_max<Left, Right>::type;

template<auto Left, auto Right>
struct binary_size_compare_max : std::conditional<is_left_greater_than_right<Left, Right>,
        std::integral_constant<std::size_t, Left>, std::integral_constant<std::size_t, Right>> {};

template<auto Left, auto Right>
inline constexpr auto binary_size_compare_max_v = binary_size_compare_max<Left, Right>::type::value;

template<typename... Ts>
struct multi_type_compare_max;

template<typename T, typename U, typename... Ts>
struct multi_type_compare_max<T, U, Ts...> : multi_type_compare_max<binary_type_compare_max_t<T, U>, Ts...> {};

template<typename T>
struct multi_type_compare_max<T> : std::integral_constant<std::size_t, sizeof(T)> {};

template<typename... Ts>
inline constexpr std::size_t multi_type_compare_max_v = multi_type_compare_max<Ts...>::value;

// 根据index获取type
template<std::size_t index, typename...>
struct my_variant_element;

template<std::size_t index, typename T, typename... Ts>
struct my_variant_element<index, T, Ts...> : my_variant_element<index - 1, Ts...> {
    static_assert(index < sizeof...(Ts) + 1, "index out of range");
};

template<typename T, typename... Ts>
struct my_variant_element<0, T, Ts...> {
    using type = T;
};

template<std::size_t index, typename... Ts>
using my_variant_element_t = typename my_variant_element<index, Ts...>::type;

// 根据type获取index
template<std::size_t, typename, typename...>
struct my_variant_index_impl;

template<std::size_t index, typename Target, typename T, typename... Ts>
struct my_variant_index_impl<index, Target, T, Ts...> : std::conditional_t<std::is_same_v<Target, T>,
        std::integral_constant<std::size_t, index>, my_variant_index_impl<index + 1, Target, Ts...>> {};

// 匹配失败
template<std::size_t index, typename T>
struct my_variant_index_impl<index, T> {
    static_assert(std::_Always_false<T>, "cant find T in args");
};

template<typename Target, typename... Ts>
inline constexpr std::size_t my_variant_index_v = my_variant_index_impl<0, Target, Ts...>::value;

template<typename Target, typename... Ts>
struct my_variant_index : std::integral_constant<std::size_t, my_variant_index_v<Target, Ts...>> {};

template<typename... Ts>
struct call_dtor_helper;

template<typename T, typename... Ts>
struct call_dtor_helper<T, Ts...> {
    void operator()(std::size_t index, void* memory)
    {
        if (index != 0)
            call_dtor_helper<Ts...>{}(index - 1, memory);
        else if (std::is_class_v<T>)
            reinterpret_cast<T*>(memory)->~T();
    }
};

template<>
struct call_dtor_helper<> {
    void operator()(std::size_t, void*) {}
};

template<typename... Ts>
class my_variant
{
public:
    void* memory;
    std::size_t index;
public:
    my_variant() : memory(nullptr), index(0) {}

    template<typename T>
    my_variant(T&& data) : my_variant() {
        store(std::forward<T>(data));
    }

    ~my_variant() {
        if (memory == nullptr)
            return;
        call_dtor_helper<Ts...>{}(index, memory);
        ::operator delete(memory);
    }

    template<typename T>
    void store(T&& data) {
        if (memory == nullptr)
            memory = ::operator new(multi_type_compare_max_v<Ts...>);
        else
            call_dtor_helper<Ts...>{}(index, memory);
        ::new(memory) T(std::forward<T>(data));
        index = my_variant_index_v<T, Ts...>;
    }

    template<typename T>
    T& get() {
        return *reinterpret_cast<T*>(memory);
    }
};

int main()
{
    my_variant<long long, NonCopyStruct_Str> v(4ll);
    std::cout << v.get<long long>() << std::endl;
    v.store<NonCopyStruct_Str>((std::string)"Jelly");
    std::cout << v.get<NonCopyStruct_Str>().name << std::endl;
}
