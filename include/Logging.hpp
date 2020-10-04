#ifndef Logging_hpp
#define Logging_hpp

#include <iostream>
#include <chrono>
#include <iomanip>
#include <ctime>   

namespace logging {
    std::string getCurrentTimestamp();
}

#define log(...)\
{\
    std::cout << logging::getCurrentTimestamp() << "| " << __VA_ARGS__ << std::endl;\
}

#ifdef DEBUG
    #define debug(...) log(__VA_ARGS__)
#else
    #define debug(...)
#endif

#endif
