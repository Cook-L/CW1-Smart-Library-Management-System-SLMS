// COM5012 Object Oriented - CW1 Smart Library Management
// COM5012 SLMS System -> Main File
// Libraries 

#include <iostream>
#include <string>
#include <stdlib.h>
#include <stdexcept>
#include <limits> 
#include <list>
#include <vector>
#include <windows.h>
#include <algorithm>
#include <chrono>

// Custom Classes
// User Classes
 
#include "Member.h"
#include "Admin.h"
#include "Librarian.h"

// Record Classes 

#include "BorrowedRecord.h"
#include "ReservedRecord.h"

// Custom Methods, Helper Methods & Data Management

#include "HelperMethods.h"
#include "CustomErrors.h"
#include "DataManagement.h"

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

vector<Book> bookList;

// Function Declarations to avoid issue with calling functions before compile.

void AddBook();
void DeleteBook();
void QuitProgram();
void Login();
void HomePage();
void LogOut();
void BorrowBook();
void ReturnBook();
void SuccesfulSave();
void ReserveBook();

void Login() {

    currentScreen = "Log-in Page";

    bool foundValidCredentials = false;

    do {
        system("CLS");

        Title(currentScreen);

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
            ErrorFormatting(error);
        }

    } while (!foundValidCredentials);

    HomePage();
}

void HomePage() {
    currentScreen = "Home Page";

    Title(currentScreen);

    string currentName;

    vector<string> optionTitles;
    vector<function<void()>> methodReferences;

    switch (currentUserType) {
    case AdminUser:
        currentName = currentLoggedInAdmin->GetName();

        // Set Admin Homepage Functions

        optionTitles = { "Reserve Book", "Return Book", "Borrow Book", "Log Out", "Quit" };
        methodReferences = { ReserveBook , ReturnBook, BorrowBook, LogOut, QuitProgram };
        break;
    case LibrarianUser:
        currentName = currentLoggedInLibrarian->GetName();

        // Set Librarian Homepage Functions

        optionTitles = { "Add Book","Delete Book","Return Book", "Reserve Book" "Borrow Book", "Log Out", "Quit" };
        methodReferences = { AddBook, DeleteBook, ReturnBook, ReserveBook, BorrowBook, LogOut, QuitProgram };
        break;
    case MemberUser:
        currentName = currentLoggedInMember->GetName();

        // Set Member Homepage Functions

        optionTitles = { "Reserve Book", "Return Book", "Borrow Book", "Log Out", "Quit" };
        methodReferences = { ReserveBook , ReturnBook, BorrowBook, LogOut, QuitProgram };
        break;
    }

    cout << "High Wycombe Library Home Page Loading . . ." << endl;

    cin.get();


    CreateMenu(currentScreen, "What do you need to do?", optionTitles, methodReferences);

    HomePage();

}

void LogOut() {

    Title("Logging Out");

    ChangeColourText(4, "We will see you next time!");

    cin.get();

    currentLoggedInAdmin = nullptr;
    currentLoggedInLibrarian = nullptr;
    currentLoggedInMember = nullptr;

    currentUserType = NoneSelected;
    Login();
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
    
    if (currentUserType == AdminUser && currentLoggedInAdmin->GetBorrowedRecords().size() == 0 || currentUserType == LibrarianUser && currentLoggedInLibrarian->GetBorrowedRecords().size() == 0 || currentUserType == MemberUser && currentLoggedInMember->GetBorrowedRecords().size() == 0)
    {
        NoCurrentBorrowedBooks error;
        ErrorFormatting(error);
        return;
    }

    list<Book> borrowedBooks;
    list<int> borrowedBookIDs;

    if (currentUserType == AdminUser) {

        for (BorrowedRecord& br : currentLoggedInAdmin->GetBorrowedRecords()) 
        {
            for (Book& book : bookList) if (book.GetID() == br.GetBook()) 
            {
                borrowedBooks.push_back(book);
                borrowedBookIDs.push_back(book.GetID());
            }
        }
    }
    else if (currentUserType == LibrarianUser) {

        for (BorrowedRecord& br : currentLoggedInLibrarian->GetBorrowedRecords())
        {
            for (Book& book : bookList) if (book.GetID() == br.GetBook())
            {
                borrowedBooks.push_back(book);
                borrowedBookIDs.push_back(book.GetID());
            }
        }
    }
    else if (currentUserType == MemberUser) {

        for (BorrowedRecord& br : currentLoggedInMember->GetBorrowedRecords())
        {
            for (Book& book : bookList) if (book.GetID() == br.GetBook())
            {
                borrowedBooks.push_back(book);
                borrowedBookIDs.push_back(book.GetID());
            }
        }
    }

    currentScreen = "Return Books";

    bool foundValue = false;
    int finalValue = 0;

    do {
        system("CLS");

        Title(currentScreen);

        cout << "What is the ID of the book you want to return? (Type CANCEL if you no longer want to borrow a book)" << "\n";

        string userInput;

        for (Book bookOption : borrowedBooks) {
            cout << bookOption.GetID() << " - " << bookOption.GetTitle() << endl;
        }

        cout << "\nUser Choice: ";

        getline(cin, userInput);

        try {
            if (userInput == "CANCEL") {

                Title(currentScreen);

                cout << "Exiting back to Home Screen . . .";

                cin.get();
                return;
            }

            int input = stoi(userInput);

            if (find(borrowedBookIDs.begin(), borrowedBookIDs.end(), input) != borrowedBookIDs.end())
            {
                foundValue = true;
                finalValue = input;
            }
            else
            {
                cout << "Please write an appropriate ID number or write CANCEL to cancel" << endl;
                cin.get();
            }
        }
        catch (invalid_argument& e) {

            cout << "Be sure to write a valid, whole integer. ";
            cin.get();
        }
    } while (!foundValue);

    // Alter valid book

    for (Book& book : bookList) {
        if (book.GetID() == finalValue) book.SetStatus(Availible);
    }
    
    list<BorrowedRecord> alteredList;

    switch (currentUserType) {
    case AdminUser:
        for (BorrowedRecord& br : currentLoggedInAdmin->GetBorrowedRecords()) {
            if (br.GetBook() != finalValue) {
                alteredList.push_back(br);
            }
        }

        currentLoggedInAdmin->SetBorrowedList(alteredList);
    break;
    case MemberUser:
        for (BorrowedRecord& br : currentLoggedInMember->GetBorrowedRecords()) {
            if (br.GetBook() != finalValue) {
                alteredList.push_back(br);
            }
        }

        currentLoggedInMember->SetBorrowedList(alteredList);
        break;
    case LibrarianUser:
        for (BorrowedRecord& br : currentLoggedInLibrarian->GetBorrowedRecords()) {
            if (br.GetBook() != finalValue) {
                alteredList.push_back(br);
            }
        }

        currentLoggedInLibrarian->SetBorrowedList(alteredList);
        break;
    }

    SuccesfulSave();

}

void ReserveBook() 
{
    // Find all Currently Borrowed Books

    // Return if No Books are Currently Borrowed

    list<Book> borrowedBookList;
    list<int> borrowedBookIDs;

    // Make sure that the user can't reserve a book they already have.

    list<BorrowedRecord> userBorrowedBooks;

    switch (currentUserType) {
    case LibrarianUser:
        userBorrowedBooks = currentLoggedInLibrarian->GetBorrowedRecords();
        break;
    case AdminUser:
        userBorrowedBooks = currentLoggedInAdmin->GetBorrowedRecords();
        break;
    case MemberUser:
        userBorrowedBooks = currentLoggedInMember->GetBorrowedRecords();
        break;
    }

    for (Book& book : bookList) {
        if (book.GetStatus() == Borrowed) {
            bool notUserBorrowed = true;

            for (BorrowedRecord br : userBorrowedBooks) {
                if (book.GetID() == br.GetBook()) {
                    notUserBorrowed = false;
                    break; 
                }
            }

            if (notUserBorrowed) {
                borrowedBookIDs.push_back(book.GetID());
                borrowedBookList.push_back(book);
            }
        }
    }

    if (borrowedBookList.size() == 0) {
        NoReserveReadyBooks error;
        ErrorFormatting(error);
        return;
    }

    // Display List of Books to User & Prompt for Input

    bool foundValue = false;
    int finalValue = 0;

    do {
        system("CLS");

        Title(currentScreen);

        cout << "What is the ID of the book you want to reserve? (Type CANCEL if you no longer want to borrow a book)" << "\n";

        string userInput;

        for (Book book : borrowedBookList) {
            cout << book.GetID() << " - " << book.GetTitle() << endl;
        }

        cout << "\nUser Choice: ";

        getline(cin, userInput);

        try {
            if (userInput == "CANCEL") {

                Title(currentScreen);

                cout << "Exiting back to Home Screen . . .";

                cin.get();
                return;
            }

            int input = stoi(userInput);

            if (find(borrowedBookIDs.begin(), borrowedBookIDs.end(), input) != borrowedBookIDs.end())
            {
                foundValue = true;
                finalValue = input;
            }
            else
            {
                cout << "Please write an appropriate ID number or write CANCEL to cancel" << endl;
                cin.get();
            }
        }
        catch (invalid_argument& e) {

            cout << "Be sure to write a valid, whole integer. ";
            cin.get();
        }
    } while (!foundValue);

    for (Book& book : bookList) {
        if (book.GetID() == finalValue) book.SetStatus(Reserved);
    }

    // Find maximum borrowed Records by checking all availible records in all kinds of member

    list<BorrowedRecord> allBorrowedRecords;
    int maximumRecordID = 0;

    for (MemberC& member : memberList) {
        auto records = member.GetBorrowedRecords();
        allBorrowedRecords.insert(allBorrowedRecords.end(), records.begin(), records.end());
    }

    for (LibrarianC& librarian : librarianList) {
        auto records = librarian.GetBorrowedRecords();
        allBorrowedRecords.insert(allBorrowedRecords.end(), records.begin(), records.end());
    }

    for (AdminC& admin : adminList) {
        auto records = admin.GetBorrowedRecords();
        allBorrowedRecords.insert(allBorrowedRecords.end(), records.begin(), records.end());
    }

    for (BorrowedRecord& br : allBorrowedRecords) {
        if (br.GetRecordID() >= maximumRecordID) maximumRecordID = br.GetRecordID();
    }


    list<ReservedRecord> newReservedList;

    ReservedRecord rr = ReservedRecord();

    rr.SetRecordID(maximumRecordID + 1);
    rr.SetDateCreated(std::chrono::system_clock::now());
    rr.SetConfirmation(false);
    rr.SetBook(finalValue);

    switch (currentUserType) {
    case MemberUser:
        newReservedList = currentLoggedInMember->GetReservedRecordsList();
        newReservedList.push_back(rr);

        currentLoggedInMember->SetReservedList(newReservedList);
        break;
    case AdminUser:
        newReservedList = currentLoggedInAdmin->GetReservedRecordsList();
        newReservedList.push_back(rr);

        currentLoggedInAdmin->SetReservedList(newReservedList);
        break;
    case LibrarianUser:
        newReservedList = currentLoggedInLibrarian->GetReservedRecordsList();
        newReservedList.push_back(rr);

        currentLoggedInLibrarian->SetReservedList(newReservedList);
        break;
    }

    currentScreen = "Book Reserve - Save Status";

    SuccesfulSave();

}

void BorrowBook()
{
    vector<string> booksToBorrow;
    vector<int> bookIDs;
    
    for (Book& book : bookList) {
        if (book.GetStatus() == Availible) {
            booksToBorrow.push_back(book.GetTitle());
            bookIDs.push_back(book.GetID());
        }
    }

    if (booksToBorrow.size() == 0) 
    {
        NoAvailibleBooks error;
        ErrorFormatting(error);
        return;
    }

    currentScreen = "BorrowBook";

    vector<string> formattedOptions;

    for (int i = 0; i < booksToBorrow.size(); i++) formattedOptions.push_back("ID: " + to_string(bookIDs[i]) + " Title: " + booksToBorrow[i]);

    bool foundValue = false;
    int finalValue = 0;

    do {
        system("CLS");

        Title(currentScreen);

        cout << "What is the ID of the book you want to borrow? (Type CANCEL if you no longer want to borrow a book)" << "\n";

        string userInput;

        for (string option : formattedOptions) {
            cout << option << endl;
        }

        cout << "\nUser Choice: ";

        getline(cin, userInput);

        try {
            if (userInput == "CANCEL") {

                Title(currentScreen);

                cout << "Exiting back to Home Screen . . .";

                cin.get();
                return;
            }

            int input = stoi(userInput);

            if (find(bookIDs.begin(),bookIDs.end(),input) != bookIDs.end())
            {
                foundValue = true;
                finalValue = input;
            }
            else
            {
                cout << "Please write an appropriate ID number or write CANCEL to cancel" << endl;
                cin.get();
            }
        }
        catch (invalid_argument& e) {

            cout << "Be sure to write a valid, whole integer. ";
            cin.get();
        }
    } while (!foundValue);

    bool foundBook = false;

    for (Book& book : bookList) {
        if (book.GetID() == finalValue) {
            book.SetStatus(Borrowed);
            foundBook = true;
            break;
        }
    }

    if (!foundBook) {
        BookNotFound error;
        ErrorFormatting(error);
        return;
    }

    // Find maximum borrowed Records by checking all availible records in all kinds of member

    list<BorrowedRecord> allBorrowedRecords;
    int maximumRecordID = 0;
    
    for (MemberC& member : memberList) {
        auto records = member.GetBorrowedRecords();
        allBorrowedRecords.insert(allBorrowedRecords.end(), records.begin(), records.end());
    }

    for (LibrarianC& librarian : librarianList) {
        auto records = librarian.GetBorrowedRecords();
        allBorrowedRecords.insert(allBorrowedRecords.end(), records.begin(), records.end());
    }

    for (AdminC& admin : adminList) {
        auto records = admin.GetBorrowedRecords();
        allBorrowedRecords.insert(allBorrowedRecords.end(), records.begin(), records.end());
    }

    for (BorrowedRecord& br : allBorrowedRecords) {
        if (br.GetRecordID() >= maximumRecordID) maximumRecordID = br.GetRecordID();
    }


    list<BorrowedRecord> newBorrowedList;

    BorrowedRecord br = BorrowedRecord();

    br.SetRecordID(maximumRecordID + 1);
    br.SetDateCreated(std::chrono::system_clock::now());
    br.SetConfirmation(false);
    br.SetBook(finalValue);
    br.SetDueDate(std::chrono::system_clock::now() + std::chrono::days(5));
    br.SetReturned(false);

    switch (currentUserType) {
    case MemberUser:
        newBorrowedList = currentLoggedInMember->GetBorrowedRecords();
        newBorrowedList.push_back(br);

        currentLoggedInMember->SetBorrowedList(newBorrowedList);
        break;
    case AdminUser:
        newBorrowedList = currentLoggedInAdmin->GetBorrowedRecords();
        newBorrowedList.push_back(br);

        currentLoggedInAdmin->SetBorrowedList(newBorrowedList);
        break;
    case LibrarianUser:
        newBorrowedList = currentLoggedInLibrarian->GetBorrowedRecords();
        newBorrowedList.push_back(br);

        currentLoggedInLibrarian->SetBorrowedList(newBorrowedList);
        break;
    }
    
    currentScreen = "Book Borrow - Save Status";
    
    SuccesfulSave();
}

void SuccesfulSave() {

    Title(currentScreen);

    if (SaveBookList(bookList) && SaveUsersList(memberList, librarianList, adminList))
    {
        ChangeColourText(32, "==============\n\n");

        ChangeColourText(32, "Save Succesful!\n");

        ChangeColourText(32, "Returning to Home Page\n\n");

        ChangeColourText(32, "==============\n");
    }

    cin.get();
}

void QuitProgram() {

    Title("Quitting Program");

    ChangeColourText(4, "Exiting High Wycombe Library System . . .\n\n");

    exit(0);

    cin.get();
}

int main()
{
    SetConsoleTitleA("High Wycombe Library System");
    LoadBookList();
    LoadUserData();
    Login();
}

