#pragma once
#include <string>
#include <optional>
#include <list>

#include "Roles.h"
#include "BorrowedRecord.h"
#include "ReservedRecord.h"

using namespace std;

class MemberC {
private:
	string firstName;
	string email;
	string password;
	string username;
	list<BorrowedRecord> borrowedRecordList;
	list<ReservedRecord> reservedRecordList;
	Roles role;
	int memberID; 
public: 	

	MemberC() {
		role = Member;
	}

	string GetName() {
		return firstName; 
	}

	string GetUsername() {
		return username;
	}

	string GetPassword() {
		return password;
	}

	string GetEmail() {
		return email;
	}

	Roles GetRole() {
		return role;
	}

	int GetID() {
		return memberID;
	}

	list<BorrowedRecord> GetBorrowedRecords() {
		return borrowedRecordList;
	}

	list<ReservedRecord> GetReservedRecordsList() {
		return reservedRecordList;
	}

	void SetName(string firstNamePass) {firstName = firstNamePass;}
	void SetEmail(string emailPass) { email = emailPass; }
	void SetPassword(string passwordPass) { password = passwordPass; }
	void SetUsername(string usernamePass) { username = usernamePass; }
	void SetRole(Roles rolePass) { role = rolePass; }
	void SetMemberID(int idPass) { memberID = idPass; }
	void SetBorrowedList(list<BorrowedRecord> borrowedRecordListPass) { borrowedRecordList = borrowedRecordListPass; }
	void SetReservedList (list<ReservedRecord> reservedRecordList) { reservedRecordList = reservedRecordList; }
	void DisplayMemberDetails() {

		string roleString;

		switch (role) {
		case Member:
			roleString = "Member";
			break;
		case Administrator: 
			roleString = "Administrator";
			break;
		case Librarian:
			roleString = "Librarian";
			break;
		}


		cout << "Name: " << firstName << "\n";
		cout << "Email: " << email << "\n";
		cout << "Password: " << password << "\n";
		cout << "Username: " << username << "\n";
		cout << "Role: " << roleString << "\n";
		cout << "MemberID: " << memberID << "\n";
	}

	bool CheckLoginDetails(string usernamePass, string passwordPass) {
		if (username == usernamePass && password == passwordPass) return true;
		else return false;
	}

	
};