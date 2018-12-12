/**
  * Copyright(C), 2018
  * Name: remove_list_elements
  * Author: Wilson Lan
  * Description:
  *     Remove all elements from a linked list of integers that have value val.
  */

class Solution {
    public:
        ListNode *removeElements(ListNode *head, int val) {
            ListNode dummy(0);
            dummy.next = head;
            ListNode *ptr = &dummy;
            while (ptr != nullptr && ptr->next != nullptr) {
                if (ptr->next->val == val) {
                    ptr->next = ptr->next->next;
                } else {
                    ptr = ptr->next;
                }
            }
            return dummy.next;
        }
};
