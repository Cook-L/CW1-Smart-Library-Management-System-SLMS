#pragma once
#include <exception>
#include <iostream>
#include "HelperMethods.h"


class UserNotFoundException : public std::exception {
public: 
	const char* what() const noexcept override {
		return "\nNo Account was Found with That Username and Password.\nPlease Try Again.";
	}
};