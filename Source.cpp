
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdio.h>
using namespace std;

// initialised functions in the program
int menu();
void covidRecommendation();
bool getFile(string file, vector<string>& vectorContents);
bool inputPatient(vector<string>& vecSymptoms, vector<string>& vecHighRiskLocations);
void FilelocationUpdater(string location);
void CovidStatus();
void HighRiskLocations();
int IDFinder(string ID);
void FileUpdater(int replaceLine, string replacement);
void PatientLocationUpdater(int replaceLine, string Replacement);
void FileCovidUpdater(int replaceLine);
void UpdatePatient();
void DisplayCovidPatients();
bool searchDupID(string id);
string isEmpty(string input, string question);



/*
main - Erica Mayo 102588456
Switch statement thats gets user's selection via menu function
Checks input is valid
*/
int main()
{
	int choice;
	choice = menu();
	while (choice != 6) {
		switch (choice) {

		case 1: //Registers new patients then makes recommendantion of test or isolation based on inputs
			covidRecommendation();
			break;

		case 2: // Gets patients ID and updates their COVID test status and additional locations
			CovidStatus();
			break;

		case 3: // Displays current high risk locations
			HighRiskLocations();
			break;

		case 4: // Updates details of new COVID positive patients
			UpdatePatient();
			break;

		case 5: // Displays details of COVID Positive patients
			DisplayCovidPatients();
			break;

		case 6: // Exits program
			cout << "Goodbye";
			break;

		default: // Clears input and states invalid input
			cin.clear();
			cout << "Selection invalid. Enter a number from 1-6.\n";

		}
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		choice = menu();
	}
	return 0;
}


/*
menu - Erica Mayo 102588456
Shows user numbered list for selection and returns selection to main
*/

int menu() {
	int choice;
	cout << "\n";
	cout << "1 - Enter your detail for COVID-Test Recommendation\n";
	cout << "2 - Submit Your Covid test status and update the location database\n";
	cout << "3 - Display the Updated Location (High Risk for COVID)  \n";
	cout << "4 - Update COVID Patient Details  \n";
	cout << "5 - Display the COVID Positive Patient Detail  \n";
	cout << "6 - Quit \n";
	cout << "\nEnter your choice: ";
	cin >> choice; //gets users selection
	return choice; //returns users choice to main
}


/*
covidRecommendation - Erica Mayo 102588456
Top function controlling covid recommendation process
Displays recommendation based on returned results
*/

void covidRecommendation() {
	vector<string> vecSymptoms;
	vector<string> vecHighRiskLocations;
	bool result = getFile("symptoms.txt", (vecSymptoms)); // Checks if symptoms file can be accessed and places contents into given vector
	if (result) {
		bool result = getFile("highRiskLocations.txt", vecHighRiskLocations); // Checks if high risk locations file can be accessed and places contents into given vector
		if (result) {
			bool recommendation = inputPatient(vecSymptoms, vecHighRiskLocations); // Passes vectors to gather patient details
			// if true is returned the recommendation for a test is output
			if (recommendation) {
				cout << "\n";
				cout << "*****************************************************************" << endl;
				cout << "     COVID Test Recommended - see your closest testing site." << endl;
				cout << "*****************************************************************" << endl;

			}
			// if false is returned the recommendation for a test is output
			else {
				cout << "\n";
				cout << "*****************************************************************" << endl;
				cout << "     COVID Test NOT Recommended - isolate yourself at home." << endl;
				cout << "*****************************************************************" << endl;
			}
		}
		else {
			cout << "\nNo high risk locations present\n"; // output when there is no file of high risk locations
		}
	}
	else {
		cout << "\nUnable to recommend COVID Test - required data missing, no symptoms loaded\n"; // output when there is no file of symptoms
	}
}



/*
getFile - Erica Mayo 102588456
Opens file name given and reads file into the adress of given vector
*/

bool getFile(string file, vector<string>& vectorContents) {
	ifstream contents(file);
	if (!contents) { // checks file can be opened
		return false;
	}

	string str;
	while (getline(contents, str)) { // adds line of file to the end of vector
		vectorContents.push_back(str);
	}
	contents.close(); // closes file after completion
	return true;
}



/*
inputPatient - Erica Mayo 102588456
Gathers inputs for patient data and appends them to a file, returns to covidRecommendation
*/
bool inputPatient(vector<string>& vecSymptoms, vector<string>& vecHighRiskLocations) {

	//initialising values
	string PatientID, ID, firstName, familyName, DOB, address, overseasTravel;
	bool duplicateID = true;
	int symptomSelection, highRiskVisited, length;
	bool testStatus = false;
	string recoverySatus = "recoverySatus";
	string livingStatus = "livingStatus";

	//opening patient file to add new patients, creates file if one does not exist
	fstream patientRecord("patient.txt", ios::app);
	cin.clear();
	cin.ignore();

	//checks patient.txt has successfully opened then asks user input of required patient details
	if (patientRecord.is_open()) {
		cout << "Enter new patient ID. Patient ID can only contain numbers."<< endl;
		cout << "New ID: ";
		getline(cin, PatientID);
		PatientID = isEmpty(PatientID, "New ID: ");
		while (duplicateID) { // Loops until a non-duplicate patient ID has been entered
			ID = "ID: " + PatientID;
			bool IDFound = searchDupID(ID); // passes ID to searchDupID function to check if it is already contained in the file
			if (IDFound == 1) {
				duplicateID = 1;
				cout << "Please enter a new ID, this has already been entered or is invaild" << endl;
				cout << "Patient ID can only contain numbers." << endl;// notifies if ID is a duplicate
				cin.clear();
				cout << "New ID:" << endl;
				getline(cin, PatientID); // gets new ID from user
				PatientID = isEmpty(PatientID, "New ID: ");
			}
			else {
				duplicateID = 0;
				cin.clear();
				cout << "Patient ID entered is: " << PatientID << "\n"; // notifies of ID successfully entered
				patientRecord << "\n\n" << ID; // appends ID to file
			}
		}

		cout << "Enter patient first name: ";
		cin.clear();
		getline(cin, firstName);
		firstName = isEmpty(firstName, "Enter patient first name: ");
		patientRecord << "\nName: " << firstName; // appends first name to file

		cout << "Enter patient family name: ";
		cin.clear();
		getline(cin, familyName);
		familyName = isEmpty(familyName, "Enter patient family name: ");
		patientRecord << "\nSurname: " << familyName; // appends surname to file

		cout << "Enter patient DOB: ";
		cin.clear();
		getline(cin, DOB);
		DOB = isEmpty(DOB, "Enter patient DOB: ");
		patientRecord << "\nDOB: " << DOB; // appends DOB to file

		cout << "Enter if patient has travelled overseas: ";
		cin.clear();
		getline(cin, overseasTravel);
		overseasTravel = isEmpty(overseasTravel, "Enter if patient has travelled overseas: ");
		
		length = overseasTravel.length();
		for (int i = 0;i < length;i++) {
			overseasTravel[i] = tolower(overseasTravel[i]);
		}
		patientRecord << "\nOverseas Travel: " << overseasTravel; // appends overseas travel response to file

		
		// outputs vector elements to user with a number infront
		length = vecSymptoms.size();
		for (int i = 0; i < length; i++) {
			cout << i + 1 << ": " << vecSymptoms[i] << endl;
		}
		cout << length+1 << ": No symptoms \n";
		cout << "\nEnter " << length + 2 << " after adding all your symptoms.\n"; // lets user know how to stop entry or add no symptoms
		cout << "Select symptoms from list: ";
		cin.clear();
		patientRecord << "\nSymptoms: ";
		cin >> noskipws >> symptomSelection; // gets input from user
		if (symptomSelection == length + 2|| symptomSelection == length + 1) {
			patientRecord << "No symptoms"; // appends to file if no symptoms were selected
			symptomSelection = length + 2;
		}

		while (symptomSelection != length + 2) { // looping until given number is entered
			if (symptomSelection > 0 && symptomSelection - 1 < length) { // checks whether entry is in range
				patientRecord << vecSymptoms[symptomSelection - 1] << ", "; // appends selected symptom to file
				if (symptomSelection > 0 && symptomSelection <= 4) // file has been sorted so that first element to fourth is high risk symptom, "hard coded"
				{
					testStatus = true; // returns true that user requires a COVID Test to covidRecommendation()
				}
				else if (overseasTravel == "yes")
				{
					testStatus = true;
					cin.clear();
					cin.ignore();
					symptomSelection = -1;
				}
			}
			else if (symptomSelection == length + 1) {
				patientRecord << "No symptoms"; // appends to file if no symptoms were selected
				break;
			}
			else if (symptomSelection == 0)
			{ 
				cout << "No selection found." << endl;
				cout << "Selection, must be from 0 - " << length + 2 << endl;
			}
			else { // entry not in range
				cout << "Not a valid selection, must be from 0 - " << length + 2 << endl; // notifies user of invalid selection
			}
			cin.clear(); // clears entry
			cin.ignore();
			cin >> noskipws >> symptomSelection; // gets input from user
		}

		
		length = vecHighRiskLocations.size();
		// outputs vector elements to user with a number infront
		for (int i = 0; i < length; i++) {
			cout << i + 1 << ": " << vecHighRiskLocations[i] << endl;
		}
		cout << length + 1 << ": Haven't recently been to any locations listed above\n";

		cout << "\nEnter " << length + 2 << " after adding all visited locations.\n"; // lets user know how to stop entry or add no locations
		cout << "Select high risk locations visited from list: ";
		cin.clear();
		patientRecord << "\nHigh Risk Locations: ";
		cin >> noskipws >> highRiskVisited; // gets input from user
		if (highRiskVisited == length + 1|| highRiskVisited == length + 2) {
			patientRecord << "No high risk locations visited"; // appends to file if no locations were selected
			highRiskVisited = length + 2;
		}

		while (highRiskVisited != length + 2) { // looping until given number is entered
			if (highRiskVisited > 0 && highRiskVisited - 1 < length) { // checks whether entry is in range
				patientRecord << vecHighRiskLocations[highRiskVisited - 1] << ", "; // appends selected symptom to file
				testStatus = true; // returns true that user requires a COVID Test to covidRecommendation(), as necessary with any high risk location visit
				cin.clear();
			}
			else if (highRiskVisited == length + 1) {
				patientRecord << "No high risk locations visited"; // appends to file if no locations were selected
				break;
			}
			else if (highRiskVisited == 0)
			{
				cout << "No selection found." << endl;
				cout << "Selection, must be from 0 - " << length + 2 << endl;
			}
			else {
				cout << "Not a valid selection, must be from 0 - " << length + 2<< endl; // notifies user of invalid selection
			}
			cin.clear();
			cin.ignore();
			cin >> noskipws >> highRiskVisited; // gets input from user
		}

		if (testStatus) {
			patientRecord << "\nReccomdation: Test Required. "; // appends if COVID Test is required
			patientRecord << "\nCovid Test Result: Pending"; //place holder for future use in CovidTestStatus()
		}
		else {
			patientRecord << "\nReccomdation: Isolation Required. "; // appends if isolation is required
			patientRecord << "\nCovid Test Result: N/A"; //place holder for future use in CovidTestStatus()
		}
		patientRecord << "\nRecovery Satus: N/A"; // appends for future use in UpdatePatient()
		patientRecord << "\nLiving Status: Alive"; // appends for future use in DisplayPatient()
		patientRecord.close();
		return testStatus;
	}
	return testStatus;
}

/*
isEmpty - Michelle Ko 102927554
function that checks if a string is empty*/
string isEmpty(string input, string question)
{
	while (input.empty())
	{
		cout << "\nNo entry made. Please re-enter information." << endl;
		cout << question;
		getline(cin, input);
		
	}
	return input;
}

/* 
isNumber - Michelle Ko 102927554
function that checks if the ID input is a number*/
bool isNumber(string ID)
{
	int length = ID.length();
	for (int i = 4; i < length; i++)
	{
		if (isdigit(ID[i]) == false)
			return false;
	}
	return true;
}

/*
searchDupID - Erica Mayo 102588456 / Michelle Ko 102927554
Checkes whether an string is present in file, used to fine ID
*/

bool searchDupID(string id) {
	int searchLine = 0;
	string out;
	ifstream FileSearch("patient.txt"); // opens patient file
	// searches for the phrase in a file
	if (isNumber(id) == false)
	{
		return true;
	}
	while (!FileSearch.eof())
	{
		getline(FileSearch, out);
		if (out.find(id) != string::npos) { // returns true if matching ID is found in file
			return true;
		}
		searchLine++; // moves to next line
	}
	FileSearch.close(); // closes file
	return false;
}

/*
FilelocationUpdater - Michelle Ko 102927554
function add new high risk locations to the high risk locations file*/
void FilelocationUpdater(string location)
{
	ofstream Locationfile;
	Locationfile.open("highRiskLocations.txt", ::ios::app); //opens file for writing and appends it

	Locationfile << location << endl; //enters new location to the bottom of the file
	Locationfile.close();
}

/*
PatientLocationUpdater - Michelle Ko 102927554
function updates patient file with the new high 
risks locations a patient has been to*/
void PatientLocationUpdater(int replaceLine, string Replacement)
{
	ifstream oldfile;
	oldfile.open("patient.txt");
	ofstream newfile;
	newfile.open("temp.txt");
	string oldLine;
	int counter = 0;

	while (!oldfile.eof())
	{
		getline(oldfile, oldLine);
		if (counter == replaceLine)
		{
			if (oldLine == "High Risk Locations: No high risk locations visited")
			{
				oldLine = "High Risk Locations: " + Replacement + ',';
			}
			else
			{
				oldLine = oldLine + ' ' + Replacement + ',';
			}
		}
		oldLine += "\n";
		newfile << oldLine;
		counter++;
	}
	oldfile.close();
	newfile.close();

	remove("patient.txt");
	rename("temp.txt", "patient.txt");
}

/*
FileCovidUpdater - Michelle Ko 102927554
function puts any patients that have said they have a
positve covid test result into the postive covid patients file*/
void FileCovidUpdater(int replaceLine)
{
	ifstream oldfile;
	oldfile.open("patient.txt", ::ios::in); // opens patient file to be read
	ofstream newfile;
	newfile.open("PositivePatients.txt", ::ios::app); //opens postivePatients file to be written and appends it
	string patientInfo;
	vector<string> arr;

	while (!oldfile.eof())
	{
		//reads the patient file and put it all in a vector
		getline(oldfile, patientInfo);
		arr.push_back(patientInfo);
	}
	// selects the information of the postive patient, except their ID, from the regular patient file
	// adding it to the PostivePatients file
	for (int i = 0; i < 11; i++)
	{
		newfile << arr[replaceLine + i + 1] << endl;
	}
	oldfile.close();
	newfile.close();
}
/*
CovidStatus - Michelle Ko 102927554
function puts asks the user if they'd like to update their covid test results
and call functions that call the appropriate file editor depending on if they
test positive*/
void CovidStatus() {

	string ID, NewLocation;
	bool run = true;
	int testLocation, locationLine;

	string yesorno;
	cin.clear();
	cin.ignore();
	cout << "Enter the ID of the Patient you would like to update the covid test result for: " << endl;
	cin >> ID;

	int IDLocation = IDFinder(ID);
	//asks user if they would like to update their covid test results
	cout << "Would you like to update your patient covid test results?\n";
	cout << "1 - Yes\n";
	cout << "2 - No\n";
	cin.clear();
	cin.ignore();
	cout << "Answer: ";
	cin >> noskipws >> yesorno;
	while (run)
	{
		
		if (yesorno == "1") {

			while (run)
			{
				string pos_or_neg;
				cin.clear();
				cin.ignore();
				cout << "New test result entry: \n";
				cout << "1 - Positive\n";
				cout << "2 - Negative\n";
				cout << "Answer: ";
				cin >> pos_or_neg;
				if (pos_or_neg == "1") {
					testLocation = IDLocation + 8;
					FileUpdater(testLocation, "Covid Test Result: Positive"); //updates main patient file to read postive test resut
					cin.clear(); // clears cin
					cout << "Enter the last locations you've visted as well as the time and date you visted them. " << endl;
					cin.ignore();
					cout << "Enter x if you finished inputing all the locations you've visited." << endl;
					cout << "Location, time and date: " << endl;
					getline(cin, NewLocation);
					locationLine = IDLocation + 6;
					while (run) //keeps asking for locations the patient has visted until run is false
					{
						if (NewLocation == "x") { //checks if user is done adding locations they've visited
							FileCovidUpdater(IDLocation); //adds patient to the postivePatients file
							run = false; // exists loop
						}
						else if (NewLocation.empty()) //checks if the user input is empty
						{
							//notifys user input is empty. prompts them to reenter or finish adding locations
							cout << "No location, time and date entered." << endl;
							cout << "Enter x if you finished inputing all the locations you've visited." << endl;
							cout << "Location, time and date: " << endl;
							getline(cin, NewLocation);
						}
						else
						{
							cin.clear();
							PatientLocationUpdater(locationLine, NewLocation); // updates the locations the user has visited in the patient file
							FilelocationUpdater(NewLocation); //updates the high risk locations file with the new visisted location
							cout << "Enter x if you finished inputing all the locations you've visited." << endl; // prompts new location entry
							cout << "Location, time and date: " << endl;
							getline(cin, NewLocation);
						}
					}
					cout << "Location File and Patient Status File has been updated!"; //lets user know the file has been updated
					break;
				}
				else if (pos_or_neg == "2") {
					testLocation = IDLocation + 8;
					FileUpdater(testLocation, "Covid Test Result: Negative"); //updates negative test result
					cout << "Patient Covid test result has been updated!"; //lets user know the file has been updated
					run = false;
					break;
				}
				else
				{
					cin.clear();
					cout << "\nSelection invalid. Enter a number from 1-2.\n"; //lets user know the file has been updated
				}
			}
		}
		else if (yesorno == "2") {
			run = false;
			break;
		}
		else
		{
			cin.clear();
			cin.ignore();
			cout << "\nSelection invalid. Enter a number from 1-2.\n";
			cout << "Would you like to update your patient covid test results?\n";
			cout << "1 - Yes\n";
			cout << "2 - No\n";
			cout << "Answer: ";
			cin >> noskipws >> yesorno;
		}
	}
	
}


/*
HighRiskLocations - Erica Mayo 102588456
Opens high risk locations file and outputs to console, ensuring vector is up to date
*/
void HighRiskLocations() {
	cout << "\n";
	cout << "*****************************************************************" << endl;
	cout << "                     High Risk Locations" << endl;
	cout << "*****************************************************************" << endl;
	vector<string> vecHighRiskLocations;
	bool result = getFile("highRiskLocations.txt", vecHighRiskLocations); // Checks if high risk locations file can be accessed and places contents into given vector
	int length = vecHighRiskLocations.size();
	if (result) {
		// outputs vector elements to user with a number infront
		for (int i = 0; i < length; i++) {
			cout << i + 1 << ": " << vecHighRiskLocations[i] << endl;
		}
	}

	else {
		cout << "\nNo high risk locations present\n"; // output when there is no file of high risk locations
	}
}

/*
IDFinder - Michelle ko 102927554
Function that searches for a patient ID in the patient file
and returns the line is occurs on
*/
int IDFinder(string ID)
{
	int searchLine = 0; //stores the count a line occurs on
	int pos = 0;
	string search; // stores the line being search for the ID
	bool found = false;
	ifstream FileSearch("patient.txt");
	// searches for the pharse in a file
	ID = "ID: " + ID;

	while (found == false)
	{
		while (!FileSearch.eof())
		{
			getline(FileSearch, search);
			if (search.find(ID) != string::npos) {
				found = true;
				break;// stops the line counter when the id is found
			}
			searchLine++;
		}
		if (found == true)
		{
			break;
		}
		else
		{
			cin.clear();
			cin.ignore();
			cout << "ID not found. Please enter a valid ID. " << endl;
			cin >> ID; // gets new ID
			ID = "ID: " + ID;
			FileSearch.clear(); //clears the file stream
			FileSearch.seekg(0); // goes back to the top of the file
			searchLine = 0; // resets line counter
		}
	}

	FileSearch.close();
	return searchLine; //returns the line number the ID was found on
}

/*
FileUpdater - Michelle ko 102927554
Function that takes a new string and a line number that the old string
occurs on to replace it and update the patient file by creating a new patient file
and having it replace the out of date one
*/
void FileUpdater(int replaceLine, string replacement)
{

	ifstream oldfile;
	oldfile.open("patient.txt"); //opens read from patient file
	ofstream newfile;
	newfile.open("temp.txt"); //creates temporary write file
	string oldLine;
	int counter = 0;

	while (!oldfile.eof())
	{
		//write all of patient file data into temporary file and replaces the line 
		//needs to be replaced in the patient file
		getline(oldfile, oldLine);
		if (counter == replaceLine) // finds location in file of the line that needs to be replaced
		{
			oldLine = replacement;
		}
		oldLine += "\n";
		newfile << oldLine; // inserts data into temporary file
		counter++;
	}
	oldfile.close();
	newfile.close();

	remove("patient.txt"); //deletes out of date patient file
	rename("temp.txt", "patient.txt"); // renames temporary file into the updated patient file

}

/*
UpdatePatient - Michelle ko 102927554
Function that asks the user what information they'd like to update
and passes the information to the FileUpdater function
*/
void UpdatePatient() {

	string ID;
	cin.clear();
	cin.ignore();
	cout << "Enter the ID of the Patient you would like to edit: " << endl;
	cin >> ID;

	int patient = IDFinder(ID);
	int TravelLoc, RecovLoc, LifeLoc;
	int option = 0;
	int UserInput = 0;
	bool run = true;
	string newInfo;

	while (option != 4) { // function keeps until the user says they're done editing with a 4
		run = true;
		cout << "Select the item you would like to edit by entering a number.\nEnter 4 to exit the edit menu and return to main menu.\n";
		cout << "1. International Travel status\n";
		cout << "2. Covid Recovery Status\n";
		cout << "3. Patient Living Status\n";
		cout << "4. Return to main menu\n";
		cin >> option;

		switch (option)
		{
		case 1:

			TravelLoc = patient  + 4;
			while (run == true)
			{
				cout << "Have you recently been overseas?\n";
				cout << "1 - Yes\n";
				cout << "2 - No\n";
				cin >> UserInput;
				switch (UserInput)
				{
				case 1: //patient has been overseas resently
					newInfo = "Overseas Travel: Yes";
					FileUpdater(TravelLoc, newInfo);
					run = false;
					break;
				case 2: //patient has not been overseas resently
					newInfo = "Overseas Travel: No";
					FileUpdater(TravelLoc, newInfo);
					run = false;
					break;
				default: //exception handling
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n'); // ignores all inputs after first character
					cout << "Invalid selection. Please reselect\n";
				}
			}
			break;

		case 2:

			RecovLoc = patient + 9;
			while (run == true)
			{
				cout << "Have you recovered from covid?\n";
				cout << "1 - Yes\n";
				cout << "2 - No\n";
				cin >> UserInput;
				switch (UserInput)
				{
				case 1: // patient has recovered from covid
					newInfo = "Recovery Satus: Patient has Recovered from Covid";
					FileUpdater(RecovLoc, newInfo);
					run = false;
					break;
				case 2: // patient yet to recover from covid
					newInfo = "Recovery Satus: Patient has not recovered from Covid";
					FileUpdater(RecovLoc, newInfo);
					run = false;
					break;
				default: // exception handling
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n'); // ignores all inputs after first character
					cout << "Invalid selection. Please reselect\n";
				}
			}
			break;

		case 3:
			LifeLoc = patient + 10;
			while (run == true)
			{
				cout << "Is the covid paitent dead or alive\n";
				cout << "1 - Deceased\n";
				cout << "2 - Alive\n";
				cin >> UserInput;
				switch (UserInput)
				{
				case 1: // patient is dead

					newInfo = "Living Status: Deceased";
					FileUpdater(LifeLoc, newInfo);
					run = false;
					break;
				case 2: // patient is alive
					newInfo = "Living Status: Aive";
					FileUpdater(LifeLoc, newInfo);
					run = false;
					break;
				default: // exception handling
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n'); // ignores all inputs after first character
					cout << "Selection Invalid. Please reselect\n";
				}
			}
			break;

		case 4:

			cout << "File edited and saved.\n"; // let's user know patient details have been successfully updated
			break;

		default: // Clears input and states invalid input
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n'); // ignores all inputs after first character
			cout << "\nSelection invalid. Enter a number from 1-4.\n";
		}
	}
}


void DisplayCovidPatients() {
	cout << "\n";
	cout << "*****************************************************************" << endl;
	cout << "               Positive Tested Covid Patients" << endl;
	cout << "*****************************************************************" << endl;
	vector<string> vecPostivePatients;
	bool result = getFile("PositivePatients.txt", vecPostivePatients); // Checks if high risk locations file can be accessed and places contents into given vector
	int length = vecPostivePatients.size();
	if (result) {
		// outputs vector elements to user with a number infront
		for (int i = 0; i < length; i++) {
			cout << vecPostivePatients[i] << endl;
		}
	}

	else {
		cout << "\nNo Patients have tested positive for covid.\n"; // output when there is no file of positive patients
	}
}