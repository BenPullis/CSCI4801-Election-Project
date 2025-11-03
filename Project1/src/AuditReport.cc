#include "AuditReport.h"

#include <fstream>
#include <iostream>
#include <ctime>
#include <iomanip>

/*This report should show the ballots that
were assigned to a candidate as the election progressed. The report does not print to the screen, but should be
run to a text file. All pertinent election data must be included in the report. At the top of the report, include:
o SRS Election
o The date of the election.
o The candidates’ names and parties.
o The total number of candidates.
o The number of seats to be filled by the election.
o The Elected list, in the order in which the candidates were added to the list.
o The Non-Elected list, in the order in which candidates were added to the list.
Be sure to show the order of the ballots after the shuffle if the shuffle was turned on. I recommend giving
each ballot a unique ID and using this ID to track ballots after each pass of the algorithm. We want to
know who had what ballots after each distribution round.
*/

AuditReport::AuditReport() {
    // Default constructor
}

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