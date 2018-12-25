/**
  * Copyright(C), 2018
  * Name: palindrome_list
  * Author: Wilson Lan
  * Description:
  *     Given a singly linked list, determine if it is a palindrome.
  */

class Solution {
    public:
        bool isPalindrome(ListNode *head) {
            if (head == nullptr || head->next == nullptr) {
                return true;
            }
            ListNode *cur = nullptr, *post = head, *fast = head;
            while (fast != nullptr && fast->next != nullptr) {
                fast = fast->next->next;
                ListNode *store = post->next;
                post->next = cur;
                cur = post;
                post = store;
            }
            post = fast == nullptr ? post : post->next;
            return isSameList(cur, post);
        }

        bool isSameList(ListNode *l1, ListNode *l2) {
            while (l1 != nullptr && l2 != nullptr && l1->val == l2->val) {
                l1 = l1->next;
                l2 = l2->next;
            }
            return l1 == nullptr && l2 == nullptr;
        }
};
