#pragma once
#include "Settings.h"
#include "Member.h"

class AdminC : public MemberC {
private:
	Settings settings;
public:

	AdminC() {
		SetRole(Administrator);
	}

	bool UpdateUserFirstName(MemberC memberToUpdate,string name) {
		try {
			memberToUpdate.SetName(name);
			return true;
		} 
		catch (exception e) {
			cout << "\n" << e.what();
			cin.get();
			return false;
		}
	}

	bool UpdateUserPassword(MemberC memberToUpdate, string password) {
		try {
			memberToUpdate.SetPassword(password);
			return true;
		}
		catch (exception e) {
			cout << "\n" << e.what();
			cin.get();
			return false;
		}
	}

	bool UpdateUsername(MemberC memberToUpdate, string username) {
		try {
			memberToUpdate.SetPassword(username);
			return true;
		}
		catch (exception e) {
			cout << "\n" << e.what();
			cin.get();
			return false;
		}
	}

	bool UpdateEmail(MemberC memberToUpdate, string email) {
		try {
			memberToUpdate.SetPassword(email);
			return true;
		}
		catch (exception e) {
			cout << "\n" << e.what();
			cin.get();
			return false;
		}
	}
	
	bool SetLatePenalty(float newLatePenalty) {
		try {
			settings.SetLatePenalty(newLatePenalty);
			return true;
		} 
		catch (exception e) {
			cout << "\n" << e.what();
			cin.get();
			return false;
		}
	}

	bool SetBorrowingLimit(int newBorrowingLimit) {
		try {
			settings.SetBorrowingLimit(newBorrowingLimit);
			return true;
		}
		catch (exception e) {
			cout << "\n" << e.what();
			cin.get();
			return false;
		}
	}


};