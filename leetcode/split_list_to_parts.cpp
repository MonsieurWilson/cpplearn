/**
  * Copyright(C), 2018
  * Name: split_list_to_parts
  * Author: Wilson Lan
  * Description:
  *     Given a (singly) linked list with cur node root, write a function to
  *     split the linked list into k consecutive linked list "parts".
  *     The length of each part should be as equal as possible: no two parts
  *     should have a size differing by more than 1. This may lead to some
  *     parts being null.
  *     The parts should be in order of occurrence in the input list, and parts
  *     occurring earlier should always have a size greater than or equal parts
  *     occurring later.
  *     Return a List of ListNode's representing the linked list parts that are
  *     formed.
  *
  *     Examples 1->2->3->4, k = 5 // 5 equal parts [ [1], [2], [3], [4], null ]
  */

class Solution {
    public:
        vector<ListNode *> splitListToParts(ListNode *root, int k) {
            vector<ListNode *> parts;
            ListNode *ptr = root;
            int lens = 0;
            while (ptr != nullptr) {
                ++lens;
                ptr = ptr->next;
            }
            int quotient = lens / k, remainder = lens % k;
            ptr = root;
            for (int i = 1; i <= k; ++i, --remainder) {
                ListNode node(0);
                ListNode *cur = &node;
                int part_len = quotient + (remainder > 0);
                for (int n = 1; n <= part_len; ++n) {
                    cur->next = ptr;
                    ptr = ptr->next;
                    cur = cur->next;
                }
                cur->next = nullptr;
                parts.push_back(node.next);
            }
            return parts;
        }
};
