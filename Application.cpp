#include "Application.h"

#include <iostream>
#include <limits>

void Application::run()
{
    std::cout << "Program is running.\n";
    std::cout << "Trial mode: 4 launches.\n";
    std::cout << "Press Enter to finish...\n";

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}
