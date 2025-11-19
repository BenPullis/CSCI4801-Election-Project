#include "AuditReport.h"

#include <fstream>
#include <iostream>
#include <ctime>
#include <iomanip>

/**
 * @brief Default constructor for AuditReport.
 *
 * Initializes an AuditReport object with default settings.
 */
AuditReport::AuditReport() {
    // Default constructor
}

/**
 * @brief Generates an audit report file with the provided details.
 *
 * This method creates a file named "audit_report.txt" and writes a header
 * including the election title and current date, followed by the provided
 * details. If the file cannot be opened, an error message is printed to stderr.
 *
 * @param details A vector of strings containing the details to include in the report.
 */
void AuditReport::generateReport(const std::vector<std::string>& details) {
    std::ofstream outfile("audit_report.txt");
    if (!outfile.is_open()) {
        std::cerr << "Error: Could not open audit_report.txt for writing." << std::endl;
        return;
    }

    // Get current date
    std::time_t now = std::time(nullptr);
    std::tm* local = std::localtime(&now);
    std::string date = std::to_string(local->tm_year + 1900) + "-" +
                       std::to_string(local->tm_mon + 1) + "-" +
                       std::to_string(local->tm_mday);

    // Write header
    outfile << "Election" << std::endl;
    outfile << "Date of the election: " << date << std::endl;
    outfile << std::endl;

    for (const auto& detail : details) {
        outfile << detail << std::endl;
    }

    outfile.close();
    std::cout << "Audit report generated in audit_report.txt" << std::endl;
}