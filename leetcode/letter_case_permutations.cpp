/**
  * Copyright(C), 2018
  * Name: letter_case_permutations
  * Author: Wilson Lan
  * Description:
  *     Given a string S, we can transform every letter individually to be
  *     lowercase or uppercase to create another string.  Return a list of
  *     all possible strings we could create.
  */

class Solution {
    public:
        vector<string> letterCasePermutation(string s) {
            vector<string> permutations;
            letterCasePermutationImpl(s, 0, permutations);
            return permutations;
        }

        void letterCasePermutationImpl(string &s, int index,
                                       vector<string> &permutations) {
            int lens = s.length();
            if (index >= lens) {
                permutations.push_back(s);
                return;
            }
            letterCasePermutationImpl(s, index+1, permutations);
            if (isalpha(s[index])) {
                char tmp = islower(s[index]) ? 
                    toupper(s[index]) : tolower(s[index]);
                swap(tmp, s[index]);
                letterCasePermutationImpl(s, index+1, permutations);
                swap(tmp, s[index]);
            }
        }
};
