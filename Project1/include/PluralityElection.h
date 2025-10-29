#ifndef PLURALITYELECTION_H
#define PLURALITYELECTION_H

#include <map>
#include "IElection.h"
#include "Candidate.h"
#include <iostream>

class PluralityElection : public IElection {
    public:
<<<<<<< HEAD

        /**
         * @brief
         */
        void run() override;

    private:

=======
        void run() {}
        PluralityElection(int nSeats) : IElection() { numSeats = nSeats; }

    private:
        Candidate *breakTie(const std::vector<Candidate *>& tiedCandidates) override { return new Candidate(); }
>>>>>>> origin/Ben-Dev
        std::map<std::string, int> candidateCounts;

        /**
         * @brief
         * @param
         * @param
         */
        void setCandidateCounts(std::string candidatename, int ballotCount);
<<<<<<< HEAD

        /**
         * @brief
         */
        Candidate breakTie(const std::vector<Candidate>& tiedCandidates) override;
};

#endif
=======
};
>>>>>>> origin/Ben-Dev
