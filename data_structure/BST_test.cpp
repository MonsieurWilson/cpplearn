/**
  * Copyright(C), 2018, Nsfocus
  * Name:
  * Author: Wilson Lan
  * Description:
  */

#include <iostream>
#include <iomanip>
#include <vector>

#include "BST.h"

int main() {
    std::vector<int> bst{4,6,7,2,1,3,10,5,9,8};
    lxy::BST<int> tree;

    for (auto itr = bst.begin(); itr != bst.end(); ++itr) {
        tree.insert(*itr);
    }

    std::cout << tree << std::endl;
    
    for (auto itr = bst.begin(); itr != bst.end(); ++itr) {
        std::cout << "Is " << std::setw(2) << *itr << " in BST tree? - " 
                  << (tree.find(*itr) ? "true" : "false") << std::endl;
    }

    std::cout << "Depth of tree: " << tree.depth() << std::endl;
    
    return 0;
}
