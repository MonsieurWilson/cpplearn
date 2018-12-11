/**
  * Copyright(C), 2018
  * Name: reverse_list_ii
  * Author: Wilson Lan
  * Description:
  *     Reverse a linked list from position m to n. Do it in one-pass.
  *     Note: 1 ≤ m ≤ n ≤ length of list.
  */

class Solution {
    public:
        ListNode *reverseBetween(ListNode *head, int m, int n) {
            // Reserve a guard node to combine boundary cases.
            ListNode guard(0);
            guard.next = head;
            // Find previous node before mth point.
            ListNode *cur = &guard;
            for (int i = 1; i < m; ++i) {
                cur = cur->next;
            }
            /*                 m         n
             *      rev_guard  +         +
             *          |      +         +
             * guard -> 1  ->  2 -> 3 -> 4 -> 5
             *                 |    |
             *                cur  post
             */
            ListNode *rev_guard = cur, *post = cur->next->next;
            cur = cur->next;
            for (int i = 1; i <= n - m; ++i) {
                cur->next = post->next;
                post->next = rev_guard->next;
                rev_guard->next = post;
                post = cur->next;
            }
            return guard.next;
        }

        ListNode* reverseBetweenII(ListNode* head, int m, int n) {
            // Reserve a guard node to combine boundary cases.
            ListNode guard(0);
            guard.next = head;
            // Find previous node before mth point.
            ListNode *cur = &guard;
            for (int i = 1; i < m; ++i) {
                cur = cur->next;
            }
            /*                 m         n
             *      rev_guard  +         +
             *          |      +         +
             * guard -> 1  ->  2 -> 3 -> 4 -> 5
             *                 |    |
             *                cur  rev_tail
             */
            ListNode *rev_guard = cur, *rev_tail = cur->next->next;
            cur = cur->next;
            for (int i = 1; i <= n - m; ++i) {
                ListNode *store = rev_tail->next;
                rev_tail->next = cur;
                cur = rev_tail;
                rev_tail = store;
            }
            rev_guard->next->next = rev_tail;
            rev_guard->next = cur;
            return guard.next;
        }
};
