/**
  * Copyright(C), 2018
  * Name: rotate_list
  * Author: Wilson Lan
  * Description:
  *     Given a linked list, rotate the list to the right by k places,
  *     where k is non-negative.
  */

class Solution {
public:
    ListNode *rotateRight(ListNode *head, int k) {
        if (head == nullptr) {
            return head;
        }
        // Formulate a circle
        int lens = 1;
        ListNode *cur = head;
        while (cur->next != nullptr) {
            cur = cur->next;
            ++lens;
        }
        cur->next = head;
        // Move head pointer to new position
        cur = head;
        for (int i = 1; i < lens - k % lens; ++i) {
            cur = cur->next;
        }
        head = cur->next;
        cur->next = nullptr;
        return head;
    }
};
