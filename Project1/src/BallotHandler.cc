#include "BallotHandler.h"
#include "Ballot.h"
#include "Candidate.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <random>
#include <algorithm>
#include <limits>

std::string BallotHandler::trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

void BallotHandler::readFile(const std::string& fileName, bool first) {
    std::ifstream infile(fileName);
    if (!infile.is_open()) {
        std::cerr << "Error: Could not open file '" << fileName << "'." << std::endl;
        return;
    }

    std::string line;
    if (!std::getline(infile, line)) {
        std::cerr << "Error: Empty or invalid file '" << fileName << "'." << std::endl;
        infile.close();
        return;
    }

    if (first) {
        // Parse candidates from the header line
        size_t pos = 0;
        while (pos < line.length()) {
            if (line[pos] != '[') {
                pos++;
                continue;
            }
            pos++;  // Skip '['
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
        if (candidates.empty()) {
            std::cerr << "Error: No candidates found in header of '" << fileName << "'." << std::endl;
            infile.close();
            return;
        }
    }  // For non-first files, skip header (assume consistent)

    // Read ballot lines
    int localId = static_cast<int>(ballots.size()) + 1;
    while (std::getline(infile, line)) {
        line = trim(line);
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string token;
        std::vector<std::string> ranks;
        while (std::getline(ss, token, ',')) {
            ranks.push_back(trim(token));
        }

        // Assume correct number of fields (as per requirements)
        if (ranks.size() != candidates.size()) continue;

        std::map<int, std::string> candidateRanking;
        for (size_t i = 0; i < candidates.size(); ++i) {
            if (!ranks[i].empty()) {
                try {
                    int rank = std::stoi(ranks[i]);
                    candidateRanking[rank] = candidates[i]->getName();
                } catch (const std::exception&) {
                    // Invalid rank, skip entry (assumed correct)
                }
            }
        }

        Ballot* b = new Ballot(localId++, candidateRanking);
        ballots.push_back(b);
    }

    infile.close();
}

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

std::vector<Ballot*> BallotHandler::receiveBallots() {
    return ballots;
}

std::vector<Ballot*> BallotHandler::shuffleBallots(const std::vector<Ballot*>& ballots) {
    std::vector<Ballot*> shuffled = ballots;
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(shuffled.begin(), shuffled.end(), g);
    return shuffled;
}

std::vector<Candidate*> BallotHandler::getCandidates() const {
    return candidates;
}

BallotHandler::~BallotHandler() {
    for (auto b : ballots) delete b;
    for (auto c : candidates) delete c;
}