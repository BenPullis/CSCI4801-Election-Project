#include "MVElection.h"

MVElection::MVElection(int nSeats, std::vector<Ballot *>b, std::vector<Candidate *> c){
    numSeats = nSeats;
    ballots = b;
    candidates = c;
    reportGenerator = new AuditReport();
}


MVElection::~MVElection(){
    delete reportGenerator;
}


Candidate* MVElection::findCandidateByName(std::string candidateName, const std::vector<Candidate *> candidates){
    for(Candidate *c : candidates){
        if(c->getName() == candidateName){
            return c;
        }
    }
    return nullptr;
}


Candidate* MVElection::breakTie(const std::vector<Candidate *>& tiedCandidates){
    srand(time(0));
    int selection = rand() % (tiedCandidates.size());
    return tiedCandidates[selection];
}


std::vector<std::string> MVElection::createAuditReport(const std::vector<Candidate *>& candidates, int numSeats) {
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
        "Election Type: MV",
        "Election Date: " + dateStr,
        "Candidates: " + candidatesStr,
        "Number of Candidates: " + std::to_string(candidates.size()),
        "Number of Seats: " + std::to_string(numSeats)
    };
}

std::string MVElection::createCandidatesListStr(const std::vector<Candidate *>& candidates) {
    std::string candidatesListStr = "";
    for (Candidate *candidate : candidates) {
        candidatesListStr += candidate->getName() + ", ";
    }
    // remove last comma and space
    candidatesListStr.erase(candidatesListStr.length() - 2);
    return candidatesListStr;
}


void MVElection::run(){
    for (Ballot *ballot : ballots) {
        std::map<int, std::string> candidateRankings = ballot->getCandidateRankings();

        for(auto &[rank, name] : candidateRankings){
            if (candidateCounts.find(name) != candidateCounts.end()) {
            setCandidateCounts(name, candidateCounts[name] + 1);
            } else {
            setCandidateCounts(name, 1);
            }
        }
    }

    std::vector<std::pair<std::string, int>> sortedCandidateCounts(candidateCounts.begin(), candidateCounts.end());

    std::sort(sortedCandidateCounts.begin(), sortedCandidateCounts.end(), [](const auto& a, const auto& b) {
        return a.second > b.second;
    });

    int seatsFilled = 0;
    int i = 0;
    std::vector<Candidate *> tiedCandidates;

    while (seatsFilled < numSeats && i < sortedCandidateCounts.size()) {
        int currentVotes = sortedCandidateCounts[i].second;
        tiedCandidates.clear();

        while (i < sortedCandidateCounts.size() && sortedCandidateCounts[i].second == currentVotes) {
            Candidate *cand = findCandidateByName(sortedCandidateCounts[i].first, candidates);
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

    std::cout << "\n";
    for (std::string reportLine : auditReportDetails) {
        std::cout << reportLine << "\n";
    }
}