#pragma once
#include <string>
#include <iostream>
#include <functional>
#include <vector>

using namespace std;

class HelperMethods {
public:
    static void ChangeColourText(int colourCode, string text) {
        HANDLE console_color;
        console_color = GetStdHandle(
            STD_OUTPUT_HANDLE);

        SetConsoleTextAttribute(console_color, colourCode);

        cout << text;

        SetConsoleTextAttribute(console_color, 7);
    }

    static void Title(string currentScreen)
    {
        system("CLS");

        ChangeColourText(10, "High Wycombe City Library System\n");
        ChangeColourText(10, "Current Screen : " + currentScreen + "\n");

        cout << "=====================================\n\n";
    }

    static void ErrorFormatting(exception& e) {
        system("CLS");
        ChangeColourText(4, "\n=========================");
        ChangeColourText(4, "\nError!\n");
        ChangeColourText(4, e.what());
        ChangeColourText(4, "\n=========================\n");
        cin.get();
    }

    static int TakeNumericInput(int maximum, string userInputSentence, string currentTitle, const vector<string>& optionTitles) {
        // Stores a valid numeric input and returns it

        bool foundValue = false;
        int finalValue = 0;

        do {
            system("CLS");

            Title(currentTitle);

            cout << userInputSentence + "\n";

            string userInput;

            int titleNumber = 0;
           
            for (string title : optionTitles) {
                titleNumber++;
                cout << to_string(titleNumber) + ") - " + title + "\n";
            }

            cout << "\nUser Choice: ";

            getline(cin, userInput);

            try {
                int input = stoi(userInput);

                if (input <= maximum && input > 0)
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

    static void CreateMenu(string title, string currentMenuQuestion, const vector<string>& optionTitles, const vector<function<void()>>& methods) {
        
        methods[TakeNumericInput(optionTitles.size(), currentMenuQuestion, title, optionTitles) - 1]();
    }
    
};