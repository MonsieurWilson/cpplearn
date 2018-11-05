#ifndef QSELECT_H
#define QSELECT_H

#include <ctime>
#include <cstdlib>
#include <queue>


namespace lxy {

/* --- Declarations --- */

/*
 * A selection algorithm of O(N) average-case complexity.
 * Note that the worst-case complexity is O(N^2).
 */
template <typename ElementType>
ElementType
randomized_select(std::vector<ElementType> &,
                  typename std::vector<ElementType>::size_type);

/*
 * A selection algorithm of O(Nlogk) worst-case complexity.
 * Note that it will cost an extra O(k) space complexity.
 */
template <typename ElementType>
ElementType
heap_select(std::vector<ElementType> &,
            typename std::vector<ElementType>::size_type);

/*
 * A selection algorithm of O(N) worst-case complexity.
 * Note that it is also called BFPRT algorithm.
 */
template <typename ElementType>
ElementType
median_of_median_select(std::vector<ElementType> &,
                        typename std::vector<ElementType>::size_type);


/* --- Implementations --- */

template <typename ElementType>
ElementType
random_pivot(std::vector<ElementType> &nums,
             typename std::vector<ElementType>::size_type begin,
             typename std::vector<ElementType>::size_type end) {
    if (begin < end) {
        srand(static_cast<int>(time(0)));
        typename std::vector<ElementType>::size_type pos = 
            rand() % (end - begin + 1) + begin;
        std::swap(nums[pos], nums[begin]);
    }
    return nums[begin];
}

template <typename ElementType>
typename std::vector<ElementType>::size_type
randomized_partition(std::vector<ElementType> &nums,
                     typename std::vector<ElementType>::size_type begin,
                     typename std::vector<ElementType>::size_type end) {
    auto pos = begin, cur = begin;
    ElementType pivot = random_pivot(nums, begin, end);
    while (cur <= end) {
        if (nums[cur] < pivot) {
            std::swap(nums[++pos], nums[cur]);
        }
        ++cur;
    }
    std::swap(nums[pos], nums[begin]);
    return pos;
}

template <typename ElementType>
ElementType 
randomized_select(std::vector<ElementType> &nums,
                  typename std::vector<ElementType>::size_type begin,
                  typename std::vector<ElementType>::size_type end,
                  typename std::vector<ElementType>::size_type k) {
    typename std::vector<ElementType>::size_type pos = begin, ordinal;
    while (begin <= end) {
        pos = randomized_partition(nums, begin, end);
        ordinal = pos - begin + 1;
        if (k == ordinal) {
            break;
        } else if (k < ordinal) {
            end = pos - 1;
        } else {
            begin = pos + 1;
            k -= ordinal;
        }
    }
    return nums[pos];
}

template <typename ElementType>
ElementType 
randomized_select(std::vector<ElementType> &nums,
                  typename std::vector<ElementType>::size_type k) {
    auto lens = nums.size();
    if (lens == 0 || k == 0) {
        return 0;
    } else if (k > lens) {
        k = lens;
    }
    return randomized_select(nums, 0, lens-1, k);
}

template <typename ElementType>
ElementType
heap_select(std::vector<ElementType> &nums,
            typename std::vector<ElementType>::size_type k) {
    if (nums.size() == 0 || k == 0) {
        return 0;
    }

    std::priority_queue<ElementType> heap;
    typename std::vector<ElementType>::size_type ordinal = 1;
    for (const auto &elem : nums) {
        if (ordinal <= k) {
            heap.push(elem);
        } else if (elem < heap.top()) {
            heap.pop();
            heap.push(elem);
        }
        ++ordinal;
    }
    return heap.top();
}

template <typename ElementType>
void
isort(std::vector<ElementType> &nums,
      typename std::vector<ElementType>::size_type begin,
      typename std::vector<ElementType>::size_type end) {
    /*
     * Insertion sort of O(N^2) average-case complexity.
     * Note that it is more efficient than quick sort and heap sort
     * when N is small (Perhaps N < 15).
     */
    for (auto i = begin+1; i <= end; ++i) {
        auto j = i - 1;
        ElementType val = nums[i];
        for (; j >= begin && nums[j] > val; --j) {
            nums[j+1] = nums[j];
        }
        nums[j+1] = val;
    }
}

template <typename ElementType>
typename std::vector<ElementType>::size_type
median_partition(std::vector<ElementType> &nums,
                  typename std::vector<ElementType>::size_type begin,
                  typename std::vector<ElementType>::size_type end) {
    auto pos=begin, cur = begin, mid = begin + (end - begin) / 2;
    ElementType pivot = nums[mid];
    std::swap(nums[mid], nums[begin]);
    while (cur <= end) {
        if (nums[cur] < pivot) {
            std::swap(nums[++pos], nums[cur]);
        }
        ++cur;
    }
    std::swap(nums[pos], nums[begin]);
    return pos;
}

template <typename ElementType>
ElementType
median_of_median_select(std::vector<ElementType> &nums,
                        typename std::vector<ElementType>::size_type begin,
                        typename std::vector<ElementType>::size_type end,
                        typename std::vector<ElementType>::size_type k) {
    if (end - begin + 1 <= 5) {
        isort(nums, begin, end);
        return nums[begin+k-1];
    }

    auto tail = begin;
    for (auto i = begin; i < end; i += 5, ++tail) {
        auto j = i + 4 < end ? i + 4 : end;
        isort(nums, i, j);
        std::swap(nums[tail], nums[i+(j-i)/2]);
    }

    auto mid = begin + (tail - begin) / 2;
    median_of_median_select(nums, begin, tail, mid);
    typename std::vector<ElementType>::size_type pos = begin, ordinal;
    pos = median_partition(nums, begin, end);
    ordinal = pos - begin + 1;
    if (k == ordinal) {
        return nums[pos];
    } else if (k < ordinal) {
        return median_of_median_select(nums, begin, pos-1, k);
    } else {
        return median_of_median_select(nums, pos+1, end, k-ordinal);
    }
}

template <typename ElementType>
ElementType
median_of_median_select(std::vector<ElementType> &nums,
                        typename std::vector<ElementType>::size_type k) {
    auto lens = nums.size();
    if (lens == 0 || k == 0) {
        return 0;
    } else if (k > lens) {
        k = lens;
    }
    return median_of_median_select(nums, 0, lens-1, k);
}

} // namespace lxy

#endif
