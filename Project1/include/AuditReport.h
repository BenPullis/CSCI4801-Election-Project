#ifndef AUDITREPORT_H
#define AUDITREPORT_H

#include <vector>
#include <string>

class AuditReport {
public:
    AuditReport();
    void generateReport(const std::vector<std::string>& details);
};

#endif // AUDITREPORT_H