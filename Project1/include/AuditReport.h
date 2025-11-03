#ifndef AUDITREPORT_H
#define AUDITREPORT_H

#include <vector>
#include <string>

/**
 * @class AuditReport
 * @brief A class for generating audit reports for elections.
 *
 * This class provides functionality to create an audit report file containing
 * election details, including a header with the current date.
 */

class AuditReport {
public:
    /**
     * @brief Default constructor for AuditReport.
     *
     * Initializes an AuditReport object with default settings.
     */
    AuditReport();

    /**
     * @brief Generates an audit report file with the provided details.
     *
     * This method creates a file named "audit_report.txt" and writes a header
     * including the election title and current date, followed by the provided
     * details. If the file cannot be opened, an error message is printed to stderr.
     *
     * @param details A vector of strings containing the details to include in the report.
     */
    void generateReport(const std::vector<std::string>& details);
};

#endif // AUDITREPORT_H