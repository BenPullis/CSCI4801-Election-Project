#ifndef CANDIDATE_H_
#define CANDIDATE_H_

#include <string>

class Candidate{

    private:

        std::string candidateName;
        std::string candidateParty;
        int firstVote;


    public:

        Candidate(const std::string& name, const std::string& party) : candidateName(name), candidateParty(party), firstVote(-1){}

        std::string getName() { return candidateName; }

        std::string getParty() { return candidateParty; }

        int getFirstVote() { return firstVote; }

        void setFirstVote(int voteTime) { firstVote = voteTime; }

};

#endif /* CANDIDATE_H_ */