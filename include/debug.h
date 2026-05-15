#pragma once

#include <iostream>
#include <string>

inline bool DEBUG_MODE = false;
inline bool CIRCUIT_PRINT = true;
inline bool BEST_NN = false;

inline void debugLog(const std::string& message)
{
    if (DEBUG_MODE) {
        std::cout << "[DEBUG] " << message << std::endl;
    }
}