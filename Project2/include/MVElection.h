#ifndef MVELECTION_H
#define MVELECTION_H


//TODO: remove ones not used
#include <map>
#include <format>
#include "IElection.h"
#include "Candidate.h"
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <algorithm>


class MVElection : public IElection {
    public:
        /**
         * @brief Constructor for MVElection.
         * @param nSeats Number of seats to be filled in the election.
         * @param b Vector of pointers to Ballot objects representing the ballots cast in the election.
         * @param c Vector of pointers to Candidate objects representing the candidates in the election.
         */
        MVElection(int nSeats, std::vector<Ballot *>b, std::vector<Candidate *> c);

        /**
         * @brief Destructor for MVElection.
         */
        ~MVElection();

        /**
         * @brief Runs an MV Election process.
         */
        void run() override;
        

    private:
        std::map<std::string, int> candidateCounts;

        /**
         * @brief Updates the candidateCounts array.
         * @param candidateName candidate to be updated.
         * @param ballotCount new value for that candidate.
         */
        void setCandidateCounts(std::string candidateName, int ballotCount){candidateCounts[candidateName] = ballotCount;}

        /**
         * @brief Return count for a certain candidate.
         * @param candidateName candidate to be updated.
         * @return returns the current count for that candidate.
         */
        int getCandidateCounts(std::string candidateName){return candidateCounts[candidateName];}

        /**
         * @brief Breaks ties by randomly selecting a candidate between those who are tied.
         * @param tiedCandidates Candidates that are tied.
         * @returns Pointer to the candidate that should win the tie.
         */
        Candidate *breakTie(const std::vector<Candidate *>& tiedCandidates) override;

        /**
         * @brief Finds the candidate with a particular name.
         * @param candidateName The name of the candidate we are looking for.
         * @param candidates The list of all Candidates.
         * @returns The Candidate with the matching name.
         */
        Candidate* findCandidateByName(std::string candidateName, const std::vector<Candidate *> candidates);

        /**
         * @brief Creates a string vector with details for an audit report.
         * @param numSeats The number of seats in the election.
         * @param candidates The list of all Candidates.
         * @returns String vector with audit report details.
         */
        std::vector<std::string> createAuditReport(const std::vector<Candidate *>& candidates, int numSeats);

        /**
        * @brief Formats a comma-separated list of candidate names for reporting.
        * @param candidates Candidate pointers to include in the list.
        * @returns Comma-separated name string with trailing whitespace removed.
        */
        std::string createCandidatesListStr(const std::vector<Candidate *>& candidates);
};


#endif