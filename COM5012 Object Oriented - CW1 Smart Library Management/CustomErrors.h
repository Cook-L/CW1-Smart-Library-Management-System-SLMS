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

class ErrorLoadingFile : public std::exception {
public:
	const char* what() const noexcept override {
		return "\nError when loading file to save.\n";
	}
};

class BookNotFound : public std::exception {
public: 
	const char* what() const noexcept override {
		return "\nNo book with given ID was found";
	}
};

class NoAvailibleBooks : public std::exception {
public: 
	const char* what() const noexcept override {
		return "\nNo books are currently availible for borrow";
	}
};