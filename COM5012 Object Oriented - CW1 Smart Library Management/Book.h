#pragma once
#include <string>

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
public: 
	string GetTitle() {return title;}
	string GetAuthor() {return author;}
	string GetPublisher() {return publisher;}
	string GetDateReleased() {return dateReleased;}
	string GetGenre() {return genre;}
	int GetID() {return id;}
	int GetPageCount() {return pageCount;}
};
