#include <gtest/gtest.h>
#include "Ballot.h"
#include "Candidate.h"
#include <map>
#include <string>

/**
 * @brief Test suite for Ballot class.
 */
class BallotTest : public ::testing::Test {
    protected:
        std::map<int, std::string> rankings;

        void SetUp() override {
            rankings = {{1, "c1"},{2, "c2"},{3, "c3"}};
        }
};


/**
 * @brief Test suite for Candidate class.
 */
class CandidateTest : public ::testing::Test {
    protected:
        Candidate c;

        void SetUp() override {
            c = Candidate("testName", "testParty");
        }
};

/**
 * @test Make sure Candidate Constructor properly sets up.
 */
TEST_F(CandidateTest, ConstructorInitialization) {
    EXPECT_EQ(c.getName(), "testName");
    EXPECT_EQ(c.getParty(), "testParty");
    EXPECT_EQ(c.getFirstVote(), -1);
}

/**
 * @test Make sure getName returns correct name.
 */
TEST_F(CandidateTest, GetName) {
    EXPECT_EQ(c.getName(), "testName");
}

/**
 * @test Make sure getParty returns correct party.
 */
TEST_F(CandidateTest, GetParty) {
    EXPECT_EQ(c.getParty(), "testParty");
}

/**
 * @test Make sure getFirstVote returns correct firstVote.
 */
TEST_F(CandidateTest, GetFirstVote) {
    EXPECT_EQ(c.getFirstVote(), -1);
}

/**
 * @test Make sure setFirstVote properly changes firstVote.
 */
TEST_F(CandidateTest, SetFirstVote) {
    c.setFirstVote(5);
    EXPECT_EQ(c.getFirstVote(), 5);

    c.setFirstVote(100);
    EXPECT_EQ(c.getFirstVote(), 100);
}



/**
 * @test Make sure Ballot Constructor properly sets up.
 */
TEST_F(BallotTest, ConstructorInitialization) {
    Ballot ballot(1, rankings);

    EXPECT_EQ(ballot.getId(), 1);
    EXPECT_EQ(ballot.getPreference(), 1);
    EXPECT_EQ(ballot.getCandidateRankings().size(), 3);
    EXPECT_EQ(ballot.getCandidateRankings().at(1), "c1");
}

/**
 * @test Make sure setPreference updates the value correctly.
 */
TEST_F(BallotTest, SetPreference) {
    Ballot ballot(2, rankings);

    ballot.setPreference(1);
    EXPECT_EQ(ballot.getPreference(), 1);

    ballot.setPreference(2);
    EXPECT_EQ(ballot.getPreference(), 2);

    ballot.setPreference(ballot.getPreference() + 1);
    EXPECT_EQ(ballot.getPreference(), 3);
}

/**
 * @test Make sure getCandidateRankings returns correct map.
 */
TEST_F(BallotTest, GetCandidateRankings) {
    Ballot ballot(3, rankings);
    std::map<int, std::string> rankings = ballot.getCandidateRankings();

    EXPECT_EQ(rankings.size(), 3);
    EXPECT_EQ(rankings[1], "c1");
    EXPECT_EQ(rankings[2], "c2");
    EXPECT_EQ(rankings[3], "c3");
}



int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}