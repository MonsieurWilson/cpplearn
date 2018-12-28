/**
  * Copyright(C), 2018
  * Name: flatten_nested_list_iterator
  * Author: Wilson Lan
  * Description:
  *     Given a nested list of integers, implement an iterator to flatten it.
  *     Each element is either an integer, or a list -- whose elements may also
  *     be integers or other lists.
  *
  *     Example 1:
  *     Input: [[1,1],2,[1,1]]
  *     Output: [1,1,2,1,1]
  *     Explanation: 
  *         By calling next repeatedly until hasNext returns false, 
  *         the order of elements returned by next should be: [1,1,2,1,1].
  */

class NestedInteger {
     public:
     // Return true if this NestedInteger holds a single integer, rather than
     // a nested list
     bool isInteger() const;
     
     // Return the single integer that this NestedInteger holds, if it holds a
     // single integer
     // The result is undefined if this NestedInteger holds a nested list
     int getInteger() const;
     
     // Return the nested list that this NestedInteger holds, if it holds a
     // nested list
     // The result is undefined if this NestedInteger holds a single integer
     const vector<NestedInteger> &getList() const;
};

class NestedIterator {
    public:
        NestedIterator(vector<NestedInteger> &nestedList) {
            initFlatList(nestedList);
            itr = flatList.begin();
        }

        int next() {
            return *itr++;
        }

        bool hasNext() {
            return itr != flatList.end();
        }

    private:
        void initFlatList(const vector<NestedInteger> &nestedList) {
            for (const auto &nestedInt : nestedList) {
                if (nestedInt.isInteger()) {
                    flatList.push_back(nestedInt.getInteger());
                } else {
                    initFlatList(nestedInt.getList());
                }
            }
        }
    
    private:
        vector<int> flatList;
        vector<int>::const_iterator itr;
};
