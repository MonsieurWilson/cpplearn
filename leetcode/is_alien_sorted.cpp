/**
  * Copyright(C), 2018
  * Name: is_alien_sorted
  * Author: Wilson Lan
  * Description:
  *     In an alien language, surprisingly they also use english lowercase
  *     letters, but possibly in a different order. The order of the alphabet
  *     is some permutation of lowercase letters.
  *     Given a sequence of words written in the alien language, and the order
  *     of the alphabet, return true if and only if the given words are sorted
  *     lexicographicaly in this alien language.
  */

class Solution {
    public:
        bool isAlienSorted(vector<string> &words, string order) {
            unordered_map<char, int> dict;
            for (int i = 0; i < order.size(); ++i) {
                dict[order[i]] = i;
            }
            auto comp = [&dict] (const string &a, const string &b) {
                int lens = min(a.size(), b.size());
                for (int i = 0; i < lens; ++i) {
                    if (dict[a[i]] == dict[b[i]]) {
                    } else {
                        return dict[a[i]] < dict[b[i]];
                    }
                }
                return a.size() < b.size();
            };
            return is_sorted(words.begin(), words.end(), comp);
        }
};
