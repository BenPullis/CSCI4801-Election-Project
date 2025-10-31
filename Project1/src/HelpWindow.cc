#include "HelpWindow.h"

HelpWindow& HelpWindow::getInstance(){
    static HelpWindow instance;
    return instance;
}

void HelpWindow::getHelp(){
    std::cout << "help message\n";
}