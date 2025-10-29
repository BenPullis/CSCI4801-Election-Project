#ifndef PLURALITYELECTION_H
#define PLURALITYELECTION_H

#include <map>
#include "IElection.h"
#include "Candidate.h"
#include <iostream>

class PluralityElection : public IElection {
    public:

        /**
         * @brief
         */
        void run() override;

    private:

        std::map<std::string, int> candidateCounts;

        /**
         * @brief
         * @param
         * @param
         */
        void setCandidateCounts(std::string candidatename, int ballotCount);

        /**
         * @brief
         */
        Candidate breakTie(const std::vector<Candidate>& tiedCandidates) override;
};

#endif