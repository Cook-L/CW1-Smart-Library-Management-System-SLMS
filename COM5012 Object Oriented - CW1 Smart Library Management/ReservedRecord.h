#pragma once
#include "Record.h"
#include <chrono>

class ReservedRecord : public Record {
private: 
	bool availible;
public:
	void SetAvailible(bool availiblePass) { availible = availiblePass; }
	void Record::DisplayDetails() {
		Record::DisplayDetails();
		cout << "Available: " << (availible ? "True" : "False") << "\n";

	}
};