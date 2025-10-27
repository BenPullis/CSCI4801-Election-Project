#ifndef PLURALITYELECTION_H
#define PLURALITYELECTION_H

#include "IElection.h"
#include <iostream>

//Temp PluralityElection to test main.

class PluralityElection : public IElection {
private:
    int numSeats;

public:
    PluralityElection(int n) : numSeats(n) {}

    void run() override {
        std::cout << "Running Plurality Election with " << numSeats << " seats." << std::endl;
    }

    Candidate breakTie(const std::vector<Candidate>& tiedCandidates) override {
        std::cout << "Breaking tie among " << tiedCandidates.size() << " candidates (Plurality placeholder)." << std::endl;
        return Candidate();
    }
};

#endif
