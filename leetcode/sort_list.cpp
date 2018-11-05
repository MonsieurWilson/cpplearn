/**
  * Copyright(C), 2018, Nsfocus
  * Name: sort_list
  * Author: Wilson Lan
  * Description:
  *     Sort a linked list in O(n log n) time using constant space complexity.
  */

class Solution {
    public:
        ListNode *sortList(ListNode *head) {
            if (head == nullptr || head->next == nullptr) {
                return head;
            }

            // Merge sort:
            // 1. Split whole list into two parts
            // 2. Merge these two parts into a sorted list
            auto pivot = split(head);
            return merge(sortList(pivot.first), sortList(pivot.second));
        }

        std::pair<ListNode *, ListNode *> split(ListNode *head) {
            if (head->next == nullptr) {
                return {head, nullptr};
            }

            ListNode *slow = head, *fast = head->next;
            while (fast != nullptr && fast->next != nullptr) {
                slow = slow->next;
                fast = fast->next->next;
            }

            ListNode *first = head, *second = slow->next;
            slow->next = nullptr;
            return {first, second};
        }

        ListNode *merge(ListNode *first, ListNode *second) {
            // Create a head node to unify insertions in boundary cases
            ListNode head(-1);
            ListNode *cur = &head;
            while (first != nullptr || second != nullptr) {
                if (first != nullptr &&
                    (second == nullptr || first->val <= second->val)) {
                    cur->next = first;
                    first = first->next;
                } else {
                    cur->next = second;
                    second = second->next;
                }
                cur = cur->next;
            }
            return head.next;
        }
};
