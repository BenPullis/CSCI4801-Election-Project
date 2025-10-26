#include "STVElection.h"

STVElection::STVElection(int nSeats, bool sb) 
    : IElection(nSeats), shuffleBallots(sb)
{
    for (Candidate candidate : candidates) {
        candidatesPiles.insert({ candidate.getName(), 0 });
    }

    droopQuota = calculateDroopQuota();
}


STVElection::~STVElection() {
    delete helpWindow;
    delete reportGenerator;
    delete ballotHandler;
}


int STVElection::calculateDroopQuota() {
    return (ballots.size() / (numSeats + 1)) + 1;
}


Candidate STVElection::breakTie(std::vector<Candidate> tiedCandidates) {
    int latestFirstVote = 0;
    Candidate candWithLatestFirstVote;
    for (Candidate cand : tiedCandidates) {
        int firstVote = cand.getFirstVote();
        if (firstVote < latestFirstVote) {
            candWithLatestFirstVote = cand;
            latestFirstVote = firstVote;
        }
    }
    return candWithLatestFirstVote;
}


void STVElection::run() {
    if (shuffleBallots) {
        ballots = ballotHandler->shuffleBallots(ballots);
    }

    int numBallotsProcessed = 0;

    while (winnersList.size() < numSeats) {
        std::vector<std::string> candidatesToUpdate;

        // process ballots
        for (Ballot ballot : ballots) {
            Candidate ballotPref = ballot.getPreference();
            std::string prefName = ballotPref->getName();

            updateCandidatePiles(prefName);
            numBallotsProcessed++;

            if (candidatePiles[prefName] == 1) {
                ballotPref.setFirstVote(numBallotsProcessed);
            }

            if (candidatePiles[prefName] == droopQuota) {
                addWinner(*ballotPref);
                if (winnersList.size() == numSeats) break;
                candidatesToUpdate.push_back(prefName);
            }
        }

        if (winnersList.size() == numSeats) {
            // winners have been determined. put remaining eligible candidates in losersList
            for (Candidate cand : candidates) {
                if (findCandidateByName(cand.getName(), winnersList) == nullptr
                    && findCandidateByName(cand.getName(), losersList) == nullptr
                ) {
                    addLoser(cand);
                }
            }
        }

        if (candidatesToUpdate.size() == 0) {
            // no candidates reached quota. find candidate with least num of votes
            int minCount = ballots.size() + 1;
            std::vector<Candidate> tiedCands;

            for (const auto& candidateCount : candidatePiles) {
                int candCountNum = candidateCount.second;
                if (candCountNum < minCount) {
                    minCount = candCountNum;
                    tiedCands.clear();
                    tiedCands.push_back(findCandidateByName(candidateCount.first, candidates));

                } else if (candCountNum == minCount) {
                    tiedCands.push_back(findCandidateByName(candidateCount.first, candidates));
                }
            } 

            Candidate losingCandidate = tiedCands.size() > 1 
                ? breakTie(tiedCands) : tiedCands.at(0);
            addLoser(losingCandidate);
            candidatesToUpdate.push_back(losingCandidate.getName());
        }
        
        for (std::string candName : candidatesToUpdate) {
            for (Ballot &ballot : ballots) {
                std::unordered_map<String, int> ballotRanking = ballot.getCandidateRanking();
                std::string prefName = ballot.getPreference()->getName();
                if (prefName == candName) {
                    // need to update ballot's preference
                    int currCandRank = ballotRanking[prefName];
                    int lowestRank = candidates.size() + 1;
                    std::string newPrefName;

                    for (const auto& candidateRank : ballotRanking) {
                        // set ballot's new preference to the highest ranked candidate on ballot
                        // that isn't in either the winners or losers lists
                        int candRankNum = candidateRank.second;
                        if (candRankNum > currCandRank && candRankNum < lowestRank) {
                            std::string candName = candidateRank.first;
                            if (findCandidateByName(candName, winnersList) == nullptr
                                && findCandidateByName(candName, losersList) == nullptr
                            ) {
                                lowestRank = candRankNum;
                                newPrefName = candName;
                            }
                        }
                    }

                    ballot.setPreference(newPrefName);
                }
            }
        }
    }

    reportGenerator->generateReport(createAuditReportDetails(winnersList, losersList));
}


Candidate findCandidateByName(std::string candidateName, std::vector<Candidate> candidates) {
    for (Candidate candidate : candidates) {
        if (candidate.getName() == candidateCount.first) {
            return candidate;
        }
    }
    return nullptr;
}


std::vector<std::string> createAuditReportDetails(
    std::vector<Candidate> winnersList, std::vector<Candidate> losersList
) {
    return { "" };
}
