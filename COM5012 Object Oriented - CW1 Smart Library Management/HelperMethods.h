#pragma once
#include <String>
#include <iostream>

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
        ChangeColourText(10, "High Wycombe City Library System\n");
        ChangeColourText(10, "Current Screen : " + currentScreen + "\n");
        cout << "=====================================\n\n";
    }

    static int TakeNumericInput(int maximum, string userInputSentence, string currentTitle) {
        // Stores a valid numeric input and returns it

        bool foundValue = false;
        int finalValue = 0;

        do {
            system("CLS");

            Title(currentTitle);

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
};