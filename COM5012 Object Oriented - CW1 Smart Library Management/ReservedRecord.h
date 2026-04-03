#pragma once
#include "Record.h"
#include <chrono>

class ReservedRecord : public Record {
private: 
	bool availible;
public:
	void SetAvailible(bool availiblePass) { availible = availiblePass; }
	bool GetAvailible() { return availible; }

	void Record::DisplayDetails() {
		Record::DisplayDetails();
		cout << "Available: " << (availible ? "True" : "False") << "\n";

	}
};