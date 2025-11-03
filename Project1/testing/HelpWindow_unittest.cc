#include "gtest/gtest.h"
#include "HelpWindow.h"
#include <sstream>

TEST(GetInstanceUnitTests, InstanceProperlyRetrieved){
    // HelpWindow &help = HelpWindow::getInstance();
    ASSERT_NO_FATAL_FAILURE(HelpWindow::getInstance());
}

TEST(GetHelpUnitTests, correctPrint){
    std::string input_string = "1\n";
    std::istringstream iss(input_string);
    std::streambuf* old_cin = std::cin.rdbuf(iss.rdbuf()); 

    std::stringstream ss;
    std::streambuf* old_cout = std::cout.rdbuf();
    std::cout.rdbuf(ss.rdbuf());

    HelpWindow &help = HelpWindow::getInstance();

    help.getHelp();

    std::cout.rdbuf(old_cout);

    EXPECT_EQ("Hello, Google Test!\n", ss.str());

    std::cin.rdbuf(old_cin);
}

// TEST(GetHelpUnitTests, correctPrint){
    
// }