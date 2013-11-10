#ifndef PRINTMACROS_HPP
#define PRINTMACROS_HPP


#include <iostream>


#define PRINT_FORMATTED_INFO(printString) std::cout << __PRETTY_FUNCTION__ << "[INFO]: " << printString << std::endl;
#define PRINT_FORMATTED_ERROR(printString) std::cout << __PRETTY_FUNCTION__ << "[ERROR]: " << printString << std::endl;
#define PRINT_FORMATTED_LIST_ELEMENT(printString) std::cout << " * " << printString << std::endl;


#endif