#include <map>
#include "IElection.h"

class PluralityElection : public IElection {
    public:
        void run() {}
        PluralityElection(int nSeats) : IElection() { numSeats = nSeats; }

    private:
        Candidate *breakTie(const std::vector<Candidate *>& tiedCandidates) override { return new Candidate(); }
        std::map<std::string, int> candidateCounts;
        void setCandidateCounts(std::string candidatename, int ballotCount);
};
