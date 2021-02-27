// learningKernel.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <cstdlib>
#include "learningKernel.h"
#include "exploits.h"


int getArgs() {
    int choice = {};
    std::cout << "What shall we pwn today?" << std::endl;
    std::cout << "1. HEVD stack overflow" << std::endl;
    std::cout << "> ";
    std::cin >> choice;
    return choice;
};

int invokePwn(int choice) {
    int ret = 0;
    switch (choice) {
    case 1:  ret = hevd_stackoverflow();
             break;
    default: ret = 1;
             std::cout << "Invalid choice provided, Exiting..." << std::endl;
    }
    return ret;
}


int main(int argc, char* argv[])
{
    int ret=0;
    std::cout << banner << std::endl;
    //if we were provided args, lets just jump straight into pwn, Else lets get some from the user
    if ((argc == 2)) {
        ret = invokePwn(atoi(argv[1]));
    }
    else
    {
        ret = invokePwn(getArgs());
    }

    // stop from the console closing immediatly
    system("pause");
    return ret;
}

