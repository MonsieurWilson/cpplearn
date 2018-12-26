/**
  * Copyright(C), 2018
  * Name: merge_k_sorted_lists
  * Author: Wilson Lan
  * Description:
  *     Merge k sorted linked lists and return it as one sorted list.
  *     Analyze and describe its complexity.
  */

class Solution {
    public:
        /* Devide and Conquer of O(Nlog(k)) average-case time complexity. */
        ListNode *mergeKLists(vector<ListNode *> &lists) {
            int interval = 1;
            while (interval < lists.size()) {
                for (int i = 0; i < lists.size()-interval; i += interval*2) {
                    lists[i] = mergeTwoLists(lists[i], lists[i+interval]);
                }
                interval *= 2;
            }
            return lists.empty() ? nullptr : lists.front();
        }

        ListNode *mergeTwoLists(ListNode *l1, ListNode *l2) {
            // Create a head node to unify insertions in boundary cases
            ListNode head(-1);
            ListNode *cur = &head;
            while (l1 != nullptr || l2 != nullptr) {
                if (l1 != nullptr &&
                    (l2 == nullptr || l1->val <= l2->val)) {
                    cur->next = l1;
                    l1 = l1->next;
                } else {
                    cur->next = l2;
                    l2 = l2->next;
                }
                cur = cur->next;
            }
            return head.next;
        }

        /* Compare one-by-one with priority queue of O(Nlog(k)) average-case
         * time complexity and O(k) space complexity. */
        ListNode *mergeKListsWithPriorityQueue(vector<ListNode *> &lists) {
            priority_queue<ListNode *, vector<ListNode *>, Comparator> heap;
            for (auto &head : lists) {
                if (head != nullptr) {
                    heap.push(head);
                }
            }
            ListNode dummy(0);
            ListNode *cur = &dummy;
            while (!heap.empty()) {
                cur->next = heap.top();
                heap.pop();
                cur = cur->next;
                if (cur->next != nullptr) {
                    heap.push(cur->next);
                }
            }
            return dummy.next;
        }

        struct Comparator {
            bool operator()(ListNode *left, ListNode *right) {
                return left->val > right->val;
            }
        };
};
