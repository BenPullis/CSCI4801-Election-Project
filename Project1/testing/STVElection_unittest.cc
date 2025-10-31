#include <gtest/gtest.h>
#include "STVElection.h"
 
void updateBallotPreferences(
    std::vector<std::string> candidatesToUpdate,
    std::vector<Ballot *>& ballots,
    const std::vector<Candidate *>& winnersList,
    const std::vector<Candidate *>& losersList,
    int numCandidates
);
std::vector<Candidate *> getTiedCandidates(
    const std::unordered_map<std::string, int>& candidatePiles,
    const std::vector<Candidate *>& candidates
);
Candidate *findCandidateByName(std::string candidateName, const std::vector<Candidate *> candidates);

class STVElectionTest : public ::testing::Test {
    public:
        void SetUp() {
            cand1 = new Candidate("Cand 1", "Party 1");
            cand2 = new Candidate("Cand 2", "Party 2");
            cand3 = new Candidate("Cand 3", "Party 1");
            cand4 = new Candidate("Cand 4", "Party 3");
            cand5 = new Candidate("Cand 5", "Party 2");
            candList = { cand1, cand2, cand3, cand4, cand5 };
            for (int i = 0; i < 100; i++) {
                emptyBallots.push_back(new Ballot(i, {}));
            }
        }

        void TearDown() {
            for (Candidate *cand : candList) {
                delete cand;
            }
            for (Ballot *ballot : emptyBallots) {
                delete ballot;
            }
        }

    protected:
        Candidate *cand1;
        Candidate *cand2;
        Candidate *cand3;
        Candidate *cand4;
        Candidate *cand5;
        std::vector<Candidate *> candList;
        std::vector<Ballot *> emptyBallots;
};


template <typename T>
bool areVectorsEqualIgnoreOrder(std::vector<T> a, std::vector<T> b) {
    if (a.size() != b.size()) {
        return false;
    }
    std::sort(a.begin(), a.end());
    std::sort(b.begin(), b.end());
    return a == b;
}

class TestableSTVElection : public STVElection {
    public:
        TestableSTVElection(int nSeats, bool sb, std::vector<Ballot *> b, std::vector<Candidate *> c)
            : STVElection(nSeats, sb, b, c, new BallotHandler())
        {}
        std::vector<Candidate *> getWinners() { return getWinnersList(); }
};

TEST_F(STVElectionTest, GetTiedCandidatesTest) {
    std::unordered_map<std::string, int> candidatePiles = {{ "Cand 1", 1 }};
    std::vector<Candidate *> candidates = { cand1 };
    std::vector<Candidate *> output = getTiedCandidates(candidatePiles, candidates);
    EXPECT_EQ(candidates, output);

    // Wrong pointer
    std::vector<Candidate *> invalidCandidatePointer = { new Candidate("Cand 1", "DFL") };
    EXPECT_NE(output, invalidCandidatePointer);

    candidatePiles = {
        { "Cand 1", 1 },
        { "Cand 2", 1 },
        { "Cand 3", 1 },
        { "Cand 4", 1 },
        { "Cand 5", 1 },
    };

    output = getTiedCandidates(candidatePiles, candList);
    EXPECT_TRUE(areVectorsEqualIgnoreOrder(candList, output));

    candidatePiles = {
        { "Cand 1", 2 },
        { "Cand 2", 2 },
        { "Cand 3", 2 },
        { "Cand 4", 2 },
        { "Cand 5", 0 },
    };
    std::vector<Candidate *> expected = { cand5 };
    output = getTiedCandidates(candidatePiles, candList);
    EXPECT_EQ(output, expected);

    candidatePiles = {
        { "Cand 1", 2 },
        { "Cand 2", 0 },
        { "Cand 3", 2 },
        { "Cand 4", 2 },
        { "Cand 5", 0 },
    };
    expected = { cand2, cand5 };
    output = getTiedCandidates(candidatePiles, candList);
    EXPECT_TRUE(areVectorsEqualIgnoreOrder(expected, output));
}


TEST(UpdateBallotPreferencesTest, AdvancesPreferencePastLosers) {
    Ballot *b = new Ballot(0, {{0, "A"}, {1, "B"}, {2, "C"}});
    std::vector<Ballot *> ballots = { b };
    Candidate *candB = new Candidate("B", "P");
    std::vector<Candidate *> winnersList = {};
    std::vector<Candidate *> losersList = { candB };

    updateBallotPreferences({ "A" }, ballots, winnersList, losersList, 3);
    EXPECT_EQ(2, ballots[0]->getPreference());

    updateBallotPreferences({ "Z" }, ballots, winnersList, losersList, 3);
    EXPECT_EQ(2, ballots[0]->getPreference());

    delete b;
    delete candB;
}

TEST(FindCandidateByNameTest, FindsByNameOrReturnsNull) {
    Candidate *c1 = new Candidate("X", "P");
    Candidate *c2 = new Candidate("Y", "P");
    std::vector<Candidate *> list = { c1, c2 };

    Candidate *found = findCandidateByName("X", list);
    ASSERT_EQ(c1, found);

    Candidate *notFound = findCandidateByName("Z", list);
    EXPECT_EQ(nullptr, notFound);

    delete c1; delete c2;
}


TEST(UpdateBallotPreferencesTest, SkipsWinnersAndLosersCorrectly) {
    Ballot *b = new Ballot(0, {{0, "A"}, {1, "B"}, {2, "C"}});
    std::vector<Ballot *> ballots = { b };
    Candidate *candB = new Candidate("B", "P");
    Candidate *candC = new Candidate("C", "P");

    // if B is a winner, move to C
    std::vector<Candidate *> winners = { candB };
    std::vector<Candidate *> losers = {};
    updateBallotPreferences({ "A" }, ballots, winners, losers, 3);
    EXPECT_EQ(2, b->getPreference());

    b->setPreference(0);

    // if B is a loser, skip B to C
    winners.clear(); 
    losers = { candB };
    updateBallotPreferences({ "A" }, ballots, winners, losers, 3);
    EXPECT_EQ(2, b->getPreference());

    // all following preferences disqualified
    b->setPreference(0);
    winners = { candB, candC }; 
    losers.clear();
    updateBallotPreferences({ "A" }, ballots, winners, losers, 3);
    EXPECT_EQ(0, b->getPreference());

    delete b; delete candB; delete candC;
}

TEST(STVElectionRunIntegrationTest, ExpandedScenarios) {
    Candidate *c1 = new Candidate("Cand 1", "P1");
    Candidate *c2 = new Candidate("Cand 2", "P2");
    Candidate *c3 = new Candidate("Cand 3", "P3");
    std::vector<Candidate *> candidates = { c1, c2, c3 };

    Ballot *b0 = new Ballot(0, {{0, "Cand 1"}});
    Ballot *b1 = new Ballot(1, {{0, "Cand 1"}});
    Ballot *b2 = new Ballot(2, {{0, "Cand 2"}});
    Ballot *b3 = new Ballot(3, {{0, "Cand 2"}});
    Ballot *b4 = new Ballot(4, {{0, "Cand 3"}});
    std::vector<Ballot *> ballots = { b0, b1, b2, b3, b4 };

    TestableSTVElection election(2, false, ballots, candidates);
    election.run();
    auto winners = election.getWinners();
    EXPECT_EQ(2u, winners.size());
    EXPECT_EQ(winners[0]->getName(), "Cand 1");
    EXPECT_EQ(winners[1]->getName(), "Cand 2");

    // 1 seat with transfers
    Candidate *d1 = new Candidate("D1", "P");
    Candidate *d2 = new Candidate("D2", "P");
    Candidate *d3 = new Candidate("D3", "P");
    std::vector<Candidate *> creds = { d1, d2, d3 };

    Ballot *bb0 = new Ballot(0, {{0, "D1"}, {1, "D2"}, {2, "D3"}});
    Ballot *bb1 = new Ballot(1, {{0, "D1"}, {1, "D3"}, {2, "D2"}});
    Ballot *bb2 = new Ballot(2, {{0, "D2"}, {1, "D3"}, {2, "D1"}});
    Ballot *bb3 = new Ballot(3, {{0, "D3"}, {1, "D2"}, {2, "D1"}});
    std::vector<Ballot *> bset = { bb0, bb1, bb2, bb3 };

    TestableSTVElection election2(1, false, bset, creds);
    election2.run();
    auto winners2 = election2.getWinners();
    ASSERT_EQ(1u, winners2.size());
    EXPECT_EQ("D1", winners2[0]->getName());
}


TEST(STVElectionRunIntegrationTest, ThreeSeatTransferScenario) {
    // 3 seats with transfers
    Candidate *a = new Candidate("A", "P");
    Candidate *b = new Candidate("B", "P");
    Candidate *c = new Candidate("C", "P");
    Candidate *d = new Candidate("D", "P");
    Candidate *e = new Candidate("E", "P");
    std::vector<Candidate *> candidates = { a, b, c, d, e };

    Ballot *p0 = new Ballot(0, {{0, "A"}, {1, "B"}, {2, "C"}, {3, "D"}, {4, "E"}});
    Ballot *p1 = new Ballot(1, {{0, "A"}, {1, "B"}, {2, "D"}, {3, "C"}, {4, "E"}});
    Ballot *p2 = new Ballot(2, {{0, "B"}, {1, "A"}, {2, "C"}, {3, "D"}, {4, "E"}});
    Ballot *p3 = new Ballot(3, {{0, "B"}, {1, "C"}, {2, "A"}, {3, "D"}, {4, "E"}});
    Ballot *p4 = new Ballot(4, {{0, "B"}, {1, "D"}, {2, "C"}, {3, "A"}, {4, "E"}});
    Ballot *p5 = new Ballot(5, {{0, "C"}, {1, "B"}, {2, "A"}, {3, "D"}, {4, "E"}});
    Ballot *p6 = new Ballot(6, {{0, "C"}, {1, "D"}, {2, "B"}, {3, "A"}, {4, "E"}});
    Ballot *p7 = new Ballot(7, {{0, "D"}, {1, "C"}, {2, "B"}, {3, "A"}, {4, "E"}});
    Ballot *p8 = new Ballot(8, {{0, "E"}, {1, "C"}, {2, "B"}, {3, "A"}, {4, "D"}});
    Ballot *p9 = new Ballot(9, {{0, "E"}, {1, "B"}, {2, "C"}, {3, "A"}, {4, "D"}});
    std::vector<Ballot *> ballots = { p0,p1,p2,p3,p4,p5,p6,p7,p8,p9 };

    TestableSTVElection election(3, false, ballots, candidates);
    election.run();
    auto winners = election.getWinners();
    ASSERT_EQ(3u, winners.size());
    std::vector<std::string> winnerNames;
    for (auto *w : winners) {
        winnerNames.push_back(w->getName());
    }
    EXPECT_TRUE(areVectorsEqualIgnoreOrder(winnerNames, {"A","B","C"}));
}


TEST(STVElectionRunIntegrationTest, TieBreakEarliestFirstVoteWins) {
    // 3 candidates with one ballot each; A gets the earliest first vote and should win
    Candidate *a = new Candidate("A", "P");
    Candidate *b = new Candidate("B", "P");
    Candidate *c = new Candidate("C", "P");
    std::vector<Candidate *> candidates = { a, b, c };

    Ballot *ba = new Ballot(0, {{0, "A"}});
    Ballot *bb = new Ballot(1, {{0, "B"}});
    Ballot *bc = new Ballot(2, {{0, "C"}});
    std::vector<Ballot *> ballots = { ba, bb, bc };

    TestableSTVElection election(1, false, ballots, candidates);
    election.run();
    auto winners = election.getWinners();
    ASSERT_EQ(1u, winners.size());
    
    // Because all start with one vote and breakTie eliminates the candidate with the
    // latest firstVote, the earliest-firstVote candidate (A) should remain and win.
    EXPECT_EQ("A", winners[0]->getName());
}

TEST(STVElectionRunIntegrationTest, SingleSeatClearWinner) {
    Candidate *c1 = new Candidate("Cand 1", "P1");
    Candidate *c2 = new Candidate("Cand 2", "P2");
    Candidate *c3 = new Candidate("Cand 3", "P3");
    std::vector<Candidate *> candidates = { c1, c2, c3 };

    Ballot *b0 = new Ballot(0, {{0, "Cand 1"}, {1, "Cand 2"}, {2, "Cand 3"}});
    Ballot *b1 = new Ballot(1, {{0, "Cand 1"}, {1, "Cand 3"}, {2, "Cand 2"}});
    Ballot *b2 = new Ballot(2, {{0, "Cand 2"}, {1, "Cand 1"}, {2, "Cand 3"}});
    std::vector<Ballot *> ballots = { b0, b1, b2 };

    TestableSTVElection election(1, false, ballots, candidates);
    election.run();

    auto winners = election.getWinners();
    ASSERT_EQ(1u, winners.size());
    EXPECT_EQ("Cand 1", winners[0]->getName());
}
