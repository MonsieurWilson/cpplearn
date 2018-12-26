/**
  * Copyright(C), 2018
  * Name: random_node_of_list
  * Author: Wilson Lan
  * Description:
  *     Given a singly linked list, return a random node's value from the
  *     linked list. Each node must have the same probability of being chosen.
  *
  *     Follow up:
  *     What if the linked list is extremely large and its length is unknown
  *     to you? Could you solve this efficiently without using extra space?
  */

class Solution {
    public:
        /** @param head The linked list's head.
            Note that the head is guaranteed to be not null, so it contains at
            least one node. */
        Solution(ListNode *head) {
            m_list = head;
        }

        /** Returns a random node's value. */
        int getRandom() {
            /* Reservoir sampling */
            ListNode *cur = m_list->next;
            int res = m_list->val;
            int lens = 1;
            while (cur != nullptr) {
                res = rand() % ++lens == 0 ? cur->val : res;
                cur = cur->next;
            }
            return res;
        }

    private:
        ListNode *m_list;
};
