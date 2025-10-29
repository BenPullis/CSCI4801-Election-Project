#include "STVElection.h"
#include "PluralityElection.h"
#include "IElection.h"
#include "HelpWindow.h"
#include <string>
#include <iostream>

using namespace std;

int main(){
    
    HelpWindow help;

    cout << "------------------------" << endl;
    cout << "Time To Run An Election!" << endl;
    cout << "------------------------\n\n" << endl;

    string input;
    int numSeats;
    cout << "First, Enter The Number Of Seats For The Election Or 'Help' for Further Help: ";

    while(true){
        cin >> input;
        if(input == "Help" || input == "help"){
            help.getHelp();
            cout << "Enter The Number Of Seats For The Election Or 'Help' for Further Help: ";
        }
        else{
            try{
                numSeats = stoi(input);

                if(numSeats > 0){
                    break;
                }
                else{
                    cout << "Number Of Seats Must Be Greater Than 0, Please Try Again: ";
                }
            }
            catch(invalid_argument){
                cout << "Invalid Input, Please Try Again: ";
            }
            catch(out_of_range){
                cout << "Input Too Large, Please Try Again: ";
            }
        }
    }

    cout << "Number of seats set to: " << numSeats << endl;


    cout << "Now Choose The Type Of Election You Would Like, 'STV', 'Plurality', Or 'Help' for Further Help: ";
    IElection *election;

    while(true){
        cin >> input;
        if(input == "Help" || input == "help"){
            help.getHelp();
            cout << "Choose The Type Of Election You Would Like, 'STV', 'Plurality', Or 'Help' for Further Help: ";
        }
        else{
            if(input == "STV" || input == "stv" || input == "Stv"){
                election = new STVElection(numSeats);
                break;
            }
            else if(input == "PLURALITY" || input == "plurality" || input == "Plurality"){
                election = new PluralityElection(numSeats);
                break;
            }
            else{
                cout << "Invalid Input, Please Try Again: ";
            }
        }
    }

    election->run();

    delete(election);

    return 0;
}