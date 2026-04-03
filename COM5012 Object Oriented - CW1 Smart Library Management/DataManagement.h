#pragma once
#include <fstream>
#include <iostream>

#include "Book.h"
#include "Librarian.h"
#include "Admin.h"
#include "Member.h"

extern vector<Book> bookList;
extern list<MemberC> memberList;
extern list<LibrarianC> librarianList;
extern list<AdminC> adminList;

using namespace std;

static void LoadBookList()
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

static void LoadUserData() {
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
            else if (line.starts_with("DueDate:")) currentBorrowRecord.SetDueDate(chrono::system_clock::from_time_t(std::stoll(line.substr(8))));
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

static bool SaveUsersList(const list<MemberC>& membersToSave, const list<LibrarianC>& librariansToSave, const list<AdminC>& adminsToSave) {
    try {
        ofstream inFile;

        inFile.open("Users.txt");

        if (!inFile) {
            throw ErrorLoadingFile();
            exit(1);
        }
        
        // Write Member File Information.

        for (MemberC member : membersToSave) {
            inFile << "Member" << endl;
            inFile << "User:" << member.GetName() << endl;
            inFile << "Email:" << member.GetEmail() << endl;
            inFile << "Password:" << member.GetPassword() << endl;
            inFile << "Username:" << member.GetUsername() << endl;
            inFile << "ID:" << member.GetID() << endl;

            // Write Borrow Records for Member

            for (BorrowedRecord br : member.GetBorrowedRecords()) {
                inFile << "BorrowRecord" << endl;
                
                inFile << "BookID:" << br.GetBook() << endl;
                inFile << "CreatedDate:" << to_string(chrono::system_clock::to_time_t(br.GetDateCreated())) << endl;
                inFile << "Confirmed:" << ((br.GetConfirmation()) ? "True" : "False") << endl;
                inFile << "RecordID:" << to_string(br.GetRecordID()) << endl;
                inFile << "DateReturned:" << ((!br.GetDateReturned().has_value()) ? "null" : to_string(chrono::system_clock::to_time_t(br.GetDateReturned().value()))) << endl;
                inFile << "Returned:" << ((br.GetReturned()) ? "True" : "False") << endl;
   
                inFile << "EndBorrowRecord" << endl;
            }
            
            // Write Reservation Records for Member

            for (ReservedRecord rr : member.GetReservedRecordsList()) {
                inFile << "ReservationRecord" << endl;

                inFile << "BookID:" << rr.GetBook() << endl;
                inFile << "CreatedDate:" << to_string(chrono::system_clock::to_time_t(rr.GetDateCreated())) << endl;
                inFile << "Confirmed:" << ((rr.GetConfirmation()) ? "True" : "False") << endl;
                inFile << "RecordID:" << to_string(rr.GetRecordID()) << endl;
                inFile << "Availible:" << ((rr.GetAvailible()) ? "True" : "False") << endl;
                inFile << "EndReservationRecord" << endl;
            }

            inFile << "EndMember" << endl;
        }

        // Write Librarian File Information

        for (LibrarianC librarian : librariansToSave) {
            inFile << "Librarian" << endl;
            inFile << "User:" << librarian.GetName() << endl;
            inFile << "Email:" << librarian.GetEmail() << endl;
            inFile << "Password:" << librarian.GetPassword() << endl;
            inFile << "Username:" << librarian.GetUsername() << endl;
            inFile << "ID:" << librarian.GetID() << endl;

            // Write Borrow Records for librarian

            for (BorrowedRecord br : librarian.GetBorrowedRecords()) {
                inFile << "BorrowRecord" << endl;

                inFile << "BookID:" << br.GetBook() << endl;
                inFile << "CreatedDate:" << to_string(chrono::system_clock::to_time_t(br.GetDateCreated())) << endl;
                inFile << "Confirmed:" << ((br.GetConfirmation()) ? "True" : "False") << endl;
                inFile << "RecordID:" << to_string(br.GetRecordID()) << endl;
                inFile << "DateReturned:" << ((!br.GetDateReturned().has_value()) ? "null" : to_string(chrono::system_clock::to_time_t(br.GetDateReturned().value()))) << endl;
                inFile << "Returned:" << ((br.GetReturned()) ? "True" : "False") << endl;

                inFile << "EndBorrowRecord" << endl;
            }

            // Write Reservation Records for librarian

            for (ReservedRecord rr : librarian.GetReservedRecordsList()) {
                inFile << "ReservationRecord" << endl;

                inFile << "BookID:" << rr.GetBook() << endl;
                inFile << "CreatedDate:" << to_string(chrono::system_clock::to_time_t(rr.GetDateCreated())) << endl;
                inFile << "Confirmed:" << ((rr.GetConfirmation()) ? "True" : "False") << endl;
                inFile << "RecordID:" << to_string(rr.GetRecordID()) << endl;
                inFile << "Availible:" << ((rr.GetAvailible()) ? "True" : "False") << endl;
                inFile << "EndReservationRecord:" << endl;
            }

            inFile << "EndLibrarian" << endl;
        }
    
        // Write Admin File Information

        for (AdminC admin : adminsToSave) {
            inFile << "Admin" << endl;
            inFile << "User:" << admin.GetName() << endl;
            inFile << "Email:" << admin.GetEmail() << endl;
            inFile << "Password:" << admin.GetPassword() << endl;
            inFile << "Username:" << admin.GetUsername() << endl;
            inFile << "ID:" << admin.GetID() << endl;

            // Write Borrow Records for admin

            for (BorrowedRecord br : admin.GetBorrowedRecords()) {
                inFile << "BorrowRecord" << endl;

                inFile << "BookID:" << br.GetBook() << endl;
                inFile << "CreatedDate:" << to_string(chrono::system_clock::to_time_t(br.GetDateCreated())) << endl;
                inFile << "Confirmed:" << ((br.GetConfirmation()) ? "True" : "False") << endl;
                inFile << "RecordID:" << to_string(br.GetRecordID()) << endl;
                inFile << "DateReturned:" << ((!br.GetDateReturned().has_value()) ? "null" : to_string(chrono::system_clock::to_time_t(br.GetDateReturned().value()))) << endl;
                inFile << "Returned:" << ((br.GetReturned()) ? "True" : "False") << endl;

                inFile << "EndBorrowRecord" << endl;
            }

            // Write Reservation Records for admin

            for (ReservedRecord rr : admin.GetReservedRecordsList()) {
                inFile << "ReservationRecord" << endl;

                inFile << "BookID:" << rr.GetBook() << endl;
                inFile << "CreatedDate:" << to_string(chrono::system_clock::to_time_t(rr.GetDateCreated())) << endl;
                inFile << "Confirmed:" << ((rr.GetConfirmation()) ? "True" : "False") << endl;
                inFile << "RecordID:" << to_string(rr.GetRecordID()) << endl;
                inFile << "Availible:" << ((rr.GetAvailible()) ? "True" : "False") << endl;
                inFile << "EndReservationRecord" << endl;
            }

            inFile << "EndAdmin" << endl;

        }

        inFile.close();
        return true;
      
    }
    catch (exception e) {
        ErrorFormatting(e);
        return false;
    }
}

static bool SaveBookList(const vector<Book>& booksToSave) 
{
    try {
        ofstream inFile;

        inFile.open("Books.txt");

        if (!inFile) {
            
            throw ErrorLoadingFile();
            exit(1);
        }

        for (Book book : booksToSave)
        {
            string status;

            switch (book.GetStatus()) {
            case Availible:
                status = "Availible";
                break;
            case Reserved:
                status = "Reserved";
                break;
            case Borrowed:
                status = "Borrowed";
                break;
            }

            inFile << "Book" << endl;
            inFile << "Title:" << book.GetTitle() << endl;
            inFile << "Author:" << book.GetAuthor() << endl;
            inFile << "Publisher:" << book.GetPublisher() << endl;
            inFile << "Date:" << book.GetDateReleased() << endl;
            inFile << "Genre:" << book.GetGenre() << endl;
            inFile << "ID:" << book.GetID() << endl;
            inFile << "PageCount:" << book.GetPageCount() << endl;
            inFile << "Status:" << status << endl;
            inFile << "EndBook" << endl;

            
        }

        inFile.close();

        return true;
    }
    catch (exception e) {
        ErrorFormatting(e);
        return false;
    }
}