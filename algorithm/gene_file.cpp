/**
  * Copyright(C), 2018, Nsfocus
  * Name:
  * Author: Wilson Lan
  * Description:
  */

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

static const int MAX_LINES = 1024;
static const int MAX_LENS  = 256;
static const int MAX_ELEM  = 4096;


int main() {
    srand(static_cast<int>(time(0)));
    int lines = rand() % MAX_LINES;
    
    std::fstream fs("cases.txt", std::fstream::out|std::fstream::app);
    if (!fs.is_open()) {
        std::cerr << "Open file failed!" << std::endl;
        return 1;
    }

    for (int i = 0; i < lines; ++i) {
        int lens = rand() % MAX_LENS;
        for (int j = 0; j < lens; ++j) {
            int num  = rand() % MAX_ELEM;
            fs << num << " ";
        }
        fs << std::endl;
    }

    fs.close();

    return 0;
}
