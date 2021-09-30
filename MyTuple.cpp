//
// Created by 12784 on 2021/9/22.
//

#include <utility>

#include "AllHeader.h"

class TestClass
{
public:
    TestClass() { std::cout << "default" << std::endl; }
    TestClass(std::string&&  _data) : data(std::move(_data)) { std::cout << "param" << std::endl; }

    TestClass(const TestClass& t) : data(t.data) { std::cout << "copy" << std::endl; }
    TestClass(TestClass&& t) noexcept : data(std::move(t.data)) { std::cout << "move" << std::endl; }

    TestClass& operator=(const TestClass& t)
    {
        data = t.data;
        std::cout << "operator= copy" << std::endl;
        return *this;
    }

    TestClass& operator=(TestClass&& t) noexcept
    {
        data = std::move(t.data);
        std::cout << "operator= move" << std::endl;
        return *this;
    }

    inline constexpr std::string& get() { return data; }
private:
    std::string data;
};

template<typename>
struct my_tuple_size;

template<template<typename...> typename TupleType, typename... Ts>
struct my_tuple_size<TupleType<Ts...>> : std::integral_constant<std::size_t, sizeof...(Ts)> {};

template<typename TupleType>
inline constexpr std::size_t my_tuple_size_value = my_tuple_size<TupleType>::value;


template<std::size_t, typename>
struct my_tuple_element;

template<std::size_t index, template<typename...> typename TupleType, typename T, typename... Ts>
struct my_tuple_element<index, TupleType<T, Ts...>> : my_tuple_element<index - 1, TupleType<Ts...>> {};

template<template<typename...> typename TupleType, typename T, typename... Ts>
struct my_tuple_element<0, TupleType<T, Ts...>> {
    using type = T;
};

template<std::size_t index, typename TupleType>
using my_tuple_element_type = typename my_tuple_element<index, TupleType>::type;

template<typename T>
struct remove_ref_wrap {
    using type = T;
};
template<typename T>
struct remove_ref_wrap<std::reference_wrapper<T>> {
    using type = T&;
};

template<typename T>
using with_ref_decay_t = typename remove_ref_wrap<std::decay_t<T>>::type;


template<typename... Ts>
class MyTuple;

template<>
class MyTuple<> {};

template<typename... Ts>
constexpr MyTuple<with_ref_decay_t<Ts>...> my_make_tuple(Ts&&... args) {
    return MyTuple<with_ref_decay_t<Ts>...>(std::forward<Ts>(args)...);
}

template<typename TupleType, typename FuncType, std::size_t... Index>
void call_tuple(const TupleType& t, const FuncType& f, std::index_sequence<Index...>) {
    (f(t.template get<Index>()), ...);
}

template<template<typename...> typename TupleType, typename... Ts, typename FuncType>
void my_traverse_tuple(const TupleType<Ts...>& t, const FuncType& f) {
    call_tuple(t, f, std::make_index_sequence<my_tuple_size_value<TupleType<Ts...>>>{});
}

template<typename T, typename... Ts>
class MyTuple<T, Ts...> : public MyTuple<Ts...>
{
private:
    T data;
    using TopTuple = MyTuple<Ts...>;

    template<typename TupleType, std::size_t... Indices>
    MyTuple(TupleType&& _copy, std::index_sequence<Indices...>) : MyTuple(_copy.template get<Indices>()...) {}

public:
    MyTuple() = default;

    template<typename ThisType, typename... RestTypes>
    MyTuple(ThisType&& _data, RestTypes&&... _args) : data(std::forward<ThisType>(_data)), TopTuple(std::forward<RestTypes>(_args)...) {}

    template<template<typename...> typename TupleType, typename... RestTypes>
    MyTuple(TupleType<RestTypes...>&& _copy) : MyTuple(std::forward<TupleType<RestTypes...>>(_copy), std::make_index_sequence<my_tuple_size_value<TupleType<RestTypes...>>>{}) {}

    template<std::size_t index>
    constexpr auto& get() const
    {
        static_assert(index <= sizeof...(Ts), "out of range");
        if constexpr (index == 0)
            return data;
        else
            return TopTuple::template get<index - 1>();
    }

    template<std::size_t index>
    constexpr auto& get()
    {
        using element_type = my_tuple_element_type<index, MyTuple<T, Ts...>>;
        return const_cast<element_type&>((static_cast<const MyTuple<T, Ts...>&>(*this)).template get<index>());
    }
};



