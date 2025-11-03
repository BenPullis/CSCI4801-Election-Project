#include "HelpWindow.h"

HelpWindow::HelpWindow() {}

HelpWindow& HelpWindow::getInstance(){
    static HelpWindow instance;
    return instance;
}

void HelpWindow::getHelp(){
    std::string input;
    while (true){
        std::cout << "--------------\n";
        std::cout << "What would you need help with today?\n1) For STV Help.\n2) for Plurality Help.\n3) for Audit Help.\n4) for Ballot Handling Help.\n(Exit/exit) to exit.\n\n";
        std::cin >> input;
        if (input == "exit" || input == "Exit") {
            break;
        } else if(input == "1"){
            std::cout << "The STV (Single Transferable Voting) election system allows candidates to be ranked, \nwith multiple rounds performed to narrow down the candidates based on a variety of voter preferences.\n- Type anything to return back to the help menu. -\n";
            std::cin >> input;
        } else if(input == "2"){
            std::cout << "The Plurality election system is an election system where the candidate(s) with the most votes win.\n- Type anything to return back to the help menu. -\n";
            std::cin >> input;
        } else if(input == "3"){
            std::cout << "The Audit reports are located in the same directory as the program was ran!\n- Type anything to return back to the help menu. -\n";
            std::cin >> input;
        } else if(input == "4"){
            std::cout << "Entering your ballots is simple!\nIt's as easy as adding the file to the directory of the election and entering the filename into the program!\n- Type anything to return back to the help menu. -\n";
            std::cin >> input;
        }
    }
}