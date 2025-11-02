#include "gtest/gtest.h"
#include "PluralityElection.h"
#include "Candidate.h"
#include "Ballot.h"

class TestablePluralityElection : public PluralityElection {
    public:
        TestablePluralityElection(int nSeats, std::vector<Ballot *> b, std::vector<Candidate *> c)
            : PluralityElection(nSeats, b, c)
        {}
        std::vector<Candidate *> getWinners() { return getWinnersList(); }
};

TEST(PluralityElectionTest, RunFunctionBasic) {
    Candidate* john_pork = new Candidate("John Pork", "TikTok Party");
    Candidate* tim_cheese   = new Candidate("Tim Cheese", "Reels Party");

    std::vector<Candidate*> candidates = {john_pork, tim_cheese};

    std::vector<Ballot*> ballots = {
        new Ballot(1, {{1, "John Pork"}}),
        new Ballot(2, {{1, "John Pork"}}),
        new Ballot(3, {{1, "Tim Cheese"}})
    };

    TestablePluralityElection election(1, ballots, candidates);

    election.run();

    std::vector<Candidate*> winners = election.getWinners();

    ASSERT_EQ(winners.size(), 1) << "Expected exactly one winner.";
    EXPECT_EQ(winners[0]->getName(), "John Pork") << "Expected John Pork to win with 2 votes.";
}