#ifndef BALLOT_H_
#define BALLOT_H_

#include <map>
#include <string>

/**
 * @class Ballot
 * @brief Represents the ballot of a single voter, including their ranked candidate preferences.
 */
class Ballot {

private:

    int id;
    std::string preference;
    std::map<std::string, int> candidateRankings;

public:

    /**
     * @brief Constructor for a Ballot with an ID and candidate rankings.
     * @param newId used to initialize the ballot ID.
     * @param rankings A map associating candidate names with their ranking values.
     * @note The initial preference is set to 0.
     */
    Ballot(int newId, std::map<std::string, int> rankings)
        : id(newId), preference(0), candidateRankings(rankings) {}

    /**
     * @brief Retrieves the unique ballot ID.
     * @return The ID of this ballot.
     */
    int getId() { return id; }

    /**
     * @brief Retrieves the current preference value.
     * @return The current preference of this ballot.
     */
    int getPreference() { return preference; }

    /**
     * @brief Updates the current preference value.
     * @param newPreference which is set as preference.
     * @note The new preference should always be one more than the previous value.
     */
    void setPreference(int newPreference) { preference = newPreference; }

    /**
     * @brief Retrieves the candidate ranking map.
     * @return A map associating candidate names with their ranking values.
     */
    std::map<std::string, int> getCandidateRankings() { return candidateRankings; }
};

#endif /* BALLOT_H_ */