#include <utility>

#include "Utilities/AllHeader.h"

#include "Utilities/UtilityClasses.h"
#include "Utilities/ClassesForTest.h"

template<typename T>
void quick_sort(T& vec, std::int64_t l, std::int64_t r)
{
    if (l >= r) return;
    std::int64_t i = l - 1, j = r + 1, x = vec[l];
    while (i < j)
    {
        while (vec[++i] < x);
        while (vec[--j] > x);
        if (i < j)
            std::swap(vec[i], vec[j]);
    }
    quick_sort(vec, l, j);
    quick_sort(vec, j + 1, r);
}

template<typename T>
void single_thread_quick_sort(T& vec)
{
    quick_sort(vec, 0, vec.size() - 1);
}

class SimpleThreadPool
{
private:
public:
    std::vector<std::thread> pool;

    SimpleThreadPool() = default;

    SimpleThreadPool(std::size_t amount)
    {
        pool.reserve(amount);
    }

    ~SimpleThreadPool()
    {
        join_all();
    }

    template<typename... T>
    void add(T&& ... args)
    {
        pool.emplace_back(std::forward<T>(args)...);
    }

    void join_all()
    {
        for (auto& t: pool)
            if (t.joinable())
                t.join();
    }

    void clear()
    {
        pool.clear();
    }
};

template<typename T>
void merge(T& vec, const T& cached, const std::tuple<std::size_t, std::size_t, std::size_t>& t)
{
    const auto&[l, mid, r] = t;
    if (l >= r)
        return;
    std::size_t i = l, j = mid + 1, resultIndex = l;
    while (i <= mid && j <= r)
    {
        if (cached[i] <= cached[j])
            vec[resultIndex++] = cached[i++];
        else
            vec[resultIndex++] = cached[j++];
    }
    while (i <= mid)
        vec[resultIndex++] = cached[i++];
    while (j <= r)
        vec[resultIndex++] = cached[j++];
}

// 多线程快排
template<typename T>
void multi_thread_quick_sort(T& vec)
{
    // 获取当前最大可用的线程数量
    std::size_t threadAmount = std::thread::hardware_concurrency();
    SimpleThreadPool threadPool(threadAmount);

    // 记录快排区间
    std::vector<std::pair<std::size_t, std::size_t>> sortArea;
    sortArea.reserve(threadAmount);

    // 多线程分组快排
    for (std::size_t i = 0; i < threadAmount; i++)
    {
        std::size_t lengthPerThread = vec.size() / threadAmount;
        sortArea.emplace_back(i * lengthPerThread, (i + 1) * lengthPerThread - 1);
        // 对结果进行特殊处理
        if (i == threadAmount - 1)
            sortArea.back().second = vec.size() - 1;
        threadPool.add(quick_sort<T>, std::ref(vec), sortArea.back().first, sortArea.back().second);
    }
    threadPool.join_all();
    // 开始归并操作
    for (threadAmount /= 2; threadAmount != 0; threadAmount /= 2)
    {
        T result(vec.size());
        threadPool.clear();
        // 处理归并所需要的线程数量
        // 每条线程应该处理的数据数量
        for (std::size_t j = 0; j < threadAmount; j++)
        {
            std::size_t startIndex = j * sortArea.size() / threadAmount;
            std::size_t endIndex = (j + 1) * sortArea.size() / threadAmount - 1;
            auto start = sortArea[startIndex].first;
            auto mid = sortArea[(startIndex + endIndex) / 2].second;
            auto end = sortArea[endIndex].second;
            threadPool.add(merge<T>, std::ref(result), std::ref(vec), std::make_tuple(start, mid, end));
        }
        threadPool.join_all();
        vec = std::move(result);
    }
}

void test_multi_sort()
{
    auto vec1 = generate_random_container_single_thread<std::vector>(10, 0, 1000000);
    auto vec2 = vec1;
    single_thread_quick_sort(vec1);
    std::cout << std::endl;
    multi_thread_quick_sort(vec2);
    std::cout << std::boolalpha << std::equal(vec1.begin(), vec1.end(), vec2.begin(), vec2.end());
}

void test_generate_random_vec()
{
    {
        InTime i1;
        auto vec1 = generate_random_container_single_thread<std::vector>(10000000, 0, 1000000);
        i1.Stop();
    }
    {
        InTime i2;
        auto vec2 = generate_random_container_multi_thread<std::vector>(10000000, 0, 1000000);
        i2.Stop();
    }
}

template<template<typename> typename Container, typename Element>
Element find_max_num_single_thread(const Container<Element>& container)
{
    std::size_t size = container.size();
    Element result = container[0];
    for (std::size_t i = 0; i < size; i++)
        result = std::max(container[i], result);
    return result;
}

template<template<typename> typename Container, typename Element>
Element find_max_num_multi_thread(const Container<Element>& vec)
{
    std::size_t threadAmount = std::thread::hardware_concurrency();
    std::size_t handleLength = vec.size() / threadAmount;
    std::vector<std::thread> threadVec;
    std::vector<Element> maxVec(threadAmount);
    for (std::size_t i = 0; i < threadAmount; i++)
        threadVec.emplace_back([&maxVec, &vec, handleLength](std::size_t index)
                               {
                                   maxVec[index] = *std::max_element(vec.begin() + index * handleLength,
                                                                     vec.begin() + (index + 1) * handleLength - 1);
                               }, i);
    for (auto& t: threadVec)
        if (t.joinable())
            t.join();
    return *std::max_element(maxVec.begin(), maxVec.end());
}

void test_find_max_num()
{
    auto vec = generate_random_container_multi_thread<std::vector>(10000000, 0, 100000000);
    InTime it;
    std::cout << find_max_num_single_thread<std::vector>(vec) << std::endl;
    it.Stop();
    it.ReStart();
    std::cout << find_max_num_multi_thread<std::vector>(vec) << std::endl;
    it.Stop();
}

void test_atomic()
{
    InTime time;
    std::atomic<int> count{0};
    std::vector<std::thread> threadVec;
    threadVec.reserve(8);
    for (int i = 0; i < 8; i++)
    {
        threadVec.emplace_back([&]()
                               {
                                   for (int j = 0; j < 1000000; j++)
                                       count++;
                               });
    }
    for (auto& t: threadVec)
        if (t.joinable())
            t.join();
    std::cout << count << std::endl;
}

void test_atomic_cas()
{
    InTime time;
    std::atomic<int> count{0};
    std::vector<std::thread> threadVec;
    threadVec.reserve(8);
    for (int i = 0; i < 8; i++)
    {
        threadVec.emplace_back([&]()
                               {
                                   for (int j = 0; j < 1000000; j++)
                                   {
                                       int oldData = count.load();
                                       while (!std::atomic_compare_exchange_weak(&count, &oldData, oldData + 1));
                                   }
                               });
    }
    for (auto& t: threadVec)
        if (t.joinable())
            t.join();
    std::cout << count << std::endl;
}

void test_mutex()
{
    InTime time;
    int count = 0;
    std::vector<std::thread> threadVec;
    threadVec.reserve(8);
    std::mutex mutexLock;
    for (int i = 0; i < 8; i++)
    {
        threadVec.emplace_back([&]()
                               {
                                   for (int j = 0; j < 1000000; j++)
                                   {
                                       mutexLock.lock();
                                       count++;
                                       mutexLock.unlock();
                                   }
                               });
    }

    for (auto& t: threadVec)
        if (t.joinable())
            t.join();
    std::cout << count << std::endl;
}

class AtomicSender
{
private:
    std::atomic<std::uint64_t> prime_count;
    std::atomic<std::uint64_t> cur_number;
    std::uint64_t expected_index;

    void find_prime()
    {
        std::uint64_t t_number = 0;
        do
        {
            // return old data
            t_number = std::atomic_fetch_add(&cur_number, 1);
            if (!(t_number <= expected_index))
                break;
            bool is_prime = true;
            for (std::uint64_t i = 2; i * i <= t_number; ++i)
            {
                if (t_number % i == 0)
                {
                    is_prime = false;
                    break;
                }
            }
            if (is_prime)
                std::atomic_fetch_add(&prime_count, 1);
        } while (t_number <= expected_index);
    }

public:
    AtomicSender() : prime_count(0), cur_number(2), expected_index(0) {}

    void multi_thread_find_prime()
    {
        std::cin >> expected_index;
        std::cout << "Find the prime number count less than" << expected_index << std::endl;
        std::size_t threadMaxAmount = std::thread::hardware_concurrency();
        SimpleThreadPool threadPool(threadMaxAmount);
        for (std::size_t i = 0; i < threadMaxAmount; i++)
            threadPool.add(&AtomicSender::find_prime, this);
        threadPool.join_all();
        std::cout << prime_count.load() << std::endl;
    }
};

struct PlaceHolderTag
{
};
struct FunctorTag
{
};
struct BuildInTypeTag
{
};

template<typename T, typename Holder = PlaceHolderTag>
struct function_traits;

template<typename T, typename... Ts>
struct function_traits<T(Ts...)>
{
    using ReturnType = T;
    using FunctionType = T(Ts...);
    using FunctionPointer = T(*)(Ts...);

    template<std::size_t I>
    struct args
    {
        static_assert(I < sizeof...(Ts), "args index out of range");
        using ParamType = std::tuple_element_t<I, std::tuple<Ts...>>;
    };
};

// std::function类型
template<typename T, typename... Ts>
struct function_traits<std::function<T(Ts...)>> : public function_traits<T(Ts...)>
{
};

// 函数指针
template<typename T, typename... Ts>
struct function_traits<T(*)(Ts...)> : public function_traits<T(Ts...)>
{
};

// 成员函数
#define MEMBER_FUNCTION_TRAITS(...) \
template<typename ClassType, typename T, typename...Ts> \
struct function_traits<T (ClassType::*)(Ts...) __VA_ARGS__> : public function_traits<T(Ts...)> {}; \

MEMBER_FUNCTION_TRAITS()
MEMBER_FUNCTION_TRAITS(const)
MEMBER_FUNCTION_TRAITS(volatile)
MEMBER_FUNCTION_TRAITS(const volatile)

// 通过标签分发内置类型与仿函数
template<typename T, typename Holder>
struct function_traits : public function_traits<T, std::conditional_t<std::is_class_v<T>, FunctorTag, BuildInTypeTag>>
{
};

// 内置类型
template<typename BuildInType>
struct function_traits<BuildInType, BuildInTypeTag>
{
    static_assert(std::_Always_false<BuildInType>, "this is just a build in type");
};

// 仿函数
template<typename ClassType>
struct function_traits<ClassType, FunctorTag> : public function_traits<decltype(&ClassType::operator())>
{
};

#define PRINT_TYPEID(T) std::cout << typeid(T).name() << std::endl

struct Test_Functor
{
    void non_const_func(double) {}

    void const_func(int) const {}
};
