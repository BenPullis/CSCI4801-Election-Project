#ifndef CANDIDATE_H_
#define CANDIDATE_H_

#include <string>

/**
 * @class Candidate
 * @brief Represents a single candidate in the election, including their name, party, and first vote timestamp.
 */
class Candidate {

private:

    std::string candidateName;  
    std::string candidateParty; 
    int firstVote;             

public:

    /**
     * @brief Constructs a Candidate with a name and party.
     * @param name The candidate's name.
     * @param party The candidate's political party.
     * @note FirstVote is initialized to -1, meaning the candidate has not yet received their first vote.
     */
    Candidate(const std::string& name, const std::string& party) : candidateName(name), candidateParty(party), firstVote(-1) {}

    //Empty constructor for testing purposes.
    Candidate(){}

    /**
     * @brief Retrieves the candidate's name.
     * @return The candidate's name.
     */
    std::string getName() { return candidateName; }

    /**
     * @brief Retrieves the candidate's political party.
     * @return The candidate's party.
     */
    std::string getParty() { return candidateParty; }

    /**
     * @brief Retrieves when the candidate received their first vote.
     * @return The timestamp of the first vote.
     * @note A smaller firstVote value represents an earlier vote time.
     */
    int getFirstVote() { return firstVote; }

    /**
     * @brief Sets the first vote time for candidate.
     * @param voteTime The int value to assign to firstVote.
     */
    void setFirstVote(int voteTime) { firstVote = voteTime; }
};

#endif /* CANDIDATE_H_ */