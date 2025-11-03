#ifndef PLURALITYELECTION_H
#define PLURALITYELECTION_H

#include <map>
#include <format>
#include "IElection.h"
#include "Candidate.h"
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <algorithm>

class PluralityElection : public IElection {
    public:

        PluralityElection(int nSeats, std::vector<Ballot *>b, std::vector<Candidate *> c);

        ~PluralityElection();

        /**
         * @brief Runs the plurality election algorithm, sorts the candidates by their number of votes and then selects
         * the numSeats most voted ones, breaking ties using coinflips along the way.
         */
        void run() override;

    private:

        std::map<std::string, int> candidateCounts;

        /**
         * @brief Setter for candidateCounts.
         * @param candidateName The name of the candidate we want to change.
         * @param ballotCount The number of votes that the individual candidate should be updated to.
         */
        void setCandidateCounts(std::string candidateName, int ballotCount);

        /**
         * @brief Breaks a tie by randomly selecting a candidate from a list of tied candidates.
         * @param tiedCandidates The list of candidates we need to perform a tiebreaker for.
         * @return A randomly selected candidate of type Candidate.
         */
        Candidate *breakTie(const std::vector<Candidate *>& tiedCandidates) override;

};

#endif