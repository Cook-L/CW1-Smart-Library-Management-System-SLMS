#pragma once
#include "Record.h"
#include <chrono>
#include <optional>

class BorrowedRecord : public Record {
private: 
	chrono::system_clock::time_point dueDate;
	std::optional<chrono::system_clock::time_point> dateReturned;
	bool returned;
public: 
	chrono::system_clock::time_point GetDueDate() {
		return dueDate;
	}

	std::optional<chrono::system_clock::time_point> GetDateReturned() {
		return dateReturned;
	}

	bool GetReturned() {
		return returned;
	}

	void SetDueDate(chrono::system_clock::time_point dueDatePass) {
		dueDate = dueDatePass;
	}

	void SetDateReturned(chrono::system_clock::time_point returnDatePass) {
		dateReturned = returnDatePass;
	}

	void SetReturned(bool returnedPass) {
		returned = returnedPass;
	}
};