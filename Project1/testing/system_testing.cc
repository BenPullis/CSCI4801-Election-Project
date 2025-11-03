#include <gtest/gtest.h>
#include "AuditReport.h"
#include "Ballot.h"
#include "BallotHandler.h"
#include "Candidate.h"
#include "HelpWindow.h"
#include "PluralityElection.h"
#include "STVElection.h"
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

class TestableSTVElection : public STVElection {
    public:
        TestableSTVElection(int nSeats, bool sb, std::vector<Ballot *> b, std::vector<Candidate *> c, BallotHandler *bh)
            : STVElection(nSeats, sb, b, c, bh)
        {}
        std::vector<Candidate *> getWinners() { return getWinnersList(); }
        std::vector<Candidate *> getLosers() { return getLosersList(); }
};

class TestablePluralityElection : public PluralityElection {
    public:
        TestablePluralityElection(int nSeats, std::vector<Ballot *> b, std::vector<Candidate *> c)
            : PluralityElection(nSeats, b, c)
        {}
        std::vector<Candidate *> getWinners() { return getWinnersList(); }
};


// Full plurality system test hitting every component of the system
// > HelpWindow
// > BallotHandler (by extension Ballot and Candidate)
// > PluralityElection
// > AuditReport
TEST(PluralitySystemTest, PluralityRunThrough) {
    // Test out the Help Window.
    std::string fake_input_data = "1\nok\nexit\n";
    std::istringstream fake_input(fake_input_data);

    std::streambuf* old_cin = std::cin.rdbuf();
    std::streambuf* old_cout = std::cout.rdbuf();

    std::cin.rdbuf(fake_input.rdbuf());

    std::ostringstream captured_output;
    std::cout.rdbuf(captured_output.rdbuf());

    HelpWindow& help = HelpWindow::getInstance();
    help.getHelp();

    ASSERT_NO_FATAL_FAILURE(HelpWindow::getInstance());

    std::cin.rdbuf(old_cin);
    std::cout.rdbuf(old_cout);

    std::string output = captured_output.str();

    EXPECT_NE(output.find("The STV (Single Transferable Voting) election system allows candidates to be ranked"), std::string::npos) << "Expected STV help text.\n";

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

    // Run a Plurality Election based off of the previously created ballots and candidates.
    TestablePluralityElection election(1, ballots, candidates);

    election.run();

    std::vector<Candidate*> winners = election.getWinners();

    ASSERT_EQ(winners.size(), 1) << "Expected exactly one winner.";
    EXPECT_EQ(winners[0]->getName(), "Alice") << "Expected Alice to win with 2 votes.";

    // Check to see if audit report exists.
    EXPECT_TRUE(std::filesystem::exists("audit_report.txt")) << "Verify audit report was generated.";
    std::ifstream infile("audit_report.txt");
    ASSERT_TRUE(infile.is_open()) << "Unable to open audit_report.txt for reading.";

    std::string content((std::istreambuf_iterator<char>(infile)),
                         std::istreambuf_iterator<char>());
    infile.close();

    // Ensure correct contents written to audit report.
    EXPECT_NE(content.find("Election"), std::string::npos)
        << "Missing 'Election' header.";
    EXPECT_NE(content.find("Date of the election:"), std::string::npos)
        << "Missing 'Date of the election:' header.";
    EXPECT_NE(content.find("Elected Candidates: Alice"), std::string::npos)
        << "Missing the correct elected candidate";
}

// Full plurality system test WITH multiple candidates.
TEST(PluralitySystemTest, MultipleCandidatesPlurality) {
    // Test out the Help Window.
    std::string fake_input_data = "1\nok\nexit\n";
    std::istringstream fake_input(fake_input_data);

    std::streambuf* old_cin = std::cin.rdbuf();
    std::streambuf* old_cout = std::cout.rdbuf();

    std::cin.rdbuf(fake_input.rdbuf());

    std::ostringstream captured_output;
    std::cout.rdbuf(captured_output.rdbuf());

    HelpWindow& help = HelpWindow::getInstance();
    help.getHelp();

    ASSERT_NO_FATAL_FAILURE(HelpWindow::getInstance());

    std::cin.rdbuf(old_cin);
    std::cout.rdbuf(old_cout);

    std::string output = captured_output.str();

    EXPECT_NE(output.find("The STV (Single Transferable Voting) election system allows candidates to be ranked"), std::string::npos) << "Expected STV help text.\n";

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

    // Run a Plurality Election based off of the previously created ballots and candidates.
    TestablePluralityElection election(2, ballots, candidates);

    election.run();

    std::vector<Candidate*> winners = election.getWinners();

    ASSERT_EQ(winners.size(), 2) << "Expected exactly one winner.";
    EXPECT_EQ(winners[0]->getName(), "Alice") << "Expected Alice to win with 2 votes.";
    EXPECT_EQ(winners[1]->getName(), "Bob") << "Expected Alice to win with 1 vote.";


    // Check to see if audit report exists.
    EXPECT_TRUE(std::filesystem::exists("audit_report.txt")) << "Verify audit report was generated.";
    std::ifstream infile("audit_report.txt");
    ASSERT_TRUE(infile.is_open()) << "Unable to open audit_report.txt for reading.";

    std::string content((std::istreambuf_iterator<char>(infile)),
                         std::istreambuf_iterator<char>());
    infile.close();

    // Ensure correct contents written to audit report.
    EXPECT_NE(content.find("Election"), std::string::npos)
        << "Missing 'Election' header.";
    EXPECT_NE(content.find("Date of the election:"), std::string::npos)
        << "Missing 'Date of the election:' header.";
    EXPECT_NE(content.find("Elected Candidates: Alice, Bob"), std::string::npos)
        << "Missing the correct elected candidates";
}

// Full plurality system test WITH tied candidates.
TEST(PluralitySystemTest, PluralityTie) {
    // Test out the Help Window.
    std::string fake_input_data = "1\nok\nexit\n";
    std::istringstream fake_input(fake_input_data);

    std::streambuf* old_cin = std::cin.rdbuf();
    std::streambuf* old_cout = std::cout.rdbuf();

    std::cin.rdbuf(fake_input.rdbuf());

    std::ostringstream captured_output;
    std::cout.rdbuf(captured_output.rdbuf());

    HelpWindow& help = HelpWindow::getInstance();
    help.getHelp();

    ASSERT_NO_FATAL_FAILURE(HelpWindow::getInstance());

    std::cin.rdbuf(old_cin);
    std::cout.rdbuf(old_cout);

    std::string output = captured_output.str();

    EXPECT_NE(output.find("The STV (Single Transferable Voting) election system allows candidates to be ranked"), std::string::npos) << "Expected STV help text.\n";

    BallotHandler handler;

    // Simulate user entering file info via stdin
    auto cinbuf = std::cin.rdbuf();
    std::istringstream input("1\n../testing/ballots1.csv\n");
    std::cin.rdbuf(input.rdbuf());

    handler.handleFiles();

    std::cin.rdbuf(cinbuf);

    // Verify candidates
    auto candidates = handler.getCandidates();
    ASSERT_EQ(candidates.size(), 3) << "Expected 3 candidates from ballots1.csv header.";
    EXPECT_EQ(candidates[0]->getName(), "Alice");
    EXPECT_EQ(candidates[1]->getName(), "Bob");
    EXPECT_EQ(candidates[2]->getName(), "Charlie");

    // Verify ballots (2 from first file)
    auto ballots = handler.receiveBallots();
    EXPECT_EQ(ballots.size(), 2) << "Expected 2 total ballots.";

    // Run a Plurality Election based off of the previously created ballots and candidates.
    TestablePluralityElection election(1, ballots, candidates);

    election.run();

    std::vector<Candidate*> winners = election.getWinners();

    ASSERT_EQ(winners.size(), 1) << "Expected exactly one winner.";
    EXPECT_TRUE(winners[0]->getName() == "Alice" || winners[0]->getName() == "Bob") << "Expected Alice OR Bob to win with 1 vote.";

    // Check to see if audit report exists.
    EXPECT_TRUE(std::filesystem::exists("audit_report.txt")) << "Verify audit report was generated.";
    std::ifstream infile("audit_report.txt");
    ASSERT_TRUE(infile.is_open()) << "Unable to open audit_report.txt for reading.";

    std::string content((std::istreambuf_iterator<char>(infile)),
                         std::istreambuf_iterator<char>());
    infile.close();

    // Ensure correct contents written to audit report.
    EXPECT_NE(content.find("Election"), std::string::npos)
        << "Missing 'Election' header.";
    EXPECT_NE(content.find("Date of the election:"), std::string::npos)
        << "Missing 'Date of the election:' header.";
    EXPECT_FALSE(content.find("Elected Candidates: Alice") == std::string::npos && content.find("Elected Candidates: Bob") == std::string::npos)
        << "Missing the correct elected candidates";
}


// Full STV system test hitting every component of the system
// > HelpWindow
// > BallotHandler (by extension Ballot and Candidate)
// > STVElection
// > AuditReport
TEST(STVSystemTest, STVRunThrough) {
    // Test out the Help Window.
    std::string fake_input_data = "1\nok\nexit\n";
    std::istringstream fake_input(fake_input_data);

    std::streambuf* old_cin = std::cin.rdbuf();
    std::streambuf* old_cout = std::cout.rdbuf();

    std::cin.rdbuf(fake_input.rdbuf());

    std::ostringstream captured_output;
    std::cout.rdbuf(captured_output.rdbuf());

    HelpWindow& help = HelpWindow::getInstance();
    help.getHelp();

    ASSERT_NO_FATAL_FAILURE(HelpWindow::getInstance());

    std::cin.rdbuf(old_cin);
    std::cout.rdbuf(old_cout);

    std::string output = captured_output.str();

    EXPECT_NE(output.find("The STV (Single Transferable Voting) election system allows candidates to be ranked"), 
        std::string::npos) << "Expected STV help text.\n";

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

    // Run a Plurality Election based off of the previously created ballots and candidates.
    TestableSTVElection election(1, false, ballots, candidates, &handler);

    election.run();

    std::vector<Candidate*> winners = election.getWinners();
    std::vector<Candidate*> losers = election.getLosers();

    ASSERT_EQ(winners.size(), 1) << "Expected exactly one winner.";
    EXPECT_EQ(winners[0]->getName(), "Alice") << "Expected Alice to win with 2 votes.";

    ASSERT_EQ(losers.size(), 2) << "Expected exactly two losers.";
    EXPECT_EQ(losers[0]->getName(), "Bob") << "Expected Bob to be eliminated";
    EXPECT_EQ(losers[1]->getName(), "Charlie") << "Expected Charlie to be eliminated";

    // Check to see if audit report exists.
    EXPECT_TRUE(std::filesystem::exists("audit_report.txt")) << "Verify audit report was generated.";
    std::ifstream infile("audit_report.txt");
    ASSERT_TRUE(infile.is_open()) << "Unable to open audit_report.txt for reading.";

    std::string content((std::istreambuf_iterator<char>(infile)),
                         std::istreambuf_iterator<char>());
    infile.close();

    // Ensure correct contents written to audit report.
    EXPECT_NE(content.find("Election"), std::string::npos)
        << "Missing 'Election' header.";
    EXPECT_NE(content.find("Date of the election:"), std::string::npos)
        << "Missing 'Date of the election:' header.";
    EXPECT_NE(content.find("Ballot ID 1 preference at start of election: Alice"), std::string::npos)
        << "Missing Ballot ID 1 initial preference.";
    EXPECT_NE(content.find("Ballot ID 2 preference at start of election: Bob"), std::string::npos)
        << "Missing Ballot ID 2 initial preference.";
    EXPECT_NE(content.find("Ballot ID 3 preference at start of election: Alice"), std::string::npos)
        << "Missing Ballot ID 3 initial preference.";
    EXPECT_NE(content.find("Ballot ID 1 preference after pass #1: Alice"), std::string::npos)
        << "Missing Ballot ID 1 preference update.";
    EXPECT_NE(content.find("Ballot ID 2 preference after pass #1: Bob"), std::string::npos)
        << "Missing Ballot ID 2 preference update.";
    EXPECT_NE(content.find("Ballot ID 3 preference after pass #1: Alice"), std::string::npos)
        << "Missing Ballot ID 3 preference update.";
    EXPECT_NE(content.find("Elected Candidates: Alice"), std::string::npos)
        << "Missing the correct elected candidate";
    EXPECT_NE(content.find("Non-Elected Candidates: Bob, Charlie"), std::string::npos)
        << "Missing the correct non-elected candidates";
}

// Full STV system test WITH multiple candidates.
TEST(STVSystemTest, MultipleCandidatesSTV) {
    // Test out the Help Window.
    std::string fake_input_data = "1\nok\nexit\n";
    std::istringstream fake_input(fake_input_data);

    std::streambuf* old_cin = std::cin.rdbuf();
    std::streambuf* old_cout = std::cout.rdbuf();

    std::cin.rdbuf(fake_input.rdbuf());

    std::ostringstream captured_output;
    std::cout.rdbuf(captured_output.rdbuf());

    HelpWindow& help = HelpWindow::getInstance();
    help.getHelp();

    ASSERT_NO_FATAL_FAILURE(HelpWindow::getInstance());

    std::cin.rdbuf(old_cin);
    std::cout.rdbuf(old_cout);

    std::string output = captured_output.str();

    EXPECT_NE(output.find("The STV (Single Transferable Voting) election system allows candidates to be ranked"), 
        std::string::npos) << "Expected STV help text.\n";

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

    // Run a Plurality Election based off of the previously created ballots and candidates.
    TestableSTVElection election(2, false, ballots, candidates, &handler);

    election.run();

    std::vector<Candidate*> winners = election.getWinners();
    std::vector<Candidate*> losers = election.getLosers();

    ASSERT_EQ(winners.size(), 2) << "Expected two winners.";
    EXPECT_EQ(winners[0]->getName(), "Alice") << "Expected Alice to win.";
    EXPECT_EQ(winners[1]->getName(), "Bob") << "Expected Bob to win.";

    ASSERT_EQ(losers.size(), 1) << "Expected exactly one loser.";
    EXPECT_EQ(losers[0]->getName(), "Charlie") << "Expected Charlie to be eliminated";

    // Check to see if audit report exists.
    EXPECT_TRUE(std::filesystem::exists("audit_report.txt")) << "Verify audit report was generated.";
    std::ifstream infile("audit_report.txt");
    ASSERT_TRUE(infile.is_open()) << "Unable to open audit_report.txt for reading.";

    std::string content((std::istreambuf_iterator<char>(infile)),
                         std::istreambuf_iterator<char>());
    infile.close();

    // Ensure correct contents written to audit report.
    EXPECT_NE(content.find("Election"), std::string::npos)
        << "Missing 'Election' header.";
    EXPECT_NE(content.find("Date of the election:"), std::string::npos)
        << "Missing 'Date of the election:' header.";
    EXPECT_NE(content.find("Ballot ID 1 preference at start of election: Alice"), std::string::npos)
        << "Missing Ballot ID 1 initial preference.";
    EXPECT_NE(content.find("Ballot ID 2 preference at start of election: Bob"), std::string::npos)
        << "Missing Ballot ID 2 initial preference.";
    EXPECT_NE(content.find("Ballot ID 3 preference at start of election: Alice"), std::string::npos)
        << "Missing Ballot ID 3 initial preference.";
    EXPECT_NE(content.find("Ballot ID 1 preference after pass #1: Charlie"), std::string::npos)
        << "Missing Ballot ID 1 preference update.";
    EXPECT_NE(content.find("Ballot ID 2 preference after pass #1: Bob"), std::string::npos)
        << "Missing Ballot ID 2 preference update.";
    EXPECT_NE(content.find("Ballot ID 3 preference after pass #1: Charlie"), std::string::npos)
        << "Missing Ballot ID 3 preference update.";
    EXPECT_NE(content.find("Elected Candidates: Alice, Bob"), std::string::npos)
        << "Missing the correct elected candidate";
    EXPECT_NE(content.find("Non-Elected Candidates: Charlie"), std::string::npos)
        << "Missing the correct non-elected candidates";
}

// Full STV system test WITH tied candidates.
TEST(STVSystemTest, STVTie) {
    // Test out the Help Window.
    std::string fake_input_data = "1\nok\nexit\n";
    std::istringstream fake_input(fake_input_data);

    std::streambuf* old_cin = std::cin.rdbuf();
    std::streambuf* old_cout = std::cout.rdbuf();

    std::cin.rdbuf(fake_input.rdbuf());

    std::ostringstream captured_output;
    std::cout.rdbuf(captured_output.rdbuf());

    HelpWindow& help = HelpWindow::getInstance();
    help.getHelp();

    ASSERT_NO_FATAL_FAILURE(HelpWindow::getInstance());

    std::cin.rdbuf(old_cin);
    std::cout.rdbuf(old_cout);

    std::string output = captured_output.str();

    EXPECT_NE(output.find("The STV (Single Transferable Voting) election system allows candidates to be ranked"), 
        std::string::npos) << "Expected STV help text.\n";

    BallotHandler handler;

    // Simulate user entering file info via stdin
    auto cinbuf = std::cin.rdbuf();
    std::istringstream input("1\n../testing/ballots3.csv\n");
    std::cin.rdbuf(input.rdbuf());

    handler.handleFiles();

    std::cin.rdbuf(cinbuf);

    // Verify candidates
    auto candidates = handler.getCandidates();
    ASSERT_EQ(candidates.size(), 3) << "Expected 3 candidates from ballot3.csv header.";
    EXPECT_EQ(candidates[0]->getName(), "Alice");
    EXPECT_EQ(candidates[1]->getName(), "Bob");
    EXPECT_EQ(candidates[2]->getName(), "Charlie");

    // Verify ballots (3 from first file)
    auto ballots = handler.receiveBallots();
    EXPECT_EQ(ballots.size(), 3) << "Expected 3 total ballots.";

    // Run a Plurality Election based off of the previously created ballots and candidates.
    TestableSTVElection election(1, false, ballots, candidates, &handler);

    election.run();

    std::vector<Candidate*> winners = election.getWinners();

    ASSERT_EQ(winners.size(), 1) << "Expected exactly one winner.";
    EXPECT_TRUE(winners[0]->getName() == "Alice") << "Expected Alice to win with 1 vote.";

    // Check to see if audit report exists.
    EXPECT_TRUE(std::filesystem::exists("audit_report.txt")) << "Verify audit report was generated.";
    std::ifstream infile("audit_report.txt");
    ASSERT_TRUE(infile.is_open()) << "Unable to open audit_report.txt for reading.";

    std::string content((std::istreambuf_iterator<char>(infile)),
                         std::istreambuf_iterator<char>());
    infile.close();

    // Ensure correct contents written to audit report.
    EXPECT_NE(content.find("Election"), std::string::npos)
        << "Missing 'Election' header.";
    EXPECT_NE(content.find("Date of the election:"), std::string::npos)
        << "Missing 'Date of the election:' header.";
    EXPECT_NE(content.find("Ballot ID 1 preference at start of election: Alice"), std::string::npos)
        << "Missing Ballot ID 1 initial preference.";
    EXPECT_NE(content.find("Ballot ID 2 preference at start of election: Bob"), std::string::npos)
        << "Missing Ballot ID 2 initial preference.";
    EXPECT_NE(content.find("Ballot ID 3 preference at start of election: Charlie"), std::string::npos)
        << "Missing Ballot ID 3 initial preference.";
    EXPECT_NE(content.find("Ballot ID 1 preference after pass #1: Alice"), std::string::npos)
        << "Missing Ballot ID 1 preference update.";
    EXPECT_NE(content.find("Ballot ID 2 preference after pass #1: Bob"), std::string::npos)
        << "Missing Ballot ID 2 preference update.";
    EXPECT_NE(content.find("Ballot ID 3 preference after pass #1: "), std::string::npos)
        << "Missing Ballot ID 3 preference update.";
    EXPECT_NE(content.find("Elected Candidates: Alice"), std::string::npos)
        << "Missing the correct elected candidate";
    EXPECT_NE(content.find("Non-Elected Candidates: Charlie, Bob"), std::string::npos)
        << "Missing the correct non-elected candidates";
}
