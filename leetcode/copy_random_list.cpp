/**
  * Copyright(C), 2018
  * Name: copy_random_list
  * Author: Wilson Lan
  * Description:
  *     A linked list is given such that each node contains an additional
  *     random pointer which could point to any node in the list or null.
  *     Return a deep copy of the list.
  */

struct RandomListNode {
    int label;
    RandomListNode *next, *random;
    RandomListNode(int x) : label(x), next(nullptr), random(nullptr) {}
};

class Solution {
    public:
        RandomListNode *copyRandomList(RandomListNode *head) {
            RandomListNode *ptr = head;
            // Perform a copy for each node and insert the copy node.
            while (ptr != nullptr) {
                RandomListNode *dummy = new RandomListNode(ptr->label);
                dummy->next = ptr->next;
                ptr->next = dummy;
                ptr = dummy->next;
            }
            // Assign random pointers of copy nodes.
            ptr = head;
            while (ptr != nullptr) {
                ptr->next->random = ptr->random != nullptr ? 
                    ptr->random->next : nullptr;
                ptr = ptr->next->next;
            }
            ptr = head;
            RandomListNode deepcopy(0);
            RandomListNode *cptr = &deepcopy;
            // Extract copy nodes and restore original list.
            while (ptr != nullptr) {
                cptr->next = ptr->next;
                ptr->next = ptr->next->next;
                ptr = ptr->next;
                cptr = cptr->next;
            }
            return deepcopy.next;
        }
};
