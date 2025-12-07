#ifndef BALLOTHANDLER_H
#define BALLOTHANDLER_H

#include <vector>
#include <string>
#include <map>

/**
 * @class BallotHandler
 * @brief A class for handling ballots in an election system.
 *
 * This class manages reading ballots from files, parsing candidates, storing ballots,
 * and providing functionality to shuffle and retrieve ballots and candidates.
 */

class Ballot;
class Candidate;

class BallotHandler {
private:
    std::vector<Ballot*> ballots;
    std::vector<Ballot*> thrownOutBallots;
    std::vector<Candidate*> candidates;
    std::string electionType;
    int numSeats;

    void parseCandidates(const std::string& line);
    bool isShuffleBallots = false;

    /**
     * @brief Parses candidates from a csv file.
     *
     * Helper function to extract candidates for readFile.
     *
     * @param str The line with the candidates and parties.
     */
    void parseCandidates(const std::string& line);

    /**
     * @brief Reads ballot data from a file.
     *
     * If it's the first file, parses the header to extract candidates. Then reads and parses
     * ballot lines, creating Ballot objects and adding them to the collection.
     *
     * @param fileName The name of the file to read.
     * @param first Indicates if this is the first file (to parse candidates from header).
     */
    void readFile(const std::string& fileName, bool first);

    /**
     * @brief Trims whitespace from a string.
     *
     * Removes leading and trailing spaces, tabs, carriage returns, and newlines from the input string.
     *
     * @param str The string to trim.
     * @return The trimmed string.
     */
    std::string trim(const std::string& str);

public:
    /**
     * @brief Default constructor for BallotHandler.
     *
     * Initializes a BallotHandler object with empty ballots and candidates.
     */
    BallotHandler();

    /**
     * @brief Destructor for BallotHandler.
     *
     * Cleans up dynamically allocated Ballot and Candidate objects.
     */
    ~BallotHandler();
    
    /**
     * @brief Handles input for multiple ballot files.
     *
     * Prompts the user for the number of files and their names, then reads each file using readFile.
     */
    void handleFiles();

    /**
     * @brief Retrieves the list of ballots.
     *
     * @return A vector of pointers to Ballot objects.
     */
    std::vector<Ballot*> receiveBallots();

    /**
     * @brief Shuffles the list of ballots.
     *
     * Creates a copy of the ballots vector and shuffles it using a random device.
     *
     * @param ballots The vector of ballots to shuffle.
     * @return A shuffled vector of Ballot pointers.
     */
    std::vector<Ballot*> shuffleBallots(const std::vector<Ballot*>& ballots);

    /**
     * @brief Retrieves the list of candidates.
     *
     * @return A constant vector of pointers to Candidate objects.
     */
    std::vector<Candidate*> getCandidates() const;

    /**
     * @brief Retrieves the election type.
     * 
     * @return The election type as a string.
     */
    std::string getElectionType() const { return electionType; }

    /**
     * @brief Retrieves the number of seats.
     * 
     * @return The number of seats as an integer.
     */
    int getNumSeats() const { return numSeats; }

    /**
     * @brief Creates a new file containing the thrown out ballots.
     *
     * @return N/A
     */
    void thrownOutBallotsFile();
};

#endif // BALLOTHANDLER_H