/**
  * Copyright(C), 2018
  * Name: cycle_list_ii
  * Author: Wilson Lan
  * Description:
  *     Given a linked list, return the node where the cycle begins.
  *     If there is no cycle, return null.
  */

class Solution {
    public:
        ListNode *detectCycle(ListNode *head) {
            ListNode *slow = head, *fast = head;
            while (fast != nullptr && fast->next != nullptr) {
                slow = slow->next;
                fast = fast->next->next;
                if (slow == fast) {
                    slow = head;
                    while (fast != slow) {
                        slow = slow->next;
                        fast = fast->next;
                    }
                    return slow;
                }
            }
            return nullptr;
        }
};
