#ifndef STVELECTION_H_
#define STVELECTION_H_

#include <algorithm>
#include <format>
#include <unordered_map>
#include <climits>
#include <string>
#include <vector>
#include "IElection.h"
#include "Candidate.h"
#include "BallotHandler.h"
#include <iostream>

/**
 * @class STVElection
 * @brief Class representing a Single Transferable Vote (STV) election.
 */
class STVElection : public IElection {
    public:
        /**
         * @brief Constructor for STVElection.
         * @param nSeats Number of seats to be filled in the election.
         * @param shuffleBallots Boolean indicating whether to shuffle ballots.
         * @param ballots Vector of pointers to Ballot objects representing the ballots cast in the election.
         * @param candidates Vector of pointers to Candidate objects representing the candidates in the election.
         * @param ballotHandler Pointer to a BallotHandler object for managing ballot operations.
         */
        STVElection(int nSeats, bool shuffleBallots,
            std::vector<Ballot *> ballots, std::vector<Candidate *> candidates, BallotHandler *ballotHandler);

        /**
         * @brief Destructor for STVElection.
         */
        ~STVElection();

        /**
         * @brief Runs the STV election process.
         */
        void run() override;

        /**
         * @brief Adds a candidate to the losers list.
         * @param candidate Pointer to the Candidate object to be added to the losers list.
         */
        void addLoser(Candidate *candidate) { losersList.push_back(candidate); }

        /**
         * @brief Retrieves the list of losers for the election.
         * @return A vector containing the losing Candidates.
         */
        std::vector<Candidate *> getLosersList() { return losersList; }

    private:
        std::unordered_map<std::string, int> candidatePiles;
        int droopQuota;
        bool shuffleBallots;
        std::vector<Candidate *> losersList;
        BallotHandler *ballotHandler;

        /**
         * @brief Calculates the Droop quota for the election.
         * @returns The Droop quota as an int
         */
        int calculateDroopQuota();

        /**
         * @brief Updates candidate piles by processing ballots and promoting/eliminating candidates.
         * @param numBallotsProcessed Running count of ballots processed, used for tie-breaking.
         */
        void updateCandidatePiles(int &numBallotsProcessed);

        /**
         * @brief Breaks ties by selecting the candidate with the latest first vote.
         * @param tiedCandidates Candidates sharing the same minimum vote total.
         * @returns Pointer to the candidate that should be eliminated in the tie.
         */
        Candidate *breakTie(const std::vector<Candidate *>& tiedCandidates) override;
};

#endif /* STVELECTION_H_ */