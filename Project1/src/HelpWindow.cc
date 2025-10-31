#include "HelpWindow.h"

HelpWindow& HelpWindow::getInstance(){
    static HelpWindow instance;
    return instance;
}

void HelpWindow::getHelp(){
    std::string input;
    while (true){
        std::cout << "What would you need help with today?\n1) For STV Help.\n2)for Plurality Help.\n3) for Audit Help.\n4) for Ballot Handling Help.\nExit to exit.\n";
        std::cin >> input;
        if (input == "exit" || input == "Exit") {
            break;
        } else if(input == "1"){
            std::cout << "The STV (Single Transferable Voting) election system blah blah blah";
        } else if(input == "2"){
            std::cout << "The Plurality election system blah blah blah";
        } else if(input == "3"){
            std::cout << "The Audit reports are located in blah blah blah";
        } else if(input == "4"){
            std::cout << "Entering your ballots is simple! Its as easy as";
        }
    }
}