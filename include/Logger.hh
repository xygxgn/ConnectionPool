#ifndef __LOGGER_HH
#define __LOGGER_HH

#include <iostream>

#define LOG(str) \
    std::cout << __FILE__ << ":" << __LINE__ << " " << \
    __TIMESTAMP__ << " : " << str << std::endl

#endif