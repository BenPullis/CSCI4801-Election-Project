#include <gtest/gtest.h>
#include "BallotHandler.h"
#include "Ballot.h"
#include "Candidate.h"
#include <sstream>
#include <filesystem>
#include <algorithm>

/**
 * @brief Test suite for BallotHandler class.
 */
class BallotHandlerTest : public ::testing::Test {
protected:
    std::string auditReportFileName = "audit_report.txt";
    std::string path1;
    std::string path2;
    std::string path3;

    void SetUp() override {
        path1 = "../testing/ballots1.csv";
        path2 = "../testing/ballots2.csv";
        path3 = "../testing/ballots6.csv";
    }
};

TEST_F(BallotHandlerTest, MVBallotsAreParsedCorrectly) {
    BallotHandler handler;

    auto cinbuf = std::cin.rdbuf();
    std::istringstream input("1\n" + path3 + "\n");
    std::cin.rdbuf(input.rdbuf());

    handler.handleFiles();
    std::cin.rdbuf(cinbuf);

    auto candidates = handler.getCandidates();
    ASSERT_EQ(candidates.size(), 3) << "Expected 3 candidates from ballots1.csv header.";
    EXPECT_EQ(candidates[0]->getName(), "Alice");
    EXPECT_EQ(candidates[1]->getName(), "Bob");
    EXPECT_EQ(candidates[2]->getName(), "Charlie");

    auto ballots = handler.receiveBallots();
    ASSERT_EQ(ballots.size(), 2) << "Expected exactly two MV Ballot";

    Ballot* b = ballots[0];
    const std::vector<int>& votes = b->getMVVotes();

    ASSERT_EQ(votes.size(), 3);
    EXPECT_EQ(votes[0], 1);
    EXPECT_EQ(votes[1], 0);
    EXPECT_EQ(votes[2], 1);
}

/**
 * @test Ensure handleFiles() properly parses header and ballots
 *       from multiple files.
 */
TEST_F(BallotHandlerTest, TestHeaderParsing) {
    BallotHandler handler;

    // Simulate user entering file info via stdin
    auto cinbuf = std::cin.rdbuf();
    std::istringstream input("2\n" + path1 + "\n" + path2 + "\n");
    std::cin.rdbuf(input.rdbuf());

    handler.handleFiles();

    std::cin.rdbuf(cinbuf);

    // Verify election type and number of seats
    EXPECT_EQ("pv", handler.getElectionType()) << "Expected 'pv' election type";
    // Verify number of seats
    EXPECT_EQ(1, handler.getNumSeats()) << "Expected 1 seat";

    // Verify candidates
    auto candidates = handler.getCandidates();
    ASSERT_EQ(candidates.size(), 3) << "Expected 3 candidates from ballots1.csv header.";
    EXPECT_EQ(candidates[0]->getName(), "Alice");
    EXPECT_EQ(candidates[1]->getName(), "Bob");
    EXPECT_EQ(candidates[2]->getName(), "Charlie");

    // Verify ballots (2 from first file, 1 from second)
    auto ballots = handler.receiveBallots();
    EXPECT_EQ(ballots.size(), 3) << "Expected 3 total ballots.";
}

/**
 * @test Ensure shuffleBallots() changes order but keeps same elements.
 */
TEST_F(BallotHandlerTest, ShuffleBallotsKeepsSameElements) {
    Ballot b1(1, {{1, "A"}});
    Ballot b2(2, {{1, "B"}});
    Ballot b3(3, {{1, "C"}});
    std::vector<Ballot*> ballots = { &b1, &b2, &b3 };

    BallotHandler handler;
    auto shuffled = handler.shuffleBallots(ballots);

    EXPECT_EQ(shuffled.size(), ballots.size());

    std::vector<int> originalIds, shuffledIds;
    for (auto* b : ballots) originalIds.push_back(b->getId());
    for (auto* b : shuffled) shuffledIds.push_back(b->getId());

    std::sort(originalIds.begin(), originalIds.end());
    std::sort(shuffledIds.begin(), shuffledIds.end());
    EXPECT_EQ(originalIds, shuffledIds);
}

/**
 * @test Ensure destructor cleans memory safely (use sanitizer/valgrind).
 */
TEST_F(BallotHandlerTest, DestructorCleansMemory) {
    auto* handler = new BallotHandler();

    std::istringstream input("1\n" + path1 + "\n");
    std::cin.rdbuf(input.rdbuf());

    handler->handleFiles();
    auto countBefore = handler->receiveBallots().size();
    EXPECT_GT(countBefore, 0);

    delete handler; // destructor should safely free memory
    SUCCEED();
}

/**
 * @test Ensure invalid STV ballots are removed correctly.
 */
TEST_F(BallotHandlerTest, RemoveInvalidSTVBallots) {
    BallotHandler handler;

    // Simulate user entering file info via stdin
    auto cinbuf = std::cin.rdbuf();
    std::istringstream input("1\n../testing/ballots4.csv\n");
    std::cin.rdbuf(input.rdbuf());

    handler.handleFiles();

    std::cin.rdbuf(cinbuf);

    // Verify ballots (4 valid, 4 are removed)
    auto ballots = handler.receiveBallots();
    EXPECT_EQ(ballots.size(), 4) << "Expected 4 total ballots.";
}
