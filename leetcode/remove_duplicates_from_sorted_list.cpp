/**
  * Copyright(C), 2018
  * Name: remove_duplicates_from_sorted_list
  * Author: Wilson Lan
  * Description:
  *     Given a sorted linked list, delete all duplicates such that each
  *     element appear only once.
  */

class Solution {
    public:
        ListNode *deleteDuplicates(ListNode *head) {
            ListNode dummy(0);
            dummy.next = head;
            ListNode *ptr = &dummy;
            while (ptr != nullptr && ptr->next != nullptr) {
                ListNode *cur = ptr->next;
                bool isDup = false;
                while(cur->next != nullptr && 
                      cur->val == cur->next->val) {
                    isDup = true;
                    cur = cur->next;
                }
                if (isDup) {
                    ptr->next = cur->next;
                } else {
                    ptr = ptr->next;
                }
            }
            return dummy.next;
        }
};
