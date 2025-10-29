#ifndef STVELECTION_H_
#define STVELECTION_H_

#include <algorithm>
#include <unordered_map>
#include <string>
#include <vector>
#include "IElection.h"
#include "Candidate.h"

class STVElection : public IElection {
    public:
        STVElection(int nSeats, bool shuffleBallots);
        ~STVElection();
        void run() override;
        void addLoser(Candidate *candidate) { losersList.push_back(candidate); }

    private:
        std::unordered_map<std::string, int> candidatePiles;
        int droopQuota;
        bool shuffleBallots;
        std::vector<Candidate *> losersList;

        int calculateDroopQuota();
        void updateCandidatePiles(int &numBallotsProcessed);
        Candidate *breakTie(const std::vector<Candidate *>& tiedCandidates) override;
};

#endif /* STVELECTION_H_ */