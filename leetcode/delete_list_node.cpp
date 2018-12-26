/**
  * Copyright(C), 2018
  * Name: delete_list_node
  * Author: Wilson Lan
  * Description:
  *     Write a function to delete a node (except the tail) in a singly linked
  *     list, given only access to that node.
  *
  *     Note:
  *     - The linked list will have at least two elements.
  *     - All of the nodes' values will be unique.
  *     - The given node will not be the tail and it will always be a valid
  *       node of the linked list.
  *     - Do not return anything from your function.
  */

class Solution {
    public:
        void deleteNode(ListNode *node) {
            swap(node->val, node->next->val);
            node->next = node->next->next;
        }
};
