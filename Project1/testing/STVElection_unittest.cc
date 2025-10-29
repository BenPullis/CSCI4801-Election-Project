#include <gtest/gtest.h>
#include "STVElection.h"

class STVElectionTest : public ::testing::Test {
    public:
        void SetUp() {
            cand1 = new Candidate("Cand 1", "Party 1");
            cand2 = new Candidate("Cand 2", "Party 2");
            cand3 = new Candidate("Cand 3", "Party 1");
            cand4 = new Candidate("Cand 4", "Party 3");
            cand5 = new Candidate("Cand 5", "Party 2");
            candList = { cand1, cand2, cand3, cand4, cand5 };
        }

        void TearDown() {
            for (Candidate *cand : candList) {
                delete cand;
            }
        }

    protected:
        Candidate *cand1;
        Candidate *cand2;
        Candidate *cand3;
        Candidate *cand4;
        Candidate *cand5;
        std::vector<Candidate *> candList;
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


TEST_F(STVElectionTest, GetTiedCandidatesTest) {
    std::unordered_map<std::string, int> candidatePiles = {{ "Cand 1", 1 }};
    std::vector<Candidate *> candidates = { cand1 };
    std::vector<Candidate *> output = getTiedCandidates(candidatePiles, candidates);
    EXPECT_EQ(candidates, output);

    // Wrong pointer
    std::vector<Candidate *> invalidCandidatePointer = { new Candidate("Cand 1", "DFL") };
    ASSERT_NE(output, invalidCandidatePointer);

    candidatePiles = {
        { "Cand 1", 1 },
        { "Cand 2", 1 },
        { "Cand 3", 1 },
        { "Cand 4", 1 },
        { "Cand 5", 1 },
    };

    output = getTiedCandidates(candidatePiles, candList);

    EXPECT_TRUE(areVectorsEqualIgnoreOrder(candList, output));
}