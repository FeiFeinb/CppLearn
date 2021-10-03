#ifndef CPPLEARN_UTILITYTEMPLATE_H
#define CPPLEARN_UTILITYTEMPLATE_H

#include "AllHeader.h"

template<class MotionType>
class Singleton
{
public:
    Singleton() = delete;
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    static MotionType& GetInstance()
    {
        static MotionType pInstance;
        return pInstance;
    }
};

template<typename T = void>
struct pair_hash
{
    std::size_t operator()(const std::pair<T, T>& p) const
    {
        return std::hash<T>{}(p.first) ^ std::hash<T>{}(p.second);
    }
};

template<>
struct pair_hash<void>
{
    template<typename First, typename Second>
    std::size_t operator()(const std::pair<First, Second>& p) const
    {
        return std::hash<First>{}(p.first) ^ std::hash<Second>{}(p.second);
    }
};

template<typename T>
class RandomNum
{
private:
    friend class Singleton<RandomNum<T>>;
    std::random_device rd;
    std::mt19937 g;
    std::unordered_map<std::pair<T, T>, std::uniform_int_distribution<T>, pair_hash<>> map;
public:
    RandomNum() : rd(), g(rd())  {}

    T get_random(T min, T max)
    {
        auto pair = std::make_pair(min, max);
        if (map.find(pair) == map.end())
            map.insert_or_assign(pair, std::uniform_int_distribution<T>(min, max));
        return map[pair](g);
    }

};

template<template<typename> typename Container, typename Element>
Container<Element> generate_random_container(std::size_t size, Element min, Element max)
{
    Container<Element> result;
    result.reserve(size);
    std::generate_n(std::back_inserter(result), size, [&min, &max]() { return Singleton<RandomNum<Element>>::GetInstance().get_random(min, max); });
    return result;
}

template<typename Container, typename TInterval>
void printer_container(const Container& cont, TInterval t)
{
    using elementType = typename Container::iterator::value_type;
    std::copy(cont.cbegin(), cont.cend(), std::ostream_iterator<elementType>(std::cout, t));
}

template<typename Container>
void print_container_space(const Container& cont) {
    printer_container(cont, " ");
}

template<typename Container>
void print_container_line(const Container& cont) {
    printer_container(cont, "\n");
}

template<typename Container>
void print_container_endl(const Container& cont) {
    printer_container(cont, std::endl);
}

#endif