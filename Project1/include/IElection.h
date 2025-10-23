#ifndef IELECTION_H_
#define IELECTION_H_

#include <vector>
#include "HelpWindow.h"
#include "AuditReport.h"
#include "BallotHandler.h"
#include "Ballot.h"
#include "Candidate.h"

class IElection{

    public:

        virtual void run() = 0;

        virtual ~IElection() = default;

    protected:

        int numSeats;
        HelpWindow *helpWindow;
        AuditReport *reportGenerator;
        BallotHandler *ballotHandler;
        std::vector<Ballot> ballots;
        std::vector<Candidate> candidates;
        std::vector<Candidate> winnersList;

        virtual Candidate breakTie(const std::vector<Candidate>& tiedCandidates) = 0;

        std::vector<Candidate> getWinnersList(){ return winnersList; }

        void addWinner(Candidate c){ winnersList.push_back(c); }
};

#endif /* IELECTION_H_ */