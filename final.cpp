#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdio.h>
using namespace std;

int menu();
void covidRecommendation();
bool getFile(string file, vector<string>& vectorContents);
bool inputPatient(vector<string>& vecSymptoms, vector<string>& vecHighRiskLocations);
void CovidStatus();
void HighRiskLocations();
int IDFinder(string ID);
void FileUpdater(int ReplaceNum, string NewLine);
void UpdatePatient();
void DisplayCovidPatients();
bool searchDupID(string id);



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
			cin.ignore();
			cout << "Selection invalid. Enter a number from 1-6.\n";

		}
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
	int symptomSelection, highRiskVisited;
	bool testStatus = false;
	string recoverySatus = "recoverySatus";
	string livingStatus = "livingStatus";

	//opening patient file to add new patients, creates file if one does not exist
	fstream patientRecord("patient.txt", ios::app);
	cin.clear();
	cin.ignore();

	//checks patient.txt has successfully opened then asks user input of required patient details
	if (patientRecord.is_open()) {
		cout << "Enter patient ID: ";
		cin >> PatientID;
		while (duplicateID) { // Loops until a non-duplicate patient ID has been entered
			ID = "ID: " + PatientID;
			bool IDFound = searchDupID(ID); // passes ID to searchDupID function to check if it is already contained in the file
			cin.clear();
			cin.ignore();
			if (IDFound == 1) {
				duplicateID = 1;
				cout << "Please enter a new ID, this has already been entered or is invaild" << endl;
				cout << "Patient ID can only contain numbers." << endl;// notifies if ID is a duplicate
				cin >> PatientID; // gets new ID from user
			}
			else {
				duplicateID = 0;
				cin.clear();
				cout << "Patient ID entered is: " << PatientID << "\n"; // notifies of ID successfully entered
				patientRecord << "\n\n" << ID; // appends ID to file
			}
		}

		cout << "Enter patient first name: ";
		getline(cin, firstName);
		patientRecord << "\nName: " << firstName; // appends first name to file

		cout << "Enter patient family name: ";
		getline(cin, familyName);
		patientRecord << "\nSurname: " << familyName; // appends surname to file

		cout << "Enter patient DOB: ";
		getline(cin, DOB);
		patientRecord << "\nDOB: " << DOB; // appends DOB to file

		cout << "Enter if patient has travelled overseas: ";
		getline(cin, overseasTravel);
		for (int i = 0;i < overseasTravel.length();i++) {
			overseasTravel[i] = tolower(overseasTravel[i]);
		}
		patientRecord << "\nOverseas Travel: " << overseasTravel; // appends overseas travel response to file

		// outputs vector elements to user with a number infront

		for (int i = 0; i < vecSymptoms.size(); i++) {
			cout << i + 1 << ": " << vecSymptoms[i] << endl;
		}
		cout << "\nEnter " << vecSymptoms.size() + 1 << " after adding all or no symptoms.\n"; // lets user know how to stop entry or add no symptoms
		cout << "Select symptoms from list: ";
		patientRecord << "\nSymptoms: ";
		cin >> symptomSelection; // gets input from user
		cin.clear();
		cin.ignore();
		if (symptomSelection == vecSymptoms.size() + 1) {
			patientRecord << "No symptoms"; // appends to file if no symptoms were selected
		}

		while (symptomSelection != vecSymptoms.size() + 1) { // looping until given number is entered
			if (symptomSelection > 0 && symptomSelection - 1 < vecSymptoms.size()) { // checks whether entry is in range
				patientRecord << vecSymptoms[symptomSelection - 1] << ", "; // appends selected symptom to file
				if (symptomSelection > 0 && symptomSelection <= 4) // file has been sorted so that first element to fourth is high risk symptom, "hard coded"
					testStatus = true; // returns true that user requires a COVID Test to covidRecommendation()
				else if (overseasTravel == "yes")
				{
					testStatus = true;
				}
			}
			else { // entry not in range
				cout << "Not a valid selection, must be from 1 - " << vecSymptoms.size() + 1 << endl; // notifies user of invalid selection
			}
			cin >> symptomSelection; // gets input from user
			cin.clear(); // clears entry
			cin.ignore();
		}

		// outputs vector elements to user with a number infront
		for (int i = 0; i < vecHighRiskLocations.size(); i++) {
			cout << i + 1 << ": " << vecHighRiskLocations[i] << endl;
		}

		cout << "\nEnter " << vecHighRiskLocations.size() + 1 << " after adding all or no locations.\n"; // lets user know how to stop entry or add no locations
		cout << "Select high risk locations visited from list: ";
		patientRecord << "\nHigh Risk Locations: ";
		cin >> highRiskVisited; // gets input from user
		cin.clear();
		cin.ignore();
		if (highRiskVisited == vecHighRiskLocations.size() + 1) {
			patientRecord << "No high risk locations visited"; // appends to file if no locations were selected
		}

		while (highRiskVisited != vecHighRiskLocations.size() + 1) { // looping until given number is entered
			if (highRiskVisited > 0 && highRiskVisited - 1 < vecHighRiskLocations.size()) { // checks whether entry is in range
				patientRecord << vecHighRiskLocations[highRiskVisited - 1] << ", "; // appends selected symptom to file
				testStatus = true; // returns true that user requires a COVID Test to covidRecommendation(), as necessary with any high risk location visit
			}
			else {
				cout << "Not a valid selection, must be from 1 -" << vecHighRiskLocations.size() + 1; // notifies user of invalid selection
			}
			cin >> highRiskVisited; // gets input from user
			cin.clear();
			cin.ignore();
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
}


bool isNumber(string ID)
{
	for (int i = 4; i < ID.length(); i++)
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

void FilelocationUpdater(string location)
{
	ofstream Locationfile;
	Locationfile.open("highRiskLocations.txt",::ios::app);

	Locationfile << location<<endl;
	Locationfile.close();
}

void PatientLocationUpdater(int ReplaceNum, string NewLine)
{
	ifstream oldfile;
	oldfile.open("patient.txt" );
	ofstream newfile;
	newfile.open("temp.txt");
	string replace;
	int counter = 0;

	while (!oldfile.eof())
	{
		getline(oldfile, replace);
		if (counter == ReplaceNum)
		{
			if (replace == "High Risk Locations: No high risk locations visited")
			{
				replace = "High Risk Locations: " + NewLine + ',';
			}
			else
			{
				replace = replace + ' ' + NewLine + ',';
			}
		}
		replace += "\n";
		newfile << replace;
		counter++;
	}
	oldfile.close();
	newfile.close();

	remove("patient.txt");
	rename("temp.txt", "patient.txt");
}

void FileCovidUpdater(int ReplaceNum)
{
	ifstream oldfile;
	oldfile.open("patient.txt", ::ios::in);
	ofstream newfile;
	newfile.open("PositivePatients.txt", ::ios::app);
	string replace = "";
	int counter = 0;
	vector<string> arr;

	while (!oldfile.eof())
	{
		getline(oldfile, replace);
		arr.push_back(replace);

		counter++;
	}
	for (int i = 0; i < 11; i++)
	{
		newfile << arr[ReplaceNum + i+1] << endl;
	}
	oldfile.close();
	newfile.close();
}

void CovidStatus() {

	string ID;
	string line_2;
	bool run = true;
	int res, location;
	cout << "Enter the ID of the Patient you would like to update the covid test result for: " << endl;
	cin >> ID;

	int b_2 = IDFinder(ID);

	int yesorno;
	cout << "Would you like to update your patient covid test results?\n";
	cout << "1 - Yes\n";
	cout << "2 - No\n";
	cout << "Answer: ";
	cin >> yesorno;
	switch (yesorno)
	{
	case 1:
		int pos_or_neg;
		cout << "New test result entry: \n";
		cout << "1 - Positive\n";
		cout << "2 - Negative\n";
		cout << "Answer: ";
		cin >> pos_or_neg;
		switch (pos_or_neg)
		{
		case 1:
			res = b_2 + 8;
			FileUpdater(res, "Covid Test Result: Positive");
			cin.clear();
			cout << "Enter the last locations you've visted as well as the time and date you visted them. " << endl;
			cin.ignore();
			while (run)
			{
				if (line_2 == "x") {
					FileCovidUpdater(b_2);
					run = false;
				}
				else if (line_2.empty())
				{
					cout << "Enter x if you finished inputing all the locations you've visited." << endl;
					cout << "Location, time and date: " << endl;
					getline(cin, line_2);
				}
				else
				{
					location = b_2 + 6;
					PatientLocationUpdater(location, line_2);
					FilelocationUpdater(line_2);
					cout << "Enter x if you finished inputing all the locations you've visited." << endl;
					cout << "Location, time and date: " << endl;
					getline(cin, line_2);
				}
			}
			cout << "Location File and Patient Status File has been updated!" << endl;
			break;

		case 2:
			res = b_2 + 8;
			FileUpdater(res, "Covid Test Result: Negative");
			cout << "Patient Covid test result has been updated!" << endl;
			break;
		default:
			cin.clear();
			cin.ignore();
			cout << "\nSelection invalid. Enter a number from 1-2.\n";
		}
	case 2:
		break;

	default:
		cin.clear();
		cin.ignore();
		cout << "\nSelection invalid. Enter a number from 1-2.\n";
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
	if (result) {
		// outputs vector elements to user with a number infront
		for (int i = 0; i < vecHighRiskLocations.size(); i++) {
			cout << i + 1 << ": " << vecHighRiskLocations[i] << endl;
		}
	}

	else {
		cout << "\nNo high risk locations present\n"; // output when there is no file of high risk locations
	}
}

int IDFinder(string ID)
{
	int searchLine = 0;
	int counter = 0;
	int pos = 0;
	string out;
	bool found = false;
	ifstream FileSearch("patient.txt");
	// searches for the pharse in a file
	ID = "ID: " + ID;

	while (found == false)
	{
		while (!FileSearch.eof())
		{
			getline(FileSearch, out);
			if (out.find(ID) != string::npos) {
				found = true;
				break;
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
			cin >> ID;
			ID = "ID: " + ID;
			FileSearch.clear();
			FileSearch.seekg(0);
			searchLine = 0;
		}
	}

	FileSearch.close();
	return searchLine;
}

void FileUpdater(int ReplaceNum, string NewLine)
{

	ifstream oldfile;
	oldfile.open("patient.txt");
	ofstream newfile;
	newfile.open("temp.txt");
	string replace;
	int counter = 0;


	while (!oldfile.eof())
	{
		getline(oldfile, replace);
		if (counter == ReplaceNum)
		{
			replace = NewLine;
		}
		replace += "\n";
		newfile << replace;
		counter++;
	}
	oldfile.close();
	newfile.close();

	remove("patient.txt");
	rename("temp.txt", "patient.txt");

}

void UpdatePatient() {

	string ID;
	cout << "Enter the ID of the Patient you would like to edit: " << endl;
	cin >> ID;

	int b = IDFinder(ID);
	int TravelLoc, RecovLoc, LifeLoc;
	int option = 0;
	string NewLine;

	while (option != 4) {
		cin.clear();
		cin.ignore();
		cout << "Select the item you would like to edit by entering a number.\nEnter 4 to exit the edit menu and return to main menu.\n";
		cout << "1. International Travel status\n";
		cout << "2. Covid Recovery Status\n";
		cout << "3. Patient Living Status\n";
		cout << "4. Return to main menu\n";
		cin >> option;

		switch (option)
		{
		case 1:

			cout << "Have you recently been overseas, yes or no?\n";
			TravelLoc = b + 4;
			cin >> NewLine;
			NewLine = "Overseas Travel: " + NewLine;
			FileUpdater(TravelLoc, NewLine);
			break;

		case 2:

			cout << "Are you recovering or recovered from covid?\n";
			RecovLoc = b + 9;
			cin >> NewLine;
			NewLine = "Recovery Satus: " + NewLine;
			FileUpdater(RecovLoc, NewLine);
			break;

		case 3:

			cout << "Is the covid paitent dead or alive\n";
			LifeLoc = b + 10;
			cin >> NewLine;
			NewLine = "Living Status: " + NewLine;
			FileUpdater(LifeLoc, NewLine);
			break;

		case 4:

			cout << "File edited and saved.\n";
			break;

		default: // Clears input and states invalid input
			cin.clear();
			cin.ignore();
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
	if (result) {
		// outputs vector elements to user with a number infront
		for (int i = 0; i < vecPostivePatients.size(); i++) {
			cout << vecPostivePatients[i] << endl;
		}
	}

	else {
		cout << "\nNo Patients have tested positive for covid.\n"; // output when there is no file of high risk locations
	}
}