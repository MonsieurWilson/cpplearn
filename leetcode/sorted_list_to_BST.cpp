/**
  * Copyright(C), 2018
  * Name: sorted_list_to_BST
  * Author: Wilson Lan
  * Description:
  *     Given a singly linked list where elements are sorted in 
  *     ascending order, convert it to a height balanced BST.
  *
  *     For this problem, a height-balanced binary tree is defined
  *     as a binary tree in which the depth of the two subtrees of
  *     every node never differ by more than 1.
  */

class Solution {
    public:
        TreeNode *sortedListToBST(ListNode * head) {
            return sortedListToBSTImpl(head, nullptr);
        }

        TreeNode *sortedListToBSTImpl(ListNode *head, ListNode *tail) {
            if (head == tail) {
                return nullptr;
            } else if (head->next == tail) {
                return new TreeNode(head->val);
            }
            ListNode *mid = head, *fast = head;
            while (fast != tail && fast->next != tail) {
                mid = mid->next;
                fast = fast->next->next;
            }
            TreeNode *root = new TreeNode(mid->val);
            root->left = sortedListToBSTImpl(head, mid);
            root->right = sortedListToBSTImpl(mid->next, tail);
            return root;
        }
};
