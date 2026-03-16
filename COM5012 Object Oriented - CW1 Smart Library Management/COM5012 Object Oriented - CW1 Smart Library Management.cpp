// COM5012 Object Oriented - CW1 Smart Library Management
// COM5012 SLMS System -> Main File


// Libraries 

#include <iostream>
#include <String>
#include <stdlib.h>
#include <stdexcept>
#include <limits> 
#include <windows.h>
#include <list>
#include <fstream>
#include <vector>

// Custom Classes

#include "Member.h"
#include "BorrowedRecord.h"
#include "ReservedRecord.h"

using namespace std; 

// Global Variables

string currentScreen;



void ChangeColourText(int colourCode, string text) {
    HANDLE console_color;
    console_color = GetStdHandle(
        STD_OUTPUT_HANDLE);

    SetConsoleTextAttribute(console_color, colourCode);

    cout << text;

    SetConsoleTextAttribute(console_color, 7);
}

void Title()
{
    ChangeColourText(10, "High Wycombe City Library System\n");
    ChangeColourText(10, "Current Screen : " + currentScreen + "\n");
    cout << "=====================================\n\n";
}

int TakeNumericInput(int maximum, string userInputSentence) {
    // Stores a valid numeric input and returns it

    bool foundValue = false;
    int finalValue = 0;

    do {
        system("CLS");

        Title();

        cout << userInputSentence;

        string userInput;

        getline(cin, userInput);

        try {
            int input = stoi(userInput);

            if (input < maximum && input > 0)
            {
                foundValue = true;
                finalValue = input;
            }
            else 
            {
                cout << "Please write an input between 0 and " << maximum;
                cin.get();
            }
        }
        catch (invalid_argument& e) {

            cout << "Be sure to write a valid, whole integer. ";
            cin.get();
        }
    } while (!foundValue);

    return finalValue;
}

bool Login() {

    list<MemberC> memberList;

    ifstream inFile; 

    inFile.open("Users.txt");

    if (!inFile) {
        cout << "Unable to open file Users.txt";
        exit(1);
    }

    string line;
    MemberC currentMember;
    BorrowedRecord currentBorrowRecord;
    ReservedRecord currentReservedRecord;

    bool isCurrentlyReadingMember = false;
    bool isCurrentlyReadingBorrowRecord = false;
    bool isCurrentlyReadingReservedRecord = false;

    while (getline(inFile, line)) {
        // User Details Section

        // Start Member Object
        if (line == "Member") {
            isCurrentlyReadingMember = true;
            currentMember = MemberC();
            currentMember.SetRole(Member);
        }
        
        // End Member Object

        if (line == "EndMember") {
            isCurrentlyReadingMember = false;
            memberList.push_back(currentMember);
        }

        // Fill in Member Objects with Data

        if (isCurrentlyReadingMember) {
            if (line.starts_with("User:")) currentMember.SetName(line.substr(5));
            if (line.starts_with("Email:")) currentMember.SetEmail(line.substr(6));
            if (line.starts_with("Password:")) currentMember.SetPassword(line.substr(9));
            if (line.starts_with("Username:")) currentMember.SetUsername(line.substr(9));
            if (line.starts_with("ID:")) currentMember.SetMemberID(stoi(line.substr(3)));
        }

        // Borrow Record Creation & Deletion

        if (isCurrentlyReadingMember) {
            if (line == "BorrowRecord") {
                isCurrentlyReadingBorrowRecord = true;
                currentBorrowRecord = BorrowedRecord();
            }

            if (line == "EndBorrowRecord") {
                isCurrentlyReadingBorrowRecord = false;
            }
        }

        // Fill in Borrow Record 

        if (isCurrentlyReadingMember && isCurrentlyReadingBorrowRecord) {
            if (line.starts_with("BookID:")) currentBorrowRecord.SetBook(stoi(line.substr(7)));
            if (line.starts_with("DateCreated:")) currentBorrowRecord.SetDateCreated(std::chrono::system_clock::from_time_t(std::stoll(line.substr(12))));
            if (line.starts_with("IsConfirmed:")) {
                if (line.substr(12) == "True") currentBorrowRecord.SetConfirmation(true);
                else if (line.substr(12) == "False") currentBorrowRecord.SetConfirmation(false);
            }
            if (line.starts_with("RecordID:")) currentBorrowRecord.SetRecordID(stoi(line.substr(9)));
            if (line.starts_with("DueDate:")) currentBorrowRecord.SetDueDate(std::chrono::system_clock::from_time_t(std::stoll(line.substr(8))));
            if (line.starts_with("DateReturned:")) currentBorrowRecord.SetDateReturned(std::chrono::system_clock::from_time_t(std::stoll(line.substr(13))));
            if (line.starts_with("Returned:")) {
                if (line.substr(9) == "True") currentBorrowRecord.SetReturned(true);
                else if (line.substr(9) == "False") currentBorrowRecord.SetReturned(false);
            }
        }
    }

    inFile.close();

    memberList.front().DisplayMemberDetails();

    cin.get();

    /*currentScreen = "Log-in Page";

    Title();

    cout << "Welcome to the High Wycombe Library System !\nPlease Log-in.\n";

    cout << "Username: ";

    string username;

    getline(cin, username);

    cin.get();
    */
}

int main()
{
    Login();
}

