#ifndef STVELECTION_H
#define STVELECTION_H

#include "IElection.h"
#include <iostream>

//Temp STVElection to test main.

class STVElection : public IElection {
private:
    int numSeats;

public:
    STVElection(int n) : numSeats(n) {}

    void run() override {
        std::cout << "Running STV Election with " << numSeats << " seats." << std::endl;
    }

    Candidate breakTie(const std::vector<Candidate>& tiedCandidates) override {
        std::cout << "Breaking tie among " << tiedCandidates.size() << " candidates (STV placeholder)." << std::endl;
        return Candidate();
    }
};

#endif
