#include "HelpWindow.h"

HelpWindow& HelpWindow::getInstance(){
    static HelpWindow instance;
    return instance;
}

std::string HelpWindow::getHelp(){
    std::cout << "help message\n";
    return "hi";
}