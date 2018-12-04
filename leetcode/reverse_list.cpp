/**
  * Copyright(C), 2018
  * Name: reverse_list
  * Author: Wilson Lan
  * Description:
  *     Reverse a singly linked list.
  */

class Solution {
    public:
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
