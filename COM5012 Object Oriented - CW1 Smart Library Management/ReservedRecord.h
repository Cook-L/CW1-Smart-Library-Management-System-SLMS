#pragma once
#include "Record.h"
#include <chrono>

class ReservedRecord : public Record {
private: 
	bool availible;
public:
	void SetAvailible(bool availiblePass) { availible = availiblePass; }
};