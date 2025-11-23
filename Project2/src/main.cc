#include "STVElection.h"
#include "PluralityElection.h"
#include "IElection.h"
#include "HelpWindow.h"
#include <string>
#include <iostream>
#include "Ballot.h"
#include "Candidate.h"
#include "BallotHandler.h"
#include <vector>

using namespace std;

int main(){
    
    HelpWindow &help = HelpWindow::getInstance();

    cout << "------------------------" << endl;
    cout << "Time To Run An Election!" << endl;
    cout << "------------------------\n\n" << endl;

    string input;
    cout << "Type 'Help' for Further Help, or press any key to continue election: ";
    cin >> input;
    if(input == "Help" || input == "help"){
        help.getHelp();
    }

    BallotHandler *bh = new BallotHandler();
    bh->handleFiles();
    std::string electionType = bh->getElectionType();
    int numSeats = bh->getNumSeats();
    vector<Ballot*> ballots = bh->receiveBallots();

    vector<Candidate*> candidates = bh->getCandidates();

    IElection *election;
    if(electionType == "stv"){
        cout << "Choose whether you would like to shuffle the ballots (Y/N): ";
        string shuffleChoice;
        cin >> shuffleChoice;
        election = new STVElection(numSeats, shuffleChoice == "y" || shuffleChoice == "Y" || shuffleChoice == "yes" || shuffleChoice == "Yes",
            ballots, candidates, bh);
    }
    else if(electionType == "pv"){
        election = new PluralityElection(numSeats, ballots, candidates);
    }

    election->run();

    delete(bh);
    delete(election);

    return 0;
}