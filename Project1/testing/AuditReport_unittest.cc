#include <gtest/gtest.h>
#include "AuditReport.h"
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>

/**
 * @brief Test suite for AuditReport class.
 */
class AuditReportTest : public ::testing::Test {
protected:
    std::string filename = "audit_report.txt";

    void TearDown() override {
        if (std::filesystem::exists(filename)) {
            std::filesystem::remove(filename);
        }
    }
};

/**
 * @test Ensure generateReport() creates the audit report file.
 */
TEST_F(AuditReportTest, CreatesAuditReportFile) {
    AuditReport report;
    std::vector<std::string> details = {
        "Election Type: STV",
        "Number of Candidates: 3",
        "Number of Seats: 1",
        "Elected Candidates: Alice",
        "Non-Elected Candidates: Bob, Charlie"
    };

    report.generateReport(details);

    EXPECT_TRUE(std::filesystem::exists(filename))
        << "audit_report.txt should be created after generateReport() is called.";
}

/**
 * @test Ensure report file contains expected headers and details.
 */
TEST_F(AuditReportTest, ContainsExpectedHeadersAndDetails) {
    AuditReport report;
    std::vector<std::string> details = {
        "Election Type: Plurality",
        "Number of Candidates: 4",
        "Number of Seats: 2",
        "Elected Candidates: Alice, Bob",
        "Non-Elected Candidates: Carol, Dave"
    };

    report.generateReport(details);

    ASSERT_TRUE(std::filesystem::exists(filename))
        << "audit_report.txt should exist after report generation.";

    std::ifstream infile(filename);
    ASSERT_TRUE(infile.is_open()) << "Unable to open audit_report.txt for reading.";

    std::string content((std::istreambuf_iterator<char>(infile)),
                         std::istreambuf_iterator<char>());
    infile.close();

    // Verify header
    EXPECT_NE(content.find("Election"), std::string::npos)
        << "Missing 'Election' header.";
    EXPECT_NE(content.find("Date of the election:"), std::string::npos)
        << "Missing 'Date of the election:' header.";

    // Verify each provided detail appears in file
    for (const auto& detail : details) {
        EXPECT_NE(content.find(detail), std::string::npos)
            << "Detail missing in report: " << detail;
    }
}

/**
 * @test Ensure generateReport() handles empty details gracefully.
 */
TEST_F(AuditReportTest, HandlesEmptyDetailsGracefully) {
    AuditReport report;
    std::vector<std::string> emptyDetails;

    report.generateReport(emptyDetails);

    ASSERT_TRUE(std::filesystem::exists(filename))
        << "audit_report.txt should still be created even with empty details.";

    std::ifstream infile(filename);
    std::string firstLine;
    std::getline(infile, firstLine);
    infile.close();

    EXPECT_EQ(firstLine, "Election")
        << "First line should always contain 'Election'.";
}

/**
 * @test Ensure repeated calls overwrite the previous report file.
 */
TEST_F(AuditReportTest, OverwritesPreviousFile) {
    AuditReport report;
    std::vector<std::string> details1 = {"Election Type: STV"};
    std::vector<std::string> details2 = {"Election Type: Plurality"};

    // Generate first report
    report.generateReport(details1);

    // Generate second report (should overwrite)
    report.generateReport(details2);

    std::ifstream infile(filename);
    std::string content((std::istreambuf_iterator<char>(infile)),
                         std::istreambuf_iterator<char>());
    infile.close();

    EXPECT_NE(content.find("Election Type: Plurality"), std::string::npos)
        << "Second call should overwrite the first report.";
    EXPECT_EQ(content.find("Election Type: STV"), std::string::npos)
        << "Old content should not remain after overwrite.";
}
