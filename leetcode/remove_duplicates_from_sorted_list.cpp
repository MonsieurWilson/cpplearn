/**
  * Copyright(C), 2018
  * Name: remote_duplicates_from_sorted_list
  * Author: Wilson Lan
  * Description:
  *     Given a sorted linked list, delete all duplicates such that each
  *     element appear only once.
  */

class Solution {
    public:
        ListNode *deleteDuplicates(ListNode *head) {
            ListNode *cur = head;
            while (cur != nullptr && cur->next != nullptr) {
                if (cur->val == cur->next->val) {
                    cur->next = cur->next->next;
                } else {
                    cur = cur->next;
                }
            }
            return head;
        }
};
