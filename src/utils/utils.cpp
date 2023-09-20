#include "utils/utils.h"
#include <iostream>
#include <cstring>

void errif(bool condition, const std::string& errmsg) {
    if (condition) {
        std::cerr << errmsg << ": " << std::strerror(errno) << std::endl;
        std::exit(EXIT_FAILURE);
    }
}
