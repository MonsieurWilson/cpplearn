/**
  * Copyright(C), 2018
  * Name: reorder_list
  * Author: Wilson Lan
  * Description:
  *     Given a singly linked list L: L0->L1->L2->...->Ln-1->Ln,
  *     reorder it to: L0->Ln->L1->Ln-1->L2...->Ln-2->...
  *
  *     You may not modify the values in the list's nodes, only nodes
  *     itself may be changed.
  */

class Solution {
    public:
        void reorderList(ListNode *head) {
            if (head == nullptr || head->next == nullptr) {
                return;
            }
            ListNode *slow = head, *fast = head->next;
            while (fast != nullptr && fast->next != nullptr) {
                slow = slow->next;
                fast = fast->next->next;
            }
            ListNode *l1 = head, *l2 = reverseList(slow->next);
            slow->next = nullptr;
            ListNode *cur = l1;
            while (l1 != nullptr && l2 != nullptr) {
                l1 = l1->next;
                cur->next = l2;
                l2 = l2->next;
                cur->next->next = l1;
                cur = l1;
            }
        }

        ListNode *reverseList(ListNode *head) {
            ListNode *cur = nullptr, *post = head;
            while (post != nullptr) {
                ListNode *store = post->next;
                post->next = cur;
                cur = post;
                post = store;
            }
            return cur;
        }
};
