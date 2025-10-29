#ifndef BALLOTHANDLER_H
#define BALLOTHANDLER_H

#include <vector>
#include <string>
#include <map>

class Ballot;
class Candidate;

class BallotHandler {
private:
    std::vector<Ballot> ballots;
    std::vector<Candidate> candidates;
    void readFile(const std::string& fileName, bool first);
    std::string trim(const std::string& str);

public:
    void handleFiles();
    std::vector<Ballot> recieveBallots();
    std::vector<Ballot> shuffleBallots(const std::vector<Ballot>& ballots);
    std::vector<Candidate> getCandidates() const;
};

#endif // BALLOTHANDLER_H
