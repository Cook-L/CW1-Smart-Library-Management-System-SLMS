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

list<Book> bookList;

// Function Declarations to avoid issue with calling functions before compile.

void AddBook();
void DeleteBook();
void QuitProgram();
void Login();
void HomePage();
void LogOut();
void BorrowBook();
void ReturnBook();

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

    cout << "High Wycombe Library Home Page Loading . . ." << endl;

    cin.get();


    CreateMenu(currentScreen, "What do you need to do?", optionTitles, methodReferences);

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
    Title("Return Book");

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

    int userChoice = TakeNumericInput(bookTitlesToReturn.size(), "What book are you returning today?", currentScreen, bookTitlesToReturn);
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

                HomePage();
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

    for (Book& book : bookList) {
        if (book.GetID() == finalValue) {
            book.SetStatus(Borrowed);
            break;
        }
        else 
        {
            BookNotFound error;
            ErrorFormatting(error);
            return;
        }
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
    
    
    if (SaveBookList(bookList)) cout << "\n\nBook Records Save Succesful";
    if (SaveUsersList( memberList, librarianList, adminList )) cout << "\n\nUser Records Save Succesful";

    cin.get();

    HomePage();
        
    
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

