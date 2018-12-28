/**
  * Copyright(C), 2018
  * Name: flatten_multilevel_doubly_list
  * Author: Wilson Lan
  * Description:
  *     You are given a doubly linked list which in addition to the next and
  *     previous pointers, it could have a child pointer, which may or may
  *     not point to a separate doubly linked list. These child lists may have
  *     one or more children of their own, and so on, to produce a multilevel
  *     data structure, as shown in the example below.
  *     Flatten the list so that all the nodes appear in a single-level, doubly
  *     linked list. You are given the head of the first level of the list.
  */

class Node {
    public:
        int val;
        Node *prev, *next, *child;

        Node() {}
        Node(int _val, Node *_prev, Node *_next, Node *_child)
            : val(_val), prev(_prev), next(_next), child(_child) {}
};

class Solution {
    public:
        Node *flatten(Node *head) {
            Node *tail;
            return flattenImpl(head, tail);
        }

    private:
        Node *flattenImpl(Node *head, Node *&tail) {
            Node *cur = head;
            while (cur != nullptr) {
                if (cur->child != nullptr) {
                    Node *chead, *ctail;
                    chead = flattenImpl(cur->child, ctail);
                    cur->child = nullptr;
                    chead->prev = cur;
                    ctail->next = cur->next;
                    if (cur->next != nullptr) {
                        cur->next->prev = ctail;
                    }
                    cur->next = chead;
                    cur = ctail;
                }
                tail = cur;
                cur = cur->next;
            }
            return head;
        }

    public:
        /* A more concise solution. */
        Node *flatten(Node *head) {
            Node *cur = head, *store = nullptr;
            while (cur != nullptr) {
                if (cur->child != nullptr) {
                    store = cur->next;
                    cur->child->prev = cur;
                    cur->next = flatten(cur->child);
                    cur->child = nullptr;
                }
                if (cur->next == nullptr && store != nullptr) {
                    cur->next = store;
                    store->prev = cur;
                    store = nullptr;
                }
                cur = cur->next;
            }
            return head;
        }
};
