#ifndef MVELECTION_H
#define MVELECTION_H


//TODO: remove ones not used
#include <map>
#include <format>
#include "IElection.h"
#include "Candidate.h"
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <algorithm>


class MVElection : public IElection {
    public:
        MVElection(int nSeats, std::vector<Ballot *>b, std::vector<Candidate *> c);

        ~MVElection();

        void run() override;
        

    private:
        std::map<std::string, int> candidateCounts;

        void setCandidateCounts(std::string candidateName, int ballotCount){candidateCounts[candidateName] = ballotCount;}

        int getCandidateCounts(std::string candidateName){return candidateCounts[candidateName];}

        Candidate *breakTie(const std::vector<Candidate *>& tiedCandidates) override;
        
};


#endif