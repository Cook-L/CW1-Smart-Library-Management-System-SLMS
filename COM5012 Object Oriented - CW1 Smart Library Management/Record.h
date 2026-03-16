#pragma once
#include <chrono>
#include "Book.h"
#include <iostream>

using namespace std;

class Record {
private:
	int bookID;
	chrono::system_clock::time_point dateCreated;
	bool isConfirmed;
	int recordID;
public:
	int GetBook() { return bookID; }
	chrono::system_clock::time_point GetDateCreated() { return dateCreated; }
	bool GetConfirmation() { return isConfirmed; }
	int GetRecordID() { return recordID; }

	void SetBook(int bookPass) { bookID = bookPass; }
	void SetDateCreated(chrono::system_clock::time_point dateCreatedPass) { dateCreated = dateCreatedPass; }
	void SetConfirmation(bool confirmedPass) { isConfirmed = confirmedPass; }
	void SetRecordID(int recordIDPass) { recordID = recordIDPass; }
};
