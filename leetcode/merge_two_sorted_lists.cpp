/**
  * Copyright(C), 2018
  * Name:
  * Author: Wilson Lan
  * Description:
  *     Merge two sorted linked lists and return it as a new list. The new
  *     list should be made by splicing together the nodes of the first two
  *     lists.
  *
  *     Example:
  *     Input: 1->2->4, 1->3->4
  *     Output: 1->1->2->3->4->4
  */

class Solution {
    public:
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
};
