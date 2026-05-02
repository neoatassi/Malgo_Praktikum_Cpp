#pragma once

#include <iostream>
#include <fstream>

void assertEqual(int expected, int actual, const std::string& testName);
 
void runTests();