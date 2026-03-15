#pragma once
#include <string>
#include "Roles.h";

using namespace std;

class MemberC {
private:
	string firstName;
	string email;
	string password;
	string username;
	// Borrowed Books
	// Reserved Books
	Roles role;
	int memberID; 
public: 	
	string GetName() {
		return firstName; 
	}

	string GetUsername() {
		return username;
	}

	Roles GetRole() {
		return role;
	}

	int GetID() {
		return memberID;
	}

};