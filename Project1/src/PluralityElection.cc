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

/**
 * @brief Retrieves a candidate pointer by name from a supplied list.
 * @param candidateName Name of the candidate being searched for.
 * @param candidates Collection of candidate pointers to scan.
 * @returns Matching candidate pointer or nullptr when not found.
 */
Candidate *findCandidateViaName(std::string candidateName, const std::vector<Candidate *> candidates) {
    for (Candidate *candidate : candidates) {
        if (candidate->getName() == candidateName) {
            return candidate;
        }
    }
    return nullptr;
}

/**
 * @brief Generates the static portion of the audit report metadata.
 * @param candidates All candidates participating in the election.
 * @param numSeats Number of seats allocated for this race.
 * @returns Vector of formatted strings describing election metadata.
 */
std::vector<std::string> createAuditReport(const std::vector<Candidate *>& candidates, int numSeats) {
    // build a human-readable local datetime string for the audit report
    std::time_t now = std::time(nullptr);
    char dateBuf[100];
    std::strftime(dateBuf, sizeof(dateBuf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
    std::string dateStr(dateBuf);

    std::string candidatesStr = "";
    for (Candidate *cand : candidates) {
        candidatesStr += std::format("{:s} ({:s}), ", cand->getName(), cand->getParty());
    }
    // remove last comma and space
    candidatesStr.erase(candidatesStr.length() - 2);

    return {
        "Election Type: Plurality",
        "Election Date: " + dateStr,
        "Candidates: " + candidatesStr,
        "Number of Candidates: " + std::to_string(candidates.size()),
        "Number of Seats: " + std::to_string(numSeats)
    };
}

/**
 * @brief Formats a comma-separated list of candidate names for reporting.
 * @param candidates Candidate pointers to include in the list.
 * @returns Comma-separated name string with trailing whitespace removed.
 */
std::string createCandidatesListStr(const std::vector<Candidate *>& candidates) {
    std::string candidatesListStr = "";
    for (Candidate *candidate : candidates) {
        candidatesListStr += candidate->getName() + ", ";
    }
    // remove last comma and space
    candidatesListStr.erase(candidatesListStr.length() - 2);
    return candidatesListStr;
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

    std::vector<std::string> auditReportDetails = createAuditReport(candidates, numSeats);
    auditReportDetails.push_back("Elected Candidates: " + createCandidatesListStr(winnersList));
    reportGenerator->generateReport(auditReportDetails);
}

Candidate *PluralityElection::breakTie(const std::vector<Candidate *>& tiedCandidates){
    srand(time(0));
    int selection = rand() % (tiedCandidates.size());
    return tiedCandidates[selection];
}