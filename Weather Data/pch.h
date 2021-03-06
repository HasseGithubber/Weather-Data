// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

#ifndef PCH_H
#define PCH_H

#include <iostream>
#include <string>
#include <vector>

/*Macro for debugging and a regular print macro*/
#if 1
#define LOG(x) std::cout << "-DEBUG-" << x << "-DEBUG-" << "\n"
#else
#define LOG(x)
#endif // 0

#define PRINT(x) std::cout << x << "\n"

// TODO: add headers that you want to pre-compile here

#endif //PCH_H
