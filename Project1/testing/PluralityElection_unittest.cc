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

TEST(RunUnitTests, RunFunction) {
    // Basic unit test for the run function, creates two candidates and three ballots with one seat to win.
    Candidate* john_pork = new Candidate("John Pork", "TikTok Party");
    Candidate* tim_cheese = new Candidate("Tim Cheese", "Reels Party");

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

TEST(RunUnitTests, MultipleSeatsRunFunction) {
    // Unit test for the run function that creates three candidates and six ballots with two seats to win.
    // Tests the PluralityElection class's capability of handling multiple seats when electing someone.
    Candidate* john_pork = new Candidate("John Pork", "TikTok Party");
    Candidate* tim_cheese = new Candidate("Tim Cheese", "Reels Party");
    Candidate* marvin_beak = new Candidate("Marvin Beak", "Shorts Party");

    std::vector<Candidate*> candidates = {john_pork, tim_cheese, marvin_beak};

    std::vector<Ballot*> ballots = {
        new Ballot(1, {{1, "John Pork"}}),
        new Ballot(2, {{1, "John Pork"}}),
        new Ballot(3, {{1, "Tim Cheese"}}),
        new Ballot(4, {{1, "Tim Cheese"}}),
        new Ballot(5, {{1, "John Pork"}}),
        new Ballot(6, {{1, "Marvin Beak"}})
    };

    TestablePluralityElection election(2, ballots, candidates);

    election.run();

    std::vector<Candidate*> winners = election.getWinners();

    ASSERT_EQ(winners.size(), 2) << "Expected exactly two winners.";
    EXPECT_EQ(winners[0]->getName(), "John Pork") << "Expected John Pork to win with 3 votes.";
    EXPECT_EQ(winners[1]->getName(), "Tim Cheese") << "Expected Tim Cheese to win with 2 votes.";
}

TEST(RunUnitTests, TieRunFunction) {
    // Unit test for the run function that has a tie for one seat.
    // Passes if either one of the tied candidates receives it (up to chance).
    Candidate* john_pork = new Candidate("John Pork", "TikTok Party");
    Candidate* tim_cheese = new Candidate("Tim Cheese", "Reels Party");
    Candidate* marvin_beak = new Candidate("Marvin Beak", "Shorts Party");

    std::vector<Candidate*> candidates = {john_pork, tim_cheese, marvin_beak};

    std::vector<Ballot*> ballots = {
        new Ballot(1, {{1, "John Pork"}}),
        new Ballot(2, {{1, "John Pork"}}),
        new Ballot(3, {{1, "Tim Cheese"}}),
        new Ballot(4, {{1, "Tim Cheese"}}),
        new Ballot(5, {{1, "Marvin Beak"}})
    };

    TestablePluralityElection election(1, ballots, candidates);

    election.run();

    std::vector<Candidate*> winners = election.getWinners();

    ASSERT_EQ(winners.size(), 1) << "Expected exactly one winner.";
    EXPECT_TRUE(winners[0]->getName() == "John Pork" || winners[0]->getName() == "Tim Cheese");
}