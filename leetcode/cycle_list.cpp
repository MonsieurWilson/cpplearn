/**
  * Copyright(C), 2018
  * Name: cycle_list
  * Author: Wilson Lan
  * Description:
  *     Given a linked list, determine if it has a cycle in it.
  */

class Solution {
    public:
        bool hasCycle(ListNode *head) {
            ListNode *slow = head, *fast = head;
            while (fast != nullptr && fast->next != nullptr) {
                slow = slow->next;
                fast = fast->next->next;
                if (slow == fast) {
                    return true;
                }
            }
            return false;
        }
};
