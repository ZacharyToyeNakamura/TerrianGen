// A box that can only take specific input

#ifndef RESTRICTBOX_CPP
#define RESTRICTBOX_CPP

#include <SFML/Graphics.hpp>
#include "Box.cpp"

using namespace std;

class RestrictBox: public Box {
    public:
        string validType; // The type of input the box can take.


        /**
         * Initializer with everything
        */
        RestrictBox(int x1, int y1, int x2, int y2, sf::Color color, string label, string text, int size, string fontPath, string validType) : 
        Box(x1, y1, x2, y2, color, label, text, size, fontPath) {
            this->validType = "string";
            const string validTypes[] = {"long long", "double", "int"};
            for(string t: validTypes) {
                if(validType == t) this->validType = validType;
            }
        }

        RestrictBox (sf::Color color, string label, string text, int size, string fontPath, string validType) : 
        Box(color, label, text, size, fontPath) {
            this->validType = "string";
            const string validTypes[] = {"long long", "double", "int"};
            for(string t: validTypes) {
                if(validType == t) this->validType = validType;
            }
        }

        // Processes keyboard input from the user.
        // Returns 0 if the box is unselected, 1 if a character is entered, 2 if it's not selected, and 3 if input is invalid
        // 4 is unselected but the input didn't match the required type.
        int keyboardInput(sf::Event inputEvent) {
            if(!isSelected) return 2;
            int unicodeInput = inputEvent.text.unicode;
            if(unicodeInput >= 128) return 3;
            if(unicodeInput == DELETE_KEY) {
                if(text.size() > 0) { // If there are characters to delete
                    text = text.substr(0, text.length() - 1); // remove last character 
                }
            }
            else if(unicodeInput == ENTER) {
                if(validType == "int") {
                    try {
                        stoi(text);
                    }
                    catch (exception e) {
                        cancelSelect();
                        return 4;
                    }
                }
                else if (validType == "double") {
                    try {
                        stod(text);
                    }
                    catch (exception e) {
                        cancelSelect();
                        return 4;
                    }
                }
                else if (validType == "long long") {
                    try {
                        stoll(text);
                    }
                    catch (exception e) {
                        cancelSelect();
                        return 4;
                    }
                }
                else {
                    setSelected(false);
                    return 0;
                }
            }
            else if(unicodeInput == ESCAPE) {
                cancelSelect();
                return 0;
            }
            else text += static_cast<char>(unicodeInput);
            // DEBUGGING
            // cout << "Text is " + text << endl;

            return 1;
        }
};

#endif