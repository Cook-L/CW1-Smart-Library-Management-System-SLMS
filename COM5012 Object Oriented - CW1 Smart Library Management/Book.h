#pragma once
#include <string>
#include "CustomEnums.h"

using namespace std;

class Book {
private: 
	string title; 
	string author;
	string publisher; 
	string dateReleased;
	string genre;
	int id;
	int pageCount;
	bookTracking status;

public: 
	string GetTitle() {return title;}
	string GetAuthor() {return author;}
	string GetPublisher() {return publisher;}
	string GetDateReleased() {return dateReleased;}
	string GetGenre() {return genre;}
	int GetID() {return id;}
	int GetPageCount() {return pageCount;}
	bookTracking GetStatus() { return status; }

	void SetTitle(string newTitle) { title = newTitle; }
	void SetAuthor(string newAuthor) { author = newAuthor; }
	void SetPublisher(string newPublisher) { publisher = newPublisher; }
	void SetDateReleased(string newDateReleased) { dateReleased = newDateReleased; }
	void SetGenre(string newGenre) { genre = newGenre; }
	void SetID(int newID) { id = newID; }
	void SetPageCount(int newPageCount) { pageCount = newPageCount; }
	void SetStatus(bookTracking newStatus) { status = newStatus; }
};
