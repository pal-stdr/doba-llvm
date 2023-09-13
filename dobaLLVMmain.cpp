/**
 * @file dobaLLVM.cpp
 * @author Pal (you@domain.com)
 * @brief Doba LLVM Compiler
 * @version 0.0.1
 * @date 2023-09-13
 * 
 * @copyright Copyright (c) 2023
 * 
 */


#include <string>

#include "src/DobaLLVM.h"

int main() {

    /**
     * @brief Program to execute
     */
    std::string program = R"(
        42
    )";



    /**
     * @brief Compiler instance (vm means Virtual Machine)
     */
    DobaLLVM vm;



    /**
     * @brief Generate LLVM IR (a.k.a compile the program) 
     */
    vm.exec(program);



    return 0;
}