#include "STVElection.h"

STVElection::STVElection(int nSeats, bool sb) 
    : IElection(), shuffleBallots(sb)
{
    numSeats = nSeats;
    for (Candidate *candidate : candidates) {
        candidatePiles.insert({ candidate->getName(), 0 });
    }

    droopQuota = calculateDroopQuota();
}


STVElection::~STVElection() {
    delete reportGenerator;
    delete ballotHandler;
    for (Ballot *ballot : ballots) {
        delete ballot;
    }
    for (Candidate *cand : candidates) {
        delete cand;
    }
}


int STVElection::calculateDroopQuota() {
    return (ballots.size() / (numSeats + 1)) + 1;
}


Candidate *STVElection::breakTie(const std::vector<Candidate *>& tiedCandidates) {
    int latestFirstVote = 0;
    Candidate *candWithLatestFirstVote;
    for (Candidate *cand : tiedCandidates) {
        int firstVote = cand->getFirstVote();
        if (firstVote < latestFirstVote) {
            candWithLatestFirstVote = cand;
            latestFirstVote = firstVote;
        }
    }
    return candWithLatestFirstVote;
}


Candidate *findCandidateByName(std::string candidateName, const std::vector<Candidate *> candidates) {
    for (Candidate *candidate : candidates) {
        if (candidate->getName() == candidateName) {
            return candidate;
        }
    }
    return nullptr;
}


std::vector<std::string> createAuditReportDetails(
    std::vector<Candidate *> winnersList,
    std::vector<Candidate *> losersList
) {
    return { "" };
}


std::vector<Candidate *> getTiedCandidates(
    const std::unordered_map<std::string, int>& candidatePiles,
    const std::vector<Candidate *>& candidates
) {
    // find candidate with least num of votes
    int minCount = INT_MAX;
    std::vector<Candidate *> tiedCands;

    for (const auto& candidateCount : candidatePiles) {
        std::string candName = candidateCount.first;
        int candCountNum = candidateCount.second;
        if (candCountNum < minCount) {
            minCount = candCountNum;

            // no more tie, clear the tiedCands vec
            tiedCands.clear();
            tiedCands.push_back(findCandidateByName(candName, candidates));

        } else if (candCountNum == minCount) {
            tiedCands.push_back(findCandidateByName(candName, candidates));
        }
    } 

    return tiedCands;
}


void updateBallotPreferences(
    std::vector<std::string> candidatesToUpdate,
    std::vector<Ballot *>& ballots,
    const std::vector<Candidate *>& winnersList,
    const std::vector<Candidate *>& losersList,
    int numCandidates
) {
    for (std::string candName : candidatesToUpdate) {
        for (Ballot *ballot : ballots) {
            std::map<int, std::string> ballotRanking = ballot->getCandidateRankings();
            int preference = ballot->getPreference();

            if (ballotRanking[preference] == candName) {
                // need to update ballot's preference
                for (int i = preference + 1; i < numCandidates + 1; i++) {
                    std::string nextPrefName = ballotRanking[i];

                    // check if the next preference is still eligible
                    if (findCandidateByName(nextPrefName, winnersList) == nullptr
                        && findCandidateByName(nextPrefName, losersList) == nullptr
                    ) {
                        ballot->setPreference(i);
                        break;
                    }   
                }
            }
        }
    }
}


void removeWinningBallots(std::vector<Ballot *> &ballots, std::string winnerName) {
    ballots.erase(
        std::remove_if(
            ballots.begin(),
            ballots.end(),
            [winnerName](Ballot *b) { 
                std::map<int, std::string> ranking = b->getCandidateRankings();
                int pref = b->getPreference();
                return ranking[pref] == winnerName;
            }
        ),
        ballots.end()
    );
}


void STVElection::updateCandidatePiles(int &numBallotsProcessed) {
    std::vector<std::string> candidatesToUpdate;

    // process all ballots, increment candidate piles
    for (Ballot *ballot : ballots) {
        int ballotPref = ballot->getPreference();
        std::string prefName = ballot->getCandidateRankings()[ballotPref];
        Candidate *prefCandidate = findCandidateByName(prefName, candidates);

        candidatePiles[prefName]++;
        numBallotsProcessed++;

        if (candidatePiles[prefName] == 1) {
            // candidate's first vote has been received
            prefCandidate->setFirstVote(numBallotsProcessed);
        }

        if (candidatePiles[prefName] == droopQuota) {
            // add to winners list
            addWinner(prefCandidate);
            removeWinningBallots(ballots, prefName);
            if (winnersList.size() == numSeats) break;
            candidatesToUpdate.push_back(prefName);
        }
    }

    if (winnersList.size() == numSeats) {
        // winners have been determined. put remaining eligible candidates in losersList
        for (Candidate *cand : candidates) {
            if (findCandidateByName(cand->getName(), winnersList) == nullptr
                && findCandidateByName(cand->getName(), losersList) == nullptr
            ) {
                addLoser(cand);
            }
        }
        return;
    }

    if (candidatesToUpdate.size() == 0) {
        std::vector<Candidate *> tiedCands = getTiedCandidates(candidatePiles, candidates);
        Candidate *losingCandidate = tiedCands.size() > 1 
            ? breakTie(tiedCands) : tiedCands.at(0);
        addLoser(losingCandidate);
        candidatesToUpdate.push_back(losingCandidate->getName());
    }

    updateBallotPreferences(candidatesToUpdate, ballots, winnersList, losersList, candidates.size());
}


void STVElection::run() {
    if (shuffleBallots) {
        ballotHandler->shuffleBallots(ballots);
    }

    int numBallotsProcessed = 0; // used to break ties when eliminating candidates

    while (winnersList.size() < numSeats) {
        updateCandidatePiles(numBallotsProcessed);
    }

    reportGenerator->generateReport(createAuditReportDetails(winnersList, losersList));
}
