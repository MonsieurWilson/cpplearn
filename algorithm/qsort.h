#ifndef QUICK_SORT_H
#define QUICK_SORT_H

#include <vector>
#include <iostream>


namespace lxy {

/* --- Declarations --- */

/*
 * Quick sort based on median of three pivotal selection.
 * TODO
 * 1. Insertion sort acceleration
 * 2. Multiple thread acceleration
 */
template <typename ElementType>
void 
qsort(std::vector<ElementType> &);


/* --- Implementations --- */

template <typename ElementType>
ElementType
median_of_three_pivot(std::vector<ElementType> &nums,
                      typename std::vector<ElementType>::size_type begin,
                      typename std::vector<ElementType>::size_type end) {
    auto mid = (begin + end) / 2;
    typename std::vector<ElementType>::size_type pos = begin;

    if (nums[begin] <= nums[mid] && nums[mid] <= nums[end]) {
        pos = mid;
    } else if (nums[begin] <= nums[end] && nums[end] <= nums[mid]) {
        pos = end;
    }
    std::swap(nums[pos], nums[begin]);
    return nums[begin];
}

template <typename ElementType>
typename std::vector<ElementType>::size_type
partition(std::vector<ElementType> &nums,
          typename std::vector<ElementType>::size_type begin,
          typename std::vector<ElementType>::size_type end) {
    typename std::vector<ElementType>::size_type pos = begin, cur = begin;
    ElementType pivot = median_of_three_pivot(nums, begin, end);
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
void 
qsort(std::vector<ElementType> &nums,
      typename std::vector<ElementType>::size_type begin,
      typename std::vector<ElementType>::size_type end) {
    if (begin < end) {
        auto pos = partition(nums, begin, end);
        qsort(nums, begin, pos);
        qsort(nums, pos+1, end);
    }
}

template <typename ElementType>
void 
qsort(std::vector<ElementType> &nums) {
    qsort(nums, 0, nums.size()-1);
}

} // namespace lxy

#endif
