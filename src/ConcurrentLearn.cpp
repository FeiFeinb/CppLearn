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
    // 解决容量不能整除的问题
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

int main() {
    {
        std::cout << "单线程排序中... 数据量为16000000" << std::endl;
        InTime i1;
        auto vec1 = generate_random_container<std::vector>(16000000, 0, 1000000);
        single_thread_quick_sort(vec1);
    }
    {
        std::cout << "多线程排序中... 数据量为16000000" << std::endl;
        InTime i2;
        auto vec2 = generate_random_container<std::vector>(16000000, 0, 1000000);
        multi_thread_quick_sort(vec2, 0);
    }
    system("pause");
}