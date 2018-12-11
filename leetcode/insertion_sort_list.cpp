/**
  * Copyright(C), 2018
  * Name: insertion_sort_list
  * Author: Wilson Lan
  * Description:
  *     Sort a linked list using insertion sort.
  *
  *     Algorithm of Insertion Sort:
  *     1. Insertion sort iterates, consuming one input element each repetition,
  *        and growing a sorted output list.
  *     2. At each iteration, insertion sort removes one element from the input
  *        data, finds the location it belongs within the sorted list, and 
  *        inserts it there.
  *     3. It repeats until no input elements remain.
  */

class Solution {
    public:
        ListNode *insertionSortList(ListNode *head) {
            ListNode guard(0);
            guard.next = head;
            ListNode *cur = head, *last = &guard;
            while (cur != nullptr) {
                ListNode *ptr = &guard;
                while (ptr != last && ptr->next->val <= cur->val) {
                    ptr = ptr->next;
                }
                if (ptr == last) {
                    last = cur;
                } else {
                    last->next = cur->next;
                    cur->next = ptr->next;
                    ptr->next = cur;
                }
                cur = last->next;
            }
            return guard.next;
        }
};
