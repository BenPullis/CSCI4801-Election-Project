#include "gtest/gtest.h"
#include "HelpWindow.h"
#include <sstream>

TEST(GetInstanceUnitTests, InstanceProperlyRetrieved){
    // HelpWindow &help = HelpWindow::getInstance();
    ASSERT_NO_FATAL_FAILURE(HelpWindow::getInstance());
}

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

    EXPECT_NE(output.find("The STV (Single Transferable Voting) election system allows candidates to be ranked"), std::string::npos) << "Expected STV help text.\n";
}

// TEST(GetHelpUnitTests, correctPrint){
    
// }