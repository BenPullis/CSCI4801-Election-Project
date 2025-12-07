# repo-Team11
Adam Douiri, Joe Anderson, Ben Pullis, Quang Pham

To Compile:
Go to Project1/src and run "make"

To Run:
Go to Project1/src and run "make run"

To Enter CSV Files:
Enter any key when prompted and then use the GUI to select the CSV files to use

To Run Tests:
Go to Project1/src and run "make test"

Note about STV elections:
The BallotHandler class expects the third line in the STV header files to be "Y" or "N"
to determine whether the ballots will be shuffled or not. This line is optional, so if
it's omitted, the ballots will stay unshuffled. That being said, if one selected file
has this additional line in the header, then all selected files should have this line.