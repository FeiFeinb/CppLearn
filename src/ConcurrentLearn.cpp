#include "Utilities/AllHeader.h"

#include "Utilities/UtilityClasses.h"

template<typename T>
void quick_sort(T &vec, long long l, long long r) {
    if (l >= r) return;
    long long i = l - 1, j = r + 1, x = vec[l];
    while (i < j) {
        while (vec[++i] < x);
        while (vec[--j] > x);
        if (i < j)
            std::swap(vec[i], vec[j]);
    }
    quick_sort(vec, l, j);
    quick_sort(vec, j + 1, r);
}

template<typename T>
void single_thread_quick_sort(T &vec) {
    quick_sort(vec, 0, vec.size() - 1);
}

void merge(std::vector<int> &vec, int l, int r, const std::vector<int> &cached) {
    if (l >= r)
        return;
    int mid = (r + l) / 2;
    int i = l, j = mid + 1, resultIndex = l;
    while (i <= mid && j <= r) {
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

void multi_thread_quick_sort(std::vector<int> &vec, const int threadLogarithm) {
    // 多线程快排
    int threadAmount = std::pow(2, threadLogarithm);
    std::vector<std::thread> threadPool;
    threadPool.reserve(threadAmount);
    // TODO: 解决容量不能整除的问题
    std::size_t vecSize = vec.size() / threadAmount;
    for (int i = 0; i < threadAmount; i++)
        threadPool.emplace_back(quick_sort<std::vector<int>>, std::ref(vec), i * vecSize, (i + 1) * vecSize - 1);
    for (auto &t: threadPool)
        t.join();
    // 开始归并操作
    // 3 8
    for (int i = threadLogarithm - 1; i >= 0; i--) {
        std::vector<int> result(vec.size());
        threadPool.clear();
        // 处理归并所需要的线程数量
        int handleMergeThreadAmount = std::pow(2, i);
        // 每条线程应该处理的数据数量
        int vecSizePerThread = vec.size() / handleMergeThreadAmount;
        for (int j = 0; j < handleMergeThreadAmount; j++)
            threadPool.emplace_back(merge, std::ref(result), j * vecSizePerThread, (j + 1) * vecSizePerThread - 1,
                                    std::ref(vec));
        for (auto &t: threadPool)
            t.join();
        vec = std::move(result);
    }
}

void test_multi_sort() {
    std::cout << std::thread::hardware_concurrency() << std::endl;
    {
        std::cout << "单线程排序中... 数据量为16000000" << std::endl;
        auto vec1 = generate_random_container_single_thread<std::vector>(1600000, 0, 1000000);
        InTime i1;
        single_thread_quick_sort(vec1);
    }
    {
        std::cout << "多线程排序中... 数据量为16000000" << std::endl;
        auto vec2 = generate_random_container_single_thread<std::vector>(1600000, 0, 1000000);
        InTime i2;
        multi_thread_quick_sort(vec2, 3);
    }
}

int find_max_num_single_thread(const std::vector<int> &vec) {
    int size = vec.size();
    int result = vec[0];
    for (int i = 0; i < size; i++) {
        result = std::max(vec[i], result);
    }
    return result;
}

int find_max_num_multi_thread(const std::vector<int> &vec) {
    auto threadAmount = std::thread::hardware_concurrency();
    auto handleLength = vec.size() / threadAmount;
    std::vector<std::thread> threadVec;
    std::vector<int> maxVec(threadAmount);
    for (auto i = 0; i < threadAmount; i++)
        threadVec.emplace_back([&maxVec, &vec, handleLength](auto index) {
            maxVec[index] = *std::max_element(vec.begin() + index * handleLength, vec.begin() + (index + 1) * handleLength - 1);
        }, i);
    for (auto& t : threadVec)
        t.join();
    return *std::max_element(maxVec.begin(), maxVec.end());
}

void test_multi_max()
{
    auto vec = generate_random_container_multi_thread<std::vector>(10000000, 0, 100000000);
    InTime it;
    std::cout << find_max_num_single_thread(vec) << std::endl;
    it.Stop();
    it.ReStart();
    std::cout << find_max_num_multi_thread(vec) << std::endl;
    it.Stop();
}

int main() {
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
    system("pause");
}