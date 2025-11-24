#include "BallotHandler.h"
#include "Ballot.h"
#include "Candidate.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <random>
#include <algorithm>
#include <limits>

/**
 * @brief Default constructor for BallotHandler.
 *
 * Initializes a BallotHandler object with empty ballots and candidates.
 */
BallotHandler::BallotHandler() {

}

/**
 * @brief Trims whitespace from a string.
 *
 * Removes leading and trailing spaces, tabs, carriage returns, and newlines from the input string.
 *
 * @param str The string to trim.
 * @return The trimmed string.
 */
std::string BallotHandler::trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

void BallotHandler::parseCandidates(const std::string& line) {
    size_t pos = 0;

    while (pos < line.length()) {
        if (line[pos] != '[') { pos++; continue; }

        pos++; // skip '['
        size_t comma = line.find(',', pos);
        if (comma == std::string::npos) break;

        std::string name = trim(line.substr(pos, comma - pos));
        pos = comma + 1;

        size_t close = line.find(']', pos);
        if (close == std::string::npos) break;

        std::string party = trim(line.substr(pos, close - pos));
        pos = close + 1;

        if (pos < line.length() && line[pos] == ',') pos++;

        candidates.push_back(new Candidate(name, party));
    }
}

/**
 * @brief Reads ballot data from a file.
 *
 * If it's the first file, parses the header to extract candidates. Then reads and parses
 * ballot lines, creating Ballot objects and adding them to the collection.
 *
 * @param fileName The name of the file to read.
 * @param first Indicates if this is the first file (to parse candidates from header).
 */
void BallotHandler::readFile(const std::string& fileName, bool first) {
    std::ifstream infile(fileName);
    if (!infile.is_open()) {
        std::cerr << "Error: Could not open file '" << fileName << "'." << std::endl;
        return;
    }

    std::string line;
    if (first) {
        if (!std::getline(infile, line)) {
            std::cerr << "Error: Empty or invalid file '" << fileName << "'." << std::endl;
            infile.close();
            return;
        }
        
        // Read election type
        std::getline(infile, electionType);      // STV, PV, MV
        electionType = trim(electionType);

        std::getline(infile, line);              // number of seats (ignored right now)


        // Parse candidates from the header line
        std::getline(infile, line);
        parseCandidates(line);

        if (candidates.empty()) {
            std::cerr << "Error: No candidates found in header of '" << fileName << "'." << std::endl;
            infile.close();
            return;
        }
    }  else {
        // Skip header lines
        std::getline(infile, line);
        std::getline(infile, line);
        std::getline(infile, line);
    }

    // Read ballot lines
    int localId = static_cast<int>(ballots.size()) + 1;
    while (std::getline(infile, line)) {
        line = trim(line);
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string token;
        std::vector<std::string> fields;

        while (std::getline(ss, token, ',')) {
            fields.push_back(trim(token));
        }
        fields.resize(candidates.size(), "");

        if (electionType == "STV" || electionType == "PV") {

            std::map<int, std::string> ranking;

            for (size_t i = 0; i < fields.size(); i++) {
                if (!fields[i].empty()) {
                    try {
                        int rank = std::stoi(fields[i]);
                        ranking[rank] = candidates[i]->getName();
                    } catch (...) {
                        // Invalid rank, ignore
                    }
                }
            }

            Ballot* b = new Ballot(localId++, ranking);
            ballots.push_back(b);
        } else if (electionType == "MV") {

            std::vector<int> votes(candidates.size(), 0);

            for (size_t i = 0; i < fields.size(); i++) {
                if (fields[i] == "1") {
                    votes[i] = 1;
                }
            }

            // Blank ballot handling
            bool hasVote = false;
            for (int v : votes) if (v == 1) hasVote = true;
            if (!hasVote) continue; // drop empty MV ballot

            Ballot* b = new Ballot(localId++, votes);

            ballots.push_back(b);
        }
    }

    infile.close();
}

/**
 * @brief Handles input for multiple ballot files.
 *
 * Prompts the user for the number of files and their names, then reads each file using readFile.
 */
void BallotHandler::handleFiles() {
    int numFiles;
    std::cout << "Enter the number of ballot files: ";
    std::cin >> numFiles;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    bool first = true;
    for (int i = 0; i < numFiles; ++i) {
        std::string fileName;
        std::cout << "Enter file name " << (i + 1) << ": ";
        std::getline(std::cin, fileName);
        fileName = trim(fileName);
        if (fileName.empty()) continue;

        readFile(fileName, first);
        first = false;
    }
}

/**
 * @brief Retrieves the list of ballots.
 *
 * @return A vector of pointers to Ballot objects.
 */
std::vector<Ballot*> BallotHandler::receiveBallots() {
    return ballots;
}

/**
 * @brief Shuffles the list of ballots.
 *
 * Creates a copy of the ballots vector and shuffles it using a random device.
 *
 * @param ballots The vector of ballots to shuffle.
 * @return A shuffled vector of Ballot pointers.
 */
std::vector<Ballot*> BallotHandler::shuffleBallots(const std::vector<Ballot*>& ballots) {
    std::vector<Ballot*> shuffled = ballots;
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(shuffled.begin(), shuffled.end(), g);
    return shuffled;
}

/**
 * @brief Retrieves the list of candidates.
 *
 * @return A constant vector of pointers to Candidate objects.
 */
std::vector<Candidate*> BallotHandler::getCandidates() const {
    return candidates;
}

/**
 * @brief Destructor for BallotHandler.
 *
 * Cleans up dynamically allocated Ballot and Candidate objects.
 */
BallotHandler::~BallotHandler() {
    for (auto b : ballots) delete b;
    for (auto c : candidates) delete c;
}