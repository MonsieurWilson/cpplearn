/**
  * Copyright(C), 2018
  * Name: odd_even_list
  * Author: Wilson Lan
  * Description:
  *     Given a singly linked list, group all odd nodes together followed by
  *     the even nodes. Please note here we are talking about the node number
  *     and not the value in the nodes.
  *     You should try to do it in place. The program should run in O(1) space
  *     complexity and O(nodes) time complexity.
  *
  *     Example 1:
  *     Input: 1->2->3->4->5->NULL
  *     Output: 1->3->5->2->4->NULL
  */

class Solution {
    public:
        ListNode *oddEvenList(ListNode *head) {
            if (head == nullptr) {
                return nullptr;
            }
            ListNode *optr = head, *eptr = optr->next,
                     *ehead = optr->next;
            while (eptr != nullptr && eptr->next != nullptr) {
                optr->next = optr->next->next;
                eptr->next = eptr->next->next;
                optr = optr->next;
                eptr = eptr->next;
            }
            optr->next = ehead;
            return head;
        }
};
