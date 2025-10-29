#include <map>
#include "IElection.h"

class PluralityElection : public IElection {
    public:
        void run();

    private:
        std::map<std::string, int> candidateCounts;
        void setCandidateCounts(std::string candidatename, int ballotCount);
};
