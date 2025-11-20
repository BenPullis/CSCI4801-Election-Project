#include "MVElection.h"

MVElection::MVElection(int nSeats, std::vector<Ballot *>b, std::vector<Candidate *> c){
    numSeats = nSeats;
    ballots = b;
    candidates = c;
    reportGenerator = new AuditReport();
}

MVElection::~MVElection(){
    delete reportGenerator;
}