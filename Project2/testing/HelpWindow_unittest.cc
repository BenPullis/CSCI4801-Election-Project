#include "gtest/gtest.h"
#include "HelpWindow.h"
#include <sstream>

// Since this is a static method that returns a reference to a Singleton, check to see if two getInstance
// calls return the same thing. Also check that no fatal errors occur on the getInstance call.
// Not much to be tested with this method in particular.
TEST(GetInstanceUnitTests, InstanceProperlyRetrieved){
    ASSERT_NO_FATAL_FAILURE(HelpWindow::getInstance());

    HelpWindow& help1 = HelpWindow::getInstance();
    HelpWindow& help2 = HelpWindow::getInstance();

    EXPECT_EQ(&help1, &help2);
}

// Test that asks for STV help on the help menu.
TEST(GetHelpUnitTests, STVCorrectPrint) {
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

    EXPECT_NE(output.find("The Single Transferable Voting (STV) election system allows candidates to be ranked"), std::string::npos) << "Expected STV help text.\n";
}

// Test that ensures you can access multiple help pages on the help menu.
TEST(GetHelpUnitTests, MultipleCorrectPrint) {
    std::string fake_input_data = "1\nok\n2\nexit\n";
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

    EXPECT_NE(output.find("The Single Transferable Voting (STV) election system allows candidates to be ranked"), std::string::npos) << "Expected STV help text.\n";
    EXPECT_NE(output.find("The Plurality election system allows votes to select a single candidates, and the candidate(s) with the most votes win."), std::string::npos) << "Expected Plurality help text.\n";
}

// Test that ensures you can exit right away.
TEST(GetHelpUnitTests, ExitTest) {
    std::string fake_input_data = "exit\n";
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

    EXPECT_EQ(output.find("The Single Transferable Voting (STV) election system allows candidates to be ranked"), std::string::npos) << "Did not expect STV Help text.\n" << output;
    EXPECT_EQ(output.find("The Plurality election system allows votes to select a single candidates, and the candidate(s) with the most votes win."), std::string::npos) << "Did not expect Plurality Help text.\n";

}