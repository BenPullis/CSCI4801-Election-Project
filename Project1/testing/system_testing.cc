#include <gtest/gtest.h>
#include "AuditReport.h"
#include "Ballot.h"
#include "BallotHandler.h"
#include "Candidate.h"
#include "HelpWindow.h"
#include "PluralityElection.h"
#include "STVElection.h"

class TestableSTVElection : public STVElection {
    public:
        TestableSTVElection(int nSeats, bool sb, std::vector<Ballot *> b, std::vector<Candidate *> c)
            : STVElection(nSeats, sb, b, c, new BallotHandler())
        {}
        std::vector<Candidate *> getWinners() { return getWinnersList(); }
};

class TestablePluralityElection : public PluralityElection {
    public:
        TestablePluralityElection(int nSeats, std::vector<Ballot *> b, std::vector<Candidate *> c)
            : PluralityElection(nSeats, b, c)
        {}
        std::vector<Candidate *> getWinners() { return getWinnersList(); }
};

TEST(STVSystemTest, STVRunThrough) {
    BallotHandler handler;

    // Simulate user entering file info via stdin
    auto cinbuf = std::cin.rdbuf();
    std::istringstream input("2\n../testing/ballots1.csv\n../testing/ballots2.csv\n");
    std::cin.rdbuf(input.rdbuf());

    handler.handleFiles();

    std::cin.rdbuf(cinbuf);

    // Verify candidates
    auto candidates = handler.getCandidates();
    ASSERT_EQ(candidates.size(), 3) << "Expected 3 candidates from ballots1.csv header.";
    EXPECT_EQ(candidates[0]->getName(), "Alice");
    EXPECT_EQ(candidates[1]->getName(), "Bob");
    EXPECT_EQ(candidates[2]->getName(), "Charlie");

    // Verify ballots (2 from first file, 1 from second)
    auto ballots = handler.receiveBallots();
    EXPECT_EQ(ballots.size(), 3) << "Expected 3 total ballots.";

    TestableSTVElection election(1, false, ballots, candidates);
    election.run();

    auto winners = election.getWinners();
    ASSERT_EQ(1u, winners.size());
    EXPECT_EQ("Alice", winners[0]->getName());
}

TEST(PluralitySystemTest, PluralityRunThrough) {

}