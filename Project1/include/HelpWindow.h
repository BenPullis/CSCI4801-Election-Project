#ifndef HELPWINDOW_H
#define HELPWINDOW_H

#include <iostream>
#include <string>

//Temp HelpWindow to test main.

class HelpWindow {
public:
    void getHelp() {
        std::cout << "\n--- Help Menu ---\n";
        std::cout << "Enter a positive integer for the number of seats.\n";
        std::cout << "Type 'help' to see this message again.\n";
        std::cout << "-----------------\n\n";
    }
};

#endif