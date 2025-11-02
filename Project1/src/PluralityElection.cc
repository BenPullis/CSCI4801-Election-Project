#include "PluralityElection.h"

PluralityElection::PluralityElection(int nSeats, std::vector<Ballot *>b, std::vector<Candidate *> c) : IElection() {
    numSeats = nSeats;
    ballots = b;
    candidates = c;
    reportGenerator = new AuditReport();
}

void PluralityElection::setCandidateCounts(std::string candidateName, int ballotCount){
    candidateCounts[candidateName] = ballotCount;
}

PluralityElection::~PluralityElection(){
    delete reportGenerator;
}

Candidate *findCandidateViaName(std::string candidateName, const std::vector<Candidate *> candidates) {
    for (Candidate *candidate : candidates) {
        if (candidate->getName() == candidateName) {
            return candidate;
        }
    }
    return nullptr;
}

void PluralityElection::run(){

    // handling ballot stuff
    for (Ballot *ballot : ballots) {
        std::map<int, std::string> candidateRankings = ballot->getCandidateRankings();
        std::string desiredCandidate = candidateRankings.begin()->second;

        if (candidateCounts.find(desiredCandidate) != candidateCounts.end()) {
            setCandidateCounts(desiredCandidate, candidateCounts[desiredCandidate] + 1);
        } else {
            setCandidateCounts(desiredCandidate, 1);
        }
    }

    // sort so we know who has the most votes
    std::vector<std::pair<std::string, int>> sortedCandidateCounts(candidateCounts.begin(), candidateCounts.end());

    std::sort(sortedCandidateCounts.begin(), sortedCandidateCounts.end(), [](const auto& a, const auto& b) {
        return a.second > b.second;
    });

    int seatsFilled = 0;
    int i = 0;

    // based on # of votes start making people winners
    while (seatsFilled < numSeats && i < sortedCandidateCounts.size()) {
        int currentVotes = sortedCandidateCounts[i].second;
        std::vector<Candidate *> tiedCandidates;

        while (i < sortedCandidateCounts.size() && sortedCandidateCounts[i].second == currentVotes) {
            Candidate *cand = findCandidateViaName(sortedCandidateCounts[i].first, candidates);
            if (cand) {
                tiedCandidates.push_back(cand);
            }
            i++;
        }

        int seatsRemaining = numSeats - seatsFilled;
        if (tiedCandidates.size() <= seatsRemaining) {
            for (Candidate *cand : tiedCandidates) {
                addWinner(cand);
                seatsFilled++;
            }
        } else {
            while (seatsFilled < numSeats) {
                Candidate *winner = breakTie(tiedCandidates);
                addWinner(winner);
                seatsFilled++;

                tiedCandidates.erase(std::remove(tiedCandidates.begin(), tiedCandidates.end(), winner), tiedCandidates.end());
            }
        }
    }
}

Candidate *PluralityElection::breakTie(const std::vector<Candidate *>& tiedCandidates){
    srand(time(0));
    int selection = rand() % (tiedCandidates.size());
    return tiedCandidates[selection];
}