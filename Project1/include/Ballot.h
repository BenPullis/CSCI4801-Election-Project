#ifndef BALLOT_H_
#define BALLOT_H_

#include <map>
#include <string>

class Ballot{

    private:
        
        int id;
        int preference;
        std::map<std::string, int> candidateRankings;


    public:

        Ballot(int newId, std::map<std::string, int> rankings) : id(newId), preference(0), candidateRankings(rankings) {}

        int getId() { return id; }

        int getPreference() { return preference; }

        //should always be increasing the current preference by 1
        void setPreference(int newPreference){ preference = newPreference; }

        std::map<std::string, int> getCandidateRankings() { return candidateRankings; }

};

#endif /* BALLOT_H_ */