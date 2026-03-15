// COM5012 Object Oriented - CW1 Smart Library Management
// COM5012 SLMS System -> Main File


#include <iostream>
#include <String>
#include <stdlib.h>
#include <stdexcept>
#include <limits> 
#include <windows.h>
#include <list>
#include <fstream>
#include <vector>
#include "Member.h"

using namespace std; 

// Global Variables

string currentScreen;



void ChangeColourText(int colourCode, string text) {
    HANDLE console_color;
    console_color = GetStdHandle(
        STD_OUTPUT_HANDLE);

    SetConsoleTextAttribute(console_color, colourCode);

    cout << text;

    SetConsoleTextAttribute(console_color, 7);
}

void Title()
{
    ChangeColourText(10, "High Wycombe City Library System\n");
    ChangeColourText(10, "Current Screen : " + currentScreen + "\n");
    cout << "=====================================\n\n";
}

int TakeNumericInput(int maximum, string userInputSentence) {
    // Stores a valid numeric input and returns it

    bool foundValue = false;
    int finalValue = 0;

    do {
        system("CLS");

        Title();

        cout << userInputSentence;

        string userInput;

        getline(cin, userInput);

        try {
            int input = stoi(userInput);

            if (input < maximum && input > 0)
            {
                foundValue = true;
                finalValue = input;
            }
            else 
            {
                cout << "Please write an input between 0 and " << maximum;
                cin.get();
            }
        }
        catch (invalid_argument& e) {

            cout << "Be sure to write a valid, whole integer. ";
            cin.get();
        }
    } while (!foundValue);

    return finalValue;
}

bool Login() {

    list<MemberC> memberList;

    ifstream inFile; 

    inFile.open("Users.txt");

    if (!inFile) {
        cout << "Unable to open file Users.txt";
        exit(1);
    }

    string line;
    vector<string> copiedFile;


    while (getline(inFile, line, ' ')) {
        copiedFile.push_back(line);
    }

    for (string s : copiedFile) {
        if (s == "Member") {
            MemberC memberInstance;

            memberInstance.
        }
    }

    currentScreen = "Log-in Page";

    Title();

    cout << "Welcome to the High Wycombe Library System !\nPlease Log-in.\n";

    cout << "Username: ";

    string username;

    getline(cin, username);

    cin.get();
}

int main()
{
    Login();
}

