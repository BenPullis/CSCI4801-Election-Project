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

        electionType = trim(line);
        // convert electionType to all lower case
        std::transform(electionType.begin(), electionType.end(), electionType.begin(), ::tolower);

        std::getline(infile, line);
        numSeats = std::stoi(trim(line));

        std::getline(infile, line);
        // Parse candidates
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
    } else {
        // skip first 3 lines
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
        std::vector<std::string> ranks;
        while (std::getline(ss, token, ',')) {
            ranks.push_back(trim(token));
        }

        // Ensure we consider empty trailing fields 
        if (ranks.size() < candidates.size()) {
            ranks.resize(candidates.size(), "");
        }

        std::map<int, std::string> candidateRanking;
        int numRanked = 0;
        for (size_t i = 0; i < candidates.size(); ++i) {
            if (!ranks[i].empty()) {
                try {
                    int rank = std::stoi(ranks[i]);
                    candidateRanking[rank] = candidates[i]->getName();
                    numRanked++;
                } catch (const std::exception&) {
                    // Invalid rank, skip entry (assumed correct)
                }
            }
        }

        // ensure if electionType is stv, then at least 60% of candidates ranked
        if (electionType != "stv" || static_cast<float>(numRanked) / candidates.size() >= .6) {
            Ballot* b = new Ballot(localId++, candidateRanking);
            ballots.push_back(b);
        } else if (electionType == "stv" && static_cast<float>(numRanked) / candidates.size() < .6) {
            Ballot* b = new Ballot(localId++, candidateRanking);
            thrownOutBallots.push_back(b);
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

    if (!thrownOutBallots.empty()) {
        thrownOutBallotsFile();
    }

    for (const auto &entry : std::filesystem::directory_iterator("../web/uploads")) {
        try {
            std::filesystem::remove(entry.path());
        } catch (const std::exception &e) {
            std::cerr << "Failed to delete " << entry.path() << ": " << e.what() << std::endl;
        }
    }
}

/**
 * @brief Creates a new file containing the thrown out ballots.
 *
 * @return N/A
 */
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
    for (auto tb : thrownOutBallots) delete tb;
    for (auto c : candidates) delete c;
}