#include "gtest/gtest.h"
#include "MVElection.h"
#include "Candidate.h"
#include "Ballot.h"

class TestableMVElection : public MVElection {
    public:
        TestableMVElection(int nSeats, std::vector<Ballot *> b, std::vector<Candidate *> c)
            : MVElection(nSeats, b, c)
        {}
        std::vector<Candidate *> getWinners() { return getWinnersList(); }
};

TEST(MVTests, MVRunFunction) {
    Candidate* john_pork = new Candidate("John Pork", "TikTok Party");
    Candidate* tim_cheese = new Candidate("Tim Cheese", "Reels Party");
    Candidate* marvin_beak = new Candidate("Marvin Beak", "Shorts Party");

    std::vector<Candidate*> candidates = {john_pork, tim_cheese, marvin_beak};

    std::vector<Ballot*> ballots = {
        new Ballot(1, {{0, "John Pork"}, {1, "Tim Cheese"}, {2, "Marvin Beak"}}),
        new Ballot(2, {{0, "John Pork"}}),
        new Ballot(3, {{0, "Tim Cheese"}, {1, "John Pork"}})
    };

    TestableMVElection election(1, ballots, candidates);

    election.run();

    std::vector<Candidate*> winners = election.getWinners();

    ASSERT_EQ(winners.size(), 1) << "Expected exactly one winner.";
    EXPECT_EQ(winners[0]->getName(), "John Pork") << "Expected John Pork to win with 3 votes.";
}

TEST(MVTests, MVMultipleSeatsRunFunction) {
    Candidate* john_pork = new Candidate("John Pork", "TikTok Party");
    Candidate* tim_cheese = new Candidate("Tim Cheese", "Reels Party");
    Candidate* marvin_beak = new Candidate("Marvin Beak", "Shorts Party");

    std::vector<Candidate*> candidates = {john_pork, tim_cheese, marvin_beak};

    std::vector<Ballot*> ballots = {
        new Ballot(1, {{0, "John Pork"}, {1, "Tim Cheese"}, {2, "Marvin Beak"}}),
        new Ballot(2, {{0, "John Pork"}}),
        new Ballot(3, {{0, "Tim Cheese"}, {1, "John Pork"}, {2, "Marvin Beak"}}),
        new Ballot(4, {{0, "Tim Cheese"}, {1, "John Pork"}}),
        new Ballot(5, {{0, "John Pork"}}),
        new Ballot(6, {{0, "Marvin Beak"}, {1, "Tim Cheese"}})
    };

    TestableMVElection election(2, ballots, candidates);

    election.run();

    std::vector<Candidate*> winners = election.getWinners();

    ASSERT_EQ(winners.size(), 2) << "Expected exactly two winners.";
    EXPECT_EQ(winners[0]->getName(), "John Pork") << "Expected John Pork to win with 5 votes.";
    EXPECT_EQ(winners[1]->getName(), "Tim Cheese") << "Expected Tim Cheese to win with 4 votes.";
}

TEST(MVTests, MVTieRunFunction) {
    Candidate* john_pork = new Candidate("John Pork", "TikTok Party");
    Candidate* tim_cheese = new Candidate("Tim Cheese", "Reels Party");
    Candidate* marvin_beak = new Candidate("Marvin Beak", "Shorts Party");

    std::vector<Candidate*> candidates = {john_pork, tim_cheese, marvin_beak};

    std::vector<Ballot*> ballots = {
        new Ballot(1, {{0, "John Pork"}, {1, "Tim Cheese"}, {2, "Marvin Beak"}}),
        new Ballot(2, {{0, "John Pork"}, {1, "Tim Cheese"}}),
        new Ballot(3, {{0, "Tim Cheese"}, {1, "John Pork"}, {2, "Marvin Beak"}}),
        new Ballot(4, {{0, "Tim Cheese"}, {1, "John Pork"}}),
        new Ballot(5, {{0, "John Pork"}}),
        new Ballot(6, {{0, "Marvin Beak"}, {1, "Tim Cheese"}})
    };

    TestableMVElection election(1, ballots, candidates);

    election.run();

    std::vector<Candidate*> winners = election.getWinners();

    ASSERT_EQ(winners.size(), 1) << "Expected exactly one winner.";
    EXPECT_TRUE(winners[0]->getName() == "John Pork" || winners[0]->getName() == "Tim Cheese");
}