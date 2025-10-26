#ifndef STVELECTION_H_
#define STVELECTION_H_

#include <unordered_map>
#include <string>
#include <vector>
#include "IElection.h"
#include "Candidate.h"

class STVElection : public IElection {
    public:
        STVElection(int nSeats, std::vector<Ballot> ballots, std::vector<Candidate> candidates);
        void run() override;

    private:
        std::unordered_map<std::string, int> candidatePiles;
        int droopQuota;
        std::vector<Candidate> losersList;

        std::vector<Candidate> getLosersList() { return losersList; }
        void addLoser(Candidate candidate) { losersList.push_back(candidate); }
        int calculateDroopQuota();
        void updateCandidatePiles(std::string preference) { candidatePiles.get() };
}

#endif /* STVELECTION_H_ */