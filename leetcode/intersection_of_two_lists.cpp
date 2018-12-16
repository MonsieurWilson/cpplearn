/**
  * Copyright(C), 2018
  * Name: intersection_of_two_lists
  * Author: Wilson Lan
  * Description:
  *     Write a program to find the node at which the intersection of
  *     two singly linked lists begins.
  *
  *     Notes:
  *     1. If the two linked lists have no intersection at all, return null.
  *     2. The linked lists must retain their original structure after the
  *        function returns.
  *     3. You may assume there are no cycles anywhere in the entire linked
  *        structure.
  *     4. Your code should preferably run in O(n) time and use only O(1)
  *        memory.
  */

class Solution {
    public:
        ListNode *getIntersectionNode(ListNode *l1, ListNode *l2) {
            ListNode *cur1 = l1, *cur2 = l2;
            while (cur1 != cur2) {
                cur1 = cur1 == nullptr ? l2 : cur1->next;
                cur2 = cur2 == nullptr ? l1 : cur2->next;
            }
            return cur1;
        }
};
