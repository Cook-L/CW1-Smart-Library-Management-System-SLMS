// COM5012 Object Oriented - CW1 Smart Library Management
// COM5012 SLMS System -> Main File
// Libraries 

#include <iostream>
#include <string>
#include <stdlib.h>
#include <stdexcept>
#include <limits> 
#include <list>
#include <fstream>
#include <vector>
#include <windows.h>

// Custom Classes
// User Classes
 
#include "Member.h"
#include "Admin.h"
#include "Librarian.h"

// Record Classes 

#include "BorrowedRecord.h"
#include "ReservedRecord.h"

// Helper Methods

#include "HelperMethods.h"
#include "CustomErrors.h"


// Enum to use

#include "CustomEnums.h"
using namespace std; 

// Global Variables

string currentScreen;

userType currentUserType = NoneSelected;

MemberC* currentLoggedInMember;
AdminC* currentLoggedInAdmin;
LibrarianC* currentLoggedInLibrarian;

list<MemberC> memberList;
list<AdminC> adminList;
list<LibrarianC> librarianList;

list<Book> bookList;

// Function Declarations to avoid issue with calling functions before compile.

void AddBook();
void DeleteBook();
void QuitProgram();
void Login();
void HomePage();
void IntroductionPath();
void LogOut();
void BorrowBook();
void ReturnBook();

void Login() {

    currentScreen = "Log-in Page";

    bool foundValidCredentials = false;

    do {
        system("CLS");

        HelperMethods::Title(currentScreen);

        cout << "Welcome to the High Wycombe Library System !\nPlease Log-in.\n";

        string username;
        string password;

        cout << "\nUsername: ";
        getline(cin, username);

        cout << "\nPassword: ";
        getline(cin, password);



        if (username.length() > 3 && password.length() > 3) {
            // Check Login Details against Members

            for (MemberC& member : memberList) {
                if (member.CheckLoginDetails(username, password)) {
                    foundValidCredentials = true;
                    currentLoggedInMember = &member;
                    currentUserType = MemberUser;
                    break;

                }
            }
            // Check Login Details against Admins
            for (AdminC& admin : adminList) {
                if (admin.CheckLoginDetails(username, password)) {
                    foundValidCredentials = true;
                    currentLoggedInAdmin = &admin;
                    currentUserType = AdminUser;
                    break;

                }
            }

            // Check Login Details against Librarians
            for (LibrarianC& lib : librarianList) {
                if (lib.CheckLoginDetails(username, password)) {
                    foundValidCredentials = true;
                    currentLoggedInLibrarian = &lib;
                    currentUserType = LibrarianUser;
                    break;

                }
            }
        }

        if (!foundValidCredentials) {
            cout << "\n";
            UserNotFoundException error;
            HelperMethods::ErrorFormatting(error);
        }

    } while (!foundValidCredentials);
}

void HomePage() {
    currentScreen = "Home Page";

    HelperMethods::Title(currentScreen);

    string currentName;

    vector<string> optionTitles = { "Add Book","Delete Book", "Quit" };
    vector<function<void()>> methodReferences = { AddBook, DeleteBook, QuitProgram };

    switch (currentUserType) {
    case AdminUser:
        currentName = currentLoggedInAdmin->GetName();

        // Set Admin Homepage Functions

        optionTitles = { "Return Book", "Borrow Book", "Log Out", "Quit" };
        methodReferences = { ReturnBook, BorrowBook, LogOut, QuitProgram };
        break;
    case LibrarianUser:
        currentName = currentLoggedInLibrarian->GetName();

        // Set Librarian Homepage Functions

        optionTitles = { "Add Book","Delete Book","Return Book", "Borrow Book", "Log Out", "Quit" };
        methodReferences = { AddBook, DeleteBook, ReturnBook, BorrowBook, LogOut, QuitProgram };
        break;
    case MemberUser:
        currentName = currentLoggedInMember->GetName();

        // Set Member Homepage Functions

        optionTitles = { "Return Book", "Borrow Book", "Log Out", "Quit" };
        methodReferences = { ReturnBook, BorrowBook, LogOut, QuitProgram };
        break;
    }

    cout << "Welcome back to the High Wycombe City Library " << currentName << ".";

    cin.get();

    HelperMethods::CreateMenu(currentScreen, "What do you need to do?", optionTitles, methodReferences);

    cin.get();
}

void IntroductionPath() {
    Login();
    HomePage();
}

void LogOut() {

    HelperMethods::Title("Logging Out");

    HelperMethods::ChangeColourText(4, "We will see you next time!");

    cin.get();

    currentLoggedInAdmin = nullptr;
    currentLoggedInLibrarian = nullptr;
    currentLoggedInMember = nullptr;

    currentUserType = NoneSelected;
    IntroductionPath();
}

void AddBook() {
    cout << "Add Book ";

    cin.get();
}

void DeleteBook() {
    cout << "Delete Book";

    cin.get();
}

void ReturnBook() 
{
    HelperMethods::Title("Return Book");

    vector<string> bookTitlesToReturn;
    list<int> borrowedBookID;
    list<Book> booksToReturn;


    switch (currentUserType) {
        case MemberUser:
            for (BorrowedRecord br : currentLoggedInMember->GetBorrowedRecords()) borrowedBookID.push_back(br.GetBook());
        break;
        case AdminUser:
            for (BorrowedRecord br : currentLoggedInAdmin->GetBorrowedRecords()) borrowedBookID.push_back(br.GetBook());
        break;
        case LibrarianUser:
            for (BorrowedRecord br : currentLoggedInLibrarian->GetBorrowedRecords()) borrowedBookID.push_back(br.GetBook());
        break;
    }

    for (Book book : bookList) {
        for (int id : borrowedBookID) if (book.GetID() == id) {
            bookTitlesToReturn.push_back(book.GetTitle());
        }
    }

    int userChoice = HelperMethods::TakeNumericInput(bookTitlesToReturn.size(), "What book are you returning today?", currentScreen, bookTitlesToReturn);
}

void BorrowBook() 
{
    HelperMethods::Title("Borrow Book");

    vector<string> booksToBorrow;

    for (Book book : bookList) if (book.GetStatus() == Availible) booksToBorrow.push_back(book.GetTitle());

    int userChoice = HelperMethods::TakeNumericInput(booksToBorrow.size(), "What book would you like to borrow?", currentScreen, booksToBorrow);

    // Save Choice to Database

    cin.get();
}

void QuitProgram() {

    HelperMethods::Title("Quitting Program");

    HelperMethods::ChangeColourText(4, "Exiting High Wycombe Library System . . .\n\n");

    exit(0);

    cin.get();
}

void LoadBookList() 
{
    ifstream inFile; 

    inFile.open("Books.txt");

    if (!inFile) {
        cout << "Unable to open file Books.txt";
        exit(1);
    }

    string line;

    Book currentBook;

    while (getline(inFile, line)) {
        while (!line.empty() && isspace(line.back())) line.pop_back();

        if (line == "Book") currentBook = Book();
        if (line == "EndBook") bookList.push_back(currentBook);

        if (line.starts_with("Title:")) currentBook.SetTitle(line.substr(6));
        else if (line.starts_with("Author:")) currentBook.SetAuthor(line.substr(7));
        else if (line.starts_with("Publisher:")) currentBook.SetPublisher(line.substr(10));
        else if (line.starts_with("Date:")) currentBook.SetDateReleased(line.substr(5));
        else if (line.starts_with("Genre:")) currentBook.SetGenre(line.substr(6));
        else if (line.starts_with("ID:")) currentBook.SetID(stoi(line.substr(3)));
        else if (line.starts_with("PageCount:")) currentBook.SetPageCount(stoi(line.substr(10)));
        else if (line.starts_with("Status:")) 
        {
            if (line.substr(7) == "Borrowed") currentBook.SetStatus(Borrowed);
            else if (line.substr(7) == "Availible") currentBook.SetStatus(Availible);
            else if (line.substr(7) == "Reserved") currentBook.SetStatus(Reserved);
        };

    }

    inFile.close();
}

void LoadUserData() {
    ifstream inFile;

    inFile.open("Users.txt");

    if (!inFile) {
        cout << "Unable to open file Users.txt";
        exit(1);
    }

    string line;

    MemberC currentMember;
    AdminC currentAdmin;
    LibrarianC currentLibrarian;

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
            borrowedRecordsTemp.clear();
            reservedRecordsTemp.clear();
        }
        // Start Admin Object
        else if (line == "Admin") {
            isCurrentlyReadingAdmin = true;
            currentAdmin = AdminC();
            borrowedRecordsTemp.clear();
            reservedRecordsTemp.clear();
        }
        // Start Librarian Object
        else if (line == "Librarian") {
            isCurrentlyReadingLibrarian = true;
            currentLibrarian = LibrarianC();
            borrowedRecordsTemp.clear();
            reservedRecordsTemp.clear();
        }
        // End Member Object
        else if (line == "EndMember") {
            isCurrentlyReadingMember = false;
            currentMember.SetBorrowedList(borrowedRecordsTemp);
            currentMember.SetReservedList(reservedRecordsTemp);
            memberList.push_back(currentMember);
        }
        // End Admin Object
        else if (line == "EndAdmin") {
            isCurrentlyReadingAdmin = false;
            currentAdmin.SetBorrowedList(borrowedRecordsTemp);
            currentAdmin.SetReservedList(reservedRecordsTemp);
            adminList.push_back(currentAdmin);
        }
        // End Librarian Object
        else if (line == "EndLibrarian") {
            isCurrentlyReadingLibrarian = false;
            currentLibrarian.SetBorrowedList(borrowedRecordsTemp);
            currentLibrarian.SetReservedList(reservedRecordsTemp);
            librarianList.push_back(currentLibrarian);
        }
        // Borrow Record Creation & Deletion
        else if (line == "BorrowRecord") {
            isCurrentlyReadingBorrowRecord = true;
            currentBorrowRecord = BorrowedRecord();
        }

        else if (line == "EndBorrowRecord") {
            isCurrentlyReadingBorrowRecord = false;
            borrowedRecordsTemp.push_back(currentBorrowRecord);
        }
        // Reserve Record Creation & Deletion

        else if (line == "ReservationRecord") {
            isCurrentlyReadingReservedRecord = true;
            currentReservedRecord = ReservedRecord();
        }
        else if (line == "EndReservationRecord") {
            isCurrentlyReadingReservedRecord = false;
            reservedRecordsTemp.push_back(currentReservedRecord);
        }
        // Fill in Member Objects with Data
        if (isCurrentlyReadingMember) {
            if (line.starts_with("User:")) currentMember.SetName(line.substr(5));
            else if (line.starts_with("Email:")) currentMember.SetEmail(line.substr(6));
            else if (line.starts_with("Password:")) currentMember.SetPassword(line.substr(9));
            else if (line.starts_with("Username:")) currentMember.SetUsername(line.substr(9));
            else if (line.starts_with("ID:")) currentMember.SetMemberID(stoi(line.substr(3)));
        }

        // Fill in Admin Objects with Data

        else  if (isCurrentlyReadingAdmin) {
            if (line.starts_with("User:")) currentAdmin.SetName(line.substr(5));
            else if (line.starts_with("Email:")) currentAdmin.SetEmail(line.substr(6));
            else if (line.starts_with("Password:")) currentAdmin.SetPassword(line.substr(9));
            else if (line.starts_with("Username:")) currentAdmin.SetUsername(line.substr(9));
            else if (line.starts_with("ID:")) currentAdmin.SetMemberID(stoi(line.substr(3)));
        }

        // Fill in Librarian Objects with Data

        else  if (isCurrentlyReadingLibrarian) {
            if (line.starts_with("User:")) currentLibrarian.SetName(line.substr(5));
            else if (line.starts_with("Email:")) currentLibrarian.SetEmail(line.substr(6));
            else if (line.starts_with("Password:")) currentLibrarian.SetPassword(line.substr(9));
            else if (line.starts_with("Username:")) currentLibrarian.SetUsername(line.substr(9));
            else if (line.starts_with("ID:")) currentLibrarian.SetMemberID(stoi(line.substr(3)));
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
}

int main()
{
    SetConsoleTitleA("High Wycombe Library System");
    LoadBookList();
    LoadUserData();
    IntroductionPath();
}

