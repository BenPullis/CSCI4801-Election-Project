#include "HelpWindow.h"

HelpWindow::HelpWindow() {}

HelpWindow& HelpWindow::getInstance(){
    static HelpWindow instance;
    return instance;
}

void HelpWindow::getHelp(){
    std::string input;
    std::string endMsg = "\n- Type anything to return back to the help menu. -\n";
    while (true){
        std::cout << "--------------\n";
        std::cout << "What would you need help with today?\n1) For STV Help.\n2) for Plurality Help.\n3) for MV Help.\n4) for Audit Help.\n5) for Ballot Handling Help.\n(Exit/exit) to exit.\n\n";
        std::cin >> input;
        if (input == "exit" || input == "Exit") {
            break;
        } else if(input == "1"){
            std::cout << "The Single Transferable Voting (STV) election system allows candidates to be ranked, \n" 
                << "with multiple rounds performed to narrow down the candidates based on a variety of voter preferences."
                << endMsg;
            std::cin >> input;
        } else if(input == "2"){
            std::cout << "The Plurality election system allows votes to select a single candidates, and the candidate(s) "
                << "with the most votes win." << endMsg;
            std::cin >> input;
        } else if(input == "3"){
            std::cout << "The Municipal Voting (MV) election system allows voters to select multiple candidates,"
                << " up to the number of seats, and the candidate(s) with the most votes are declared the winners." 
                << endMsg;
            std::cin >> input;
        } else if(input == "4"){
            std::cout << "The Audit reports are located in the same directory as the program was ran!" << endMsg;
            std::cin >> input;
        } else if(input == "5"){
            std::cout << "Entering your ballots is simple!\nIt's as easy as adding the file to the directory "
                << "of the election and entering the filename into the program!"
                << endMsg;
            std::cin >> input;
        }
    }
}