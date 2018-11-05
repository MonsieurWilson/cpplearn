/**
  * Copyright(C), 2017, Nsfocus
  * Name:
  * Author: Wilson Lan
  * Description:
  */

#include <setjmp.h>
#include <iostream>


static jmp_buf jmpbuffer;
static int     globval;

static void f1(int, int, int, int);
static void f2();

static const int JUMP_CODE = 1;

int main() {
    int          autoval;
    register int regival;
    volatile int volaval;
    static   int statval;

    globval = 1;
    autoval = 2;
    regival = 3;
    volaval = 4;
    statval = 5;

    switch (setjmp(jmpbuffer)) {
        case 0:
            break;
        case JUMP_CODE:
            std::cout << "After longjmp:" << std::endl;
            std::cout << "globval = " << globval << ", "
                      << "autoval = " << autoval << ", "
                      << "regival = " << regival << ", "
                      << "volaval = " << volaval << ", "
                      << "statval = " << statval << std::endl;
            return 0;
        default:
            break;
    }

    globval = 91;
    autoval = 92;
    regival = 93;
    volaval = 94;
    statval = 95;

    f1(autoval, regival, volaval, statval);

    return 0;
}

static void f1(int autoval, int regival, int volaval, int statval) {
    std::cout << "Function f1 zone:" << std::endl;

    std::cout << "globval = " << globval << ", "
              << "autoval = " << autoval << ", "
              << "regival = " << regival << ", "
              << "volaval = " << volaval << ", "
              << "statval = " << statval << std::endl;

    f2();
}

static void f2() {
    longjmp(jmpbuffer, JUMP_CODE);
}
