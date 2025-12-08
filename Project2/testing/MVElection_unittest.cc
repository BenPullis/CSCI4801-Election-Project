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
        new Ballot(1, {1, 1, 1}),   // John, Tim, Marvin all chosen
        new Ballot(2, {1, 0, 0}),   // Only John
        new Ballot(3, {1, 1, 0})    // John and Tim
    };

    TestableMVElection election(1, ballots, candidates);
    election.run();
    std::vector<Candidate*> winners = election.getWinners();

    ASSERT_EQ(winners.size(), 1);
    EXPECT_EQ(winners[0]->getName(), "John Pork");
}

TEST(MVTests, MVMultipleSeatsRunFunction) {
    Candidate* john_pork = new Candidate("John Pork", "TikTok Party");
    Candidate* tim_cheese = new Candidate("Tim Cheese", "Reels Party");
    Candidate* marvin_beak = new Candidate("Marvin Beak", "Shorts Party");

    std::vector<Candidate*> candidates = {john_pork, tim_cheese, marvin_beak};

    std::vector<Ballot*> ballots = {
        new Ballot(1, {1,1,1}),
        new Ballot(2, {1,0,0}),
        new Ballot(3, {1,1,1}),
        new Ballot(4, {0,1,0}),
        new Ballot(5, {1,0,0}),
        new Ballot(6, {0,1,1})
    };

    TestableMVElection election(2, ballots, candidates);
    election.run();
    std::vector<Candidate*> winners = election.getWinners();

    ASSERT_EQ(winners.size(), 2);

    // Order not guaranteed, so check membership
    std::vector<std::string> names = {
        winners[0]->getName(),
        winners[1]->getName()
    };

    EXPECT_TRUE(std::find(names.begin(), names.end(), "John Pork") != names.end());
    EXPECT_TRUE(std::find(names.begin(), names.end(), "Tim Cheese") != names.end());
}

TEST(MVTests, MVTieRunFunction) {
    Candidate* john_pork = new Candidate("John Pork", "TikTok Party");
    Candidate* tim_cheese = new Candidate("Tim Cheese", "Reels Party");
    Candidate* marvin_beak = new Candidate("Marvin Beak", "Shorts Party");

    std::vector<Candidate*> candidates = {john_pork, tim_cheese, marvin_beak};

    std::vector<Ballot*> ballots = {
        new Ballot(1, {1,1,0}),
        new Ballot(2, {1,1,0}),
        new Ballot(3, {0,1,0}),
        new Ballot(4, {1,0,0}),
        new Ballot(5, {0,1,0}),
        new Ballot(6, {1,0,1})
    };

    TestableMVElection election(1, ballots, candidates);
    election.run();
    std::vector<Candidate*> winners = election.getWinners();

    ASSERT_EQ(winners.size(), 1);

    std::string name = winners[0]->getName();
    EXPECT_TRUE(name == "John Pork" || name == "Tim Cheese");
}