#include "BallotHandler.h"
#include "Ballot.h"
#include "Candidate.h"
#include "UserInterface.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <sstream>
#include <random>
#include <algorithm>
#include <limits>

BallotHandler::BallotHandler() {

}

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

void BallotHandler::readFile(const std::string& fileName, bool first) {
    std::ifstream infile(fileName);
    if (!infile.is_open()) {
        std::cerr << "Error: Could not open file '" << fileName << "'." << std::endl;
        return;
    }

    std::string line;
    bool skipExtraLine = false;
    if (first) {        
        // Read election type
        std::getline(infile, electionType);      // STV, PV, MV
        electionType = trim(electionType);
        std::transform(electionType.begin(), electionType.end(), electionType.begin(), ::tolower);

        std::getline(infile, line);
        numSeats = std::stoi(trim(line));
        
        std::getline(infile, line);
        if (electionType == "stv" && line.length() == 1) {
            // figure out whether to shuffle ballots
            isShuffleBallots = static_cast<char>(std::tolower(
                static_cast<unsigned char>(trim(line).at(0)))) == 'y';
            skipExtraLine = true;
            std::getline(infile, line);
        }

        // Parse candidates from the header line
        parseCandidates(line);

        if (candidates.empty()) {
            std::cerr << "Error: No candidates found in header of '" << fileName << "'." << std::endl;
            infile.close();
            return;
        }
    } else {
        // skip first 3 lines
        std::getline(infile, line);
        std::getline(infile, line);
        std::getline(infile, line);
        if (skipExtraLine) {
            std::getline(infile, line);
        }
    } 

    // Read ballot lines
    int localId = static_cast<int>(ballots.size() + thrownOutBallots.size()) + 1;
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

        Ballot* b;
        int numRanked = 0; // for STV

        if (electionType == "stv" || electionType == "pv") {

            std::map<int, std::string> candidateRanking;
            for (size_t i = 0; i < candidates.size(); ++i) {
                if (!fields[i].empty()) {
                    try {
                        int rank = std::stoi(fields[i]);
                        candidateRanking[rank] = candidates[i]->getName();
                        numRanked++;
                    } catch (const std::exception&) {
                        // Invalid rank, skip entry (assumed correct)
                    }
                }
            }

            b = new Ballot(localId++, candidateRanking);
        } else if (electionType == "mv") {

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

            b = new Ballot(localId++, votes);
        }
        // ensure if electionType is stv, then at least 60% of candidates ranked
        if (electionType != "stv" || static_cast<float>(numRanked) / candidates.size() >= .6) {
            ballots.push_back(b);
        } else {
            thrownOutBallots.push_back(b);
        }
    }

    infile.close();
}

void BallotHandler::handleFiles() {
    UserInterface &ui = UserInterface::getInstance();
    ui.getWindow();

    std::cout << "Waiting for CSV files to appear in uploads/ folder..." << std::endl;

    std::vector<std::string> uploadedFiles;

    while (uploadedFiles.empty()) {
        uploadedFiles.clear();
        for (const auto &entry : std::filesystem::directory_iterator("../web/uploads")) {
            if (entry.is_regular_file() && entry.path().extension() == ".csv") {
                uploadedFiles.push_back(entry.path().string());
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    bool first = true;
    for (const auto &filePath : uploadedFiles) {
        readFile(filePath, first);
        first = false;
    }

    if (isShuffleBallots) {
        ballots = shuffleBallots(ballots);
    }

    if (!thrownOutBallots.empty()) {
        thrownOutBallotsFile();
    }

    for (const auto &entry : std::filesystem::directory_iterator("../web/uploads")) {
        try {
            if (entry.path().extension() == ".csv") {
                std::filesystem::remove(entry.path());
            }
        } catch (const std::exception &e) {
            std::cerr << "Failed to delete " << entry.path() << ": " << e.what() << std::endl;
        }
    }
}

void BallotHandler::thrownOutBallotsFile() {
    // Get current date
    std::time_t now = std::time(nullptr);
    std::tm* local = std::localtime(&now);
    std::string date = std::to_string(local->tm_mon + 1) +
                       std::to_string(local->tm_mday) +
                       std::to_string(local->tm_year + 1900);

    std::ofstream outfile("STVBallotErrors" + date + ".csv");
    if (!outfile.is_open()) {
        std::cerr << "Error: Could not open STVBallotErrors.csv for writing." << std::endl;
        return;
    }
    for (auto* b : thrownOutBallots) {
        outfile << b->getId();
        for (const auto& [rank, name] : b->getCandidateRankings()) {
            outfile << "," << name;
        }
        outfile << std::endl;
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
    for (auto tb : thrownOutBallots) delete tb;
    for (auto c : candidates) delete c;
}