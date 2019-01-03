/**
  * Copyright(C), 2018
  * Name: reverse_only_letters
  * Author: Wilson Lan
  * Description:
  *     Given a string S, return the "reversed" string where all characters
  *     that are not a letter stay in the same place, and all letters reverse
  *     their positions.
  */

class Solution {
    public:
        string reverseOnlyLetters(string S) {
            int head = 0, tail = S.size() - 1;
            while (head < tail) {
                while (head < tail && 0 == isalpha(S[head])) ++head;
                while (head < tail && 0 == isalpha(S[tail])) --tail;
                if (head < tail) {
                    swap(S[head++], S[tail--]);
                }
            }
            return S;
        }
};
