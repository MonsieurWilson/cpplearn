/**
  * Copyright(C), 2018
  * Name: remove_nth_node_from_end_of_list
  * Author: Wilson Lan
  * Description:
  *     Given a linked list, remove the n-th node from the end of list and
  *     return its head.
  *
  *     Example:
  *     Given linked list: 1->2->3->4->5, and n = 2.
  *     After removing the second node from the end, the linked list becomes
  *     1->2->3->5.
  *
  *     Note:
  *     Given n will always be valid.
  *
  *     Follow up:
  *     Could you do this in one pass?
  */

class Solution {
    public:
        ListNode *removeNthFromEnd(ListNode *head, int n) {
            ListNode dummy(0);
            dummy.next = head;
            ListNode *slow = &dummy, *fast = &dummy;
            for (int i = 0; i <= n; ++i) {
                fast = fast->next;
            }
            while (fast != nullptr) {
                slow = slow->next;
                fast = fast->next;
            }
            slow->next = slow->next->next;
            return dummy.next;
        }
};
