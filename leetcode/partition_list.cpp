/**
  * Copyright(C), 2018
  * Name: partition_list
  * Author: Wilson Lan
  * Description:
  *     Given a linked list and a value x, partition it such that all nodes
  *     less than x come before nodes greater than or equal to x.
  *     You should preserve the original relative order of the nodes in each
  *     of the two partitions.
  *
  *     Example:
  *     Input: head = 1->4->3->2->5->2, x = 3
  *     Output: 1->2->2->4->3->5
  */

class Solution {
    public:
        ListNode *partition(ListNode *head, int x) {
            ListNode first_part(0), second_part(0);
            ListNode *f = &first_part, *s = &second_part,
                     *ptr = head;
            while (ptr != nullptr) {
                if (ptr->val >= x) {
                    s->next = ptr;
                    s = s->next;
                } else {
                    f->next = ptr;
                    f = f->next;
                }
                ptr = ptr->next;
            }
            s->next = nullptr;
            f->next = &second_part;
            return first_part.next;
        }
};
