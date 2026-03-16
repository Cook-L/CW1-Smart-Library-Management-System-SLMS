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
#include "Admin.h";
#include "BorrowedRecord.h"
#include "ReservedRecord.h"

using namespace std; 

// Global Variables

string currentScreen;
MemberC currentLoggedInMember;
AdminC currentLoggedInAdmin;

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

void Login() {

    list<MemberC> memberList;
    list<AdminC> adminList;

    ifstream inFile;

    inFile.open("Users.txt");

    if (!inFile) {
        cout << "Unable to open file Users.txt";
        exit(1);
    }

    string line;

    MemberC currentMember;
    AdminC currentAdmin;

    BorrowedRecord currentBorrowRecord;
    ReservedRecord currentReservedRecord;

    bool isCurrentlyReadingMember = false;
    bool isCurrentlyReadingAdmin = false;
    bool isCurrentlyReadingLibrarian = false;

    bool isCurrentlyReadingBorrowRecord = false;
    bool isCurrentlyReadingReservedRecord = false;

    list<BorrowedRecord> borrowedRecordsTemp;
    list<ReservedRecord> reservedRecordsTemp;

    while (getline(inFile, line)) {

        // User Details Section

        while (!line.empty() && isspace(line.back())) line.pop_back();

        // Start Member Object
        if (line == "Member") {
            isCurrentlyReadingMember = true;
            currentMember = MemberC();
            currentMember.SetRole(Member);
            borrowedRecordsTemp.clear();
            reservedRecordsTemp.clear();
        }

        // Start Admin Object
        if (line == "Admin") {
            isCurrentlyReadingAdmin = true;
            currentAdmin = AdminC();
            currentAdmin.SetRole(Administrator);
            borrowedRecordsTemp.clear();
            reservedRecordsTemp.clear();
        }

        // End Member Object

        if (line == "EndMember") {
            isCurrentlyReadingMember = false;
            currentMember.SetBorrowedList(borrowedRecordsTemp);
            currentMember.SetReservedList(reservedRecordsTemp);
            memberList.push_back(currentAdmin);
        }

        // End Admin Object

        if (line == "EndAdmin") {
            isCurrentlyReadingAdmin = false;
            currentAdmin.SetBorrowedList(borrowedRecordsTemp);
            currentAdmin.SetReservedList(reservedRecordsTemp);
            adminList.push_back(currentAdmin);
        }

        // Fill in Member Objects with Data

        if (isCurrentlyReadingMember) {
            if (line.starts_with("User:")) currentMember.SetName(line.substr(5));
            if (line.starts_with("Email:")) currentMember.SetEmail(line.substr(6));
            if (line.starts_with("Password:")) currentMember.SetPassword(line.substr(9));
            if (line.starts_with("Username:")) currentMember.SetUsername(line.substr(9));
            if (line.starts_with("ID:")) currentMember.SetMemberID(stoi(line.substr(3)));
        }

        // Fill in Member Objects with Data

        if (isCurrentlyReadingAdmin) {
            if (line.starts_with("User:")) currentAdmin.SetName(line.substr(5));
            if (line.starts_with("Email:")) currentAdmin.SetEmail(line.substr(6));
            if (line.starts_with("Password:")) currentAdmin.SetPassword(line.substr(9));
            if (line.starts_with("Username:")) currentAdmin.SetUsername(line.substr(9));
            if (line.starts_with("ID:")) currentAdmin.SetMemberID(stoi(line.substr(3)));
        }

        // Borrow Record Creation & Deletion
        if (line == "BorrowRecord") {
            isCurrentlyReadingBorrowRecord = true;
            currentBorrowRecord = BorrowedRecord();
        }

        if (line == "EndBorrowRecord") {
            isCurrentlyReadingBorrowRecord = false;
            borrowedRecordsTemp.push_back(currentBorrowRecord);
        }

        // Fill in Borrow Record 

        if (isCurrentlyReadingBorrowRecord) {
            if (line.starts_with("BookID:")) currentBorrowRecord.SetBook(stoi(line.substr(7)));
            else if (line.starts_with("CreatedDate:"))
            {
                string lineValue = line.substr(12);
                if (lineValue != "null")
                    currentBorrowRecord.SetDateCreated(
                        chrono::system_clock::from_time_t(stoll(lineValue))
                    );
            }
            else if (line.starts_with("IsConfirmed:")) {
                if (line.substr(12) == "True") currentBorrowRecord.SetConfirmation(true);
                else if (line.substr(12) == "False") currentBorrowRecord.SetConfirmation(false);
            }
            else  if (line.starts_with("RecordID:")) currentBorrowRecord.SetRecordID(stoi(line.substr(9)));
            else if (line.starts_with("DueDate:")) currentBorrowRecord.SetDueDate(std::chrono::system_clock::from_time_t(std::stoll(line.substr(8))));
            else if (line.starts_with("DateReturned:"))
            {
                string lineValue = line.substr(13);
                if (lineValue != "null") currentBorrowRecord.SetDateReturned(chrono::system_clock::from_time_t(stoll(lineValue)));
            }
            else if (line.starts_with("Returned:")) {
                if (line.substr(9) == "True") currentBorrowRecord.SetReturned(true);
                else if (line.substr(9) == "False") currentBorrowRecord.SetReturned(false);
            }
        }

        // Reserve Record Creation & Deletion

        if (line == "ReservationRecord") {
            isCurrentlyReadingReservedRecord = true;
            currentReservedRecord = ReservedRecord();
        }

        if (line == "EndReservationRecord") {
            isCurrentlyReadingReservedRecord = false;
            reservedRecordsTemp.push_back(currentReservedRecord);
        }

        // Fill in Reserved Record 

        if (isCurrentlyReadingReservedRecord) {
            if (line.starts_with("BookID:")) currentReservedRecord.SetBook(stoi(line.substr(7)));
            else if (line.starts_with("CreatedDate:"))
            {
                string lineValue = line.substr(12);
                if (lineValue != "null") currentReservedRecord.SetDateCreated(chrono::system_clock::from_time_t(stoll(lineValue)));
            }
            else if (line.starts_with("IsConfirmed:")) {
                if (line.substr(12) == "True") currentReservedRecord.SetConfirmation(true);
                else if (line.substr(12) == "False") currentReservedRecord.SetConfirmation(false);
            }
            else  if (line.starts_with("RecordID:")) currentReservedRecord.SetRecordID(stoi(line.substr(9)));
            else if (line.starts_with("Availible:")) {
                if (line.substr(10) == "True") currentReservedRecord.SetAvailible(true);
                else if (line.substr(10) == "False") currentReservedRecord.SetAvailible(false);
            }
        }

    }

    inFile.close();

    currentScreen = "Log-in Page";

    bool foundValidCredentials = false;

    do {
        system("CLS");

        Title();

        cout << "Welcome to the High Wycombe Library System !\nPlease Log-in.\n";

        string username;
        string password;

        cout << "\nUsername: ";
        getline(cin, username);

        cout << "\nPassword: ";
        getline(cin, password);

        for (MemberC& member : memberList) {
            if (member.CheckLoginDetails(username, password)) {
                foundValidCredentials = true;
                currentLoggedInMember = member;
                break;
        
            }
        }

        for (AdminC& admin : adminList) {
            if (admin.CheckLoginDetails(username, password)) {
                foundValidCredentials = true;
                currentLoggedInAdmin = admin;
                break;

            }
        }

        if (!foundValidCredentials) {
            cout << "Invalid Username or Password.\nPlease Try Again.";
            cin.get();
        }

    } while (!foundValidCredentials);
}


int main()
{
    Login();

    system("CLS");

    currentScreen = "Menu";

    Title(); 

    string currentName;
    if (currentLoggedInMember.GetUsername().empty()) {
        if (currentLoggedInAdmin.GetUsername().empty()) {
            // Librarian Code Here
        }
        else currentName = currentLoggedInAdmin.GetName();
    }
    else currentName = currentLoggedInMember.GetName();

    cout << "Welcome back to the High Wycombe City Library " << currentName << ".";

    cin.get();
}

