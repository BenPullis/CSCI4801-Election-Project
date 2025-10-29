#ifndef IELECTION_H_
#define IELECTION_H_

#include <vector>
#include "AuditReport.h"
#include "BallotHandler.h"
#include "Ballot.h"
#include "Candidate.h"


/**
 * @class IElection
 * @brief Abstract class for an election. Should be implemented concretely with both STV and Plurality. 
 */
class IElection{

    public:

    /**
     * @brief Pure virtual method to run an election.
     * @note All child classes MUST override this function.
     */
        virtual void run() = 0;

        /**
     * @brief Destructor for IElection.
     */
        virtual ~IElection() = default;

    protected:

        int numSeats;
        AuditReport *reportGenerator;
        BallotHandler *ballotHandler;
        std::vector<Ballot *> ballots;
        std::vector<Candidate *> candidates;
        std::vector<Candidate *> winnersList;

    /**
     * @brief Pure virtual Function to fairly breaks a tie between 2+ Candidates.
     * @param tiedCandidates The candidates who are tied.
     * @return The wining Candidate.
     * @note All child classes MUST override this function.
     */
        virtual Candidate *breakTie(const std::vector<Candidate *>& tiedCandidates) = 0;

    /**
     * @brief Retrieves the list of winners for an election.
     * @return A vector containing the winning Candidates.
     */
        std::vector<Candidate *> getWinnersList(){ return winnersList; }

    /**
     * @brief Adds a winner to the winners list.
     * @param c Which is the Candidate to be added to the vector of winners.
     */    
        void addWinner(Candidate *c){ winnersList.push_back(c); }
};

#endif /* IELECTION_H_ */