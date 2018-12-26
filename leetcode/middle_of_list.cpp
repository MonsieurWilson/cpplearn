/**
  * Copyright(C), 2018
  * Name: middle_of_list
  * Author: Wilson Lan
  * Description:
  *     Given a non-empty, singly linked list with head node head, return
  *     a middle node of linked list.
  *     If there are two middle nodes, return the second middle node.
  */

class Solution {
    public:
        ListNode* middleNode(ListNode* head) {
            ListNode *mid = head, *fast = head;
            while (fast != nullptr && fast->next != nullptr) {
                mid = mid->next;
                fast = fast->next->next;
            }
            return mid;
        }
};
