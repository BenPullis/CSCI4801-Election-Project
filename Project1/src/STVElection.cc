#include "STVElection.h"

STVElection::STVElection(int nSeats, bool sb, std::vector<Ballot *>b, std::vector<Candidate *> c, BallotHandler *bh) 
    : IElection(), shuffleBallots(sb), ballotHandler(bh)
{
    ballots = b;
    candidates = c;
    numSeats = nSeats;
    for (Candidate *candidate : candidates) {
        candidatePiles.insert({ candidate->getName(), 0 });
    }
    reportGenerator = new AuditReport();

    droopQuota = calculateDroopQuota();
}


STVElection::~STVElection() {
    delete reportGenerator;
}


int STVElection::calculateDroopQuota() {
    return (ballots.size() / (numSeats + 1)) + 1;
}


Candidate *STVElection::breakTie(const std::vector<Candidate *>& tiedCandidates) {
    // choose the candidate with the latest (largest) firstVote value
    int latestFirstVote = -1;
    Candidate *candWithLatestFirstVote = nullptr;
    for (Candidate *cand : tiedCandidates) {
        int firstVote = cand->getFirstVote();
        if (firstVote > latestFirstVote) {
            candWithLatestFirstVote = cand;
            latestFirstVote = firstVote;
        }
    }
    return candWithLatestFirstVote;
}


/**
 * @brief Retrieves a candidate pointer by name from a supplied list.
 * @param candidateName Name of the candidate being searched for.
 * @param candidates Collection of candidate pointers to scan.
 * @returns Matching candidate pointer or nullptr when not found.
 */
Candidate *findCandidateByName(std::string candidateName, const std::vector<Candidate *> candidates) {
    for (Candidate *candidate : candidates) {
        if (candidate->getName() == candidateName) {
            return candidate;
        }
    }
    return nullptr;
}


/**
 * @brief Builds the set of candidates currently tied at the minimum vote count.
 * @param candidatePiles Map of candidate names to their current ballot counts.
 * @param candidates All active candidates in the contest.
 * @returns Vector of candidate pointers sharing the minimum vote total.
 */
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


/**
 * @brief Advances ballots to the next valid preference after winner or loser changes.
 * @param candidatesToUpdate Candidate names whose ballots should be advanced.
 * @param ballots All ballots being processed.
 * @param winnersList Candidates already elected.
 * @param losersList Candidates already eliminated.
 * @param numCandidates Total number of candidates in the election.
 */
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
                for (int i = preference + 1; i < numCandidates; i++) {
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

        if (candidatePiles[prefName] == droopQuota
            && findCandidateByName(prefName, winnersList) == nullptr
            && findCandidateByName(prefName, losersList) == nullptr
        ) {
            // add to winners list
            addWinner(prefCandidate);
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
        // no winners this round; need to eliminate the candidate with the least votes
        std::vector<Candidate *> tiedCands = getTiedCandidates(candidatePiles, candidates);
        Candidate *losingCandidate = tiedCands.size() > 1 
            ? breakTie(tiedCands) : tiedCands.at(0);
        addLoser(losingCandidate);
        candidatesToUpdate.push_back(losingCandidate->getName());
    }

    updateBallotPreferences(candidatesToUpdate, ballots, winnersList, losersList, candidates.size());
}


/**
 * @brief Generates the static portion of the audit report metadata.
 * @param candidates All candidates participating in the election.
 * @param numSeats Number of seats allocated for this race.
 * @returns Vector of formatted strings describing election metadata.
 */
std::vector<std::string> createAuditReportDetails(const std::vector<Candidate *>& candidates, int numSeats) {
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
    if (candidatesStr.length() > 1) {
        candidatesStr.erase(candidatesStr.length() - 2);
    }

    return {
        "Election Type: STV",
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
std::string createCandidatesListString(const std::vector<Candidate *>& candidates) {
    std::string candidatesListStr = "";
    for (Candidate *candidate : candidates) {
        candidatesListStr += candidate->getName() + ", ";
    }
    // remove last comma and space
    if (candidatesListStr.length() > 1) {
        candidatesListStr.erase(candidatesListStr.length() - 2);
    }
    return candidatesListStr;
}


void STVElection::run() {
    if (shuffleBallots) {
        ballots = ballotHandler->shuffleBallots(ballots);
    }

    int numBallotsProcessed = 0; // used to break ties when eliminating candidates
    int algorithmPassNum = 0;

    std::vector<std::string> auditReportDetails = createAuditReportDetails(candidates, numSeats);

    // append initial ballot preferences to audit report details string
    for (Ballot *ballot : ballots) {
        auditReportDetails.push_back(std::format(
            "Ballot ID {:d} preference at start of election: {:s}", 
            ballot->getId(), 
            ballot->getCandidateRankings()[ballot->getPreference()]
        ));
    }

    while (winnersList.size() < numSeats) {
        algorithmPassNum++;
        updateCandidatePiles(numBallotsProcessed);

        // update audit report details string
        for (Ballot *ballot : ballots) {
            auditReportDetails.push_back(std::format(
                "Ballot ID {:d} preference after pass #{:d}: {:s}", 
                ballot->getId(), 
                algorithmPassNum, 
                ballot->getCandidateRankings()[ballot->getPreference()]
            ));
        }
    }

    // winners and losers have been declared
    auditReportDetails.push_back("Elected Candidates: " + createCandidatesListString(winnersList));
    auditReportDetails.push_back("Non-Elected Candidates: " + createCandidatesListString(losersList));
    reportGenerator->generateReport(auditReportDetails);

    for (std::string detail : auditReportDetails) {
        std::cout << detail << "\n";
    }
}
