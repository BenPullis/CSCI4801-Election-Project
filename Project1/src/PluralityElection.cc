#include "./include/PluralityElection.h"

void PluralityElection::setCandidateCounts(std::string candidateName, int ballotCount){
    candidateCounts[candidateName] = ballotCount;
}