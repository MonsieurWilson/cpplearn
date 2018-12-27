/**
  * Copyright(C), 2018
  * Name: swap_list_nodes_in_pair
  * Author: Wilson Lan
  * Description:
  *     Given a linked list, swap every two adjacent nodes and return its head.
  *     Example:
  *     Given 1->2->3->4, you should return the list as 2->1->4->3.
  *
  *     Note:
  *     - Your algorithm should use only constant extra space.
  *     - You may not modify the values in the list's nodes, only nodes itself
  *       may be changed.
  */

class Solution {
    public:
        ListNode *swapPairs(ListNode *head) {
            ListNode dummy(0);
            dummy.next = head;
            ListNode *first = &dummy, *second = first->next;
            while (second != nullptr && second->next != nullptr) {
                first->next = second->next;
                second->next = first->next->next;
                first->next->next = second;
                first = second;
                second = second->next;
            }
            return dummy.next;
        }
};
