#ifndef BOX_CPP
#define BOX_CPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <ctime>
#include <chrono>

#include "constants.h"

using namespace std;

/**
 * This class hold the parameters for a basic box, which can be drawn to the screen. (Has text)
 * 
 * @param x1, y1 The top left coordinate of the box
 * @param x2, y2 The bottom right coordinate of the box
 * @param color The color of the box (an sf::color object)
 * @param text The text that is in the box (Doesn't prevent overflow)
 * @param font The font of the text.
 * 
*/
class Box {
    
    public:
        int x1;
        int y1;
        int x2;
        int y2;
        sf::Color color;
        string text;
        string oldText;
        string label;
        sf::Font font;
        int fontSize;
        bool isSelected;


        /**
         * Initializer with text being nothing, and font being arial.
        */
        Box(int x1, int y1, int x2, int y2, sf::Color color) {
            this->x1 = x1;
            this->y1 = y1;
            this->x2 = x2;
            this->y2 = y2;
            this->color = color;
            text = "";
            label = "";
            if (!font.loadFromFile("arial.ttf")) cout << "Error: Couldn't load arial.ttf in box class" << endl;
            fontSize = 10;
            isSelected = false;
        }

        /**
         * Initializer with everything
        */
        Box(int x1, int y1, int x2, int y2, sf::Color color, string label, string text, int size, string fontPath) {
            this->x1 = x1;
            this->y1 = y1;
            this->x2 = x2;
            this->y2 = y2;
            this->color = color;
            this->text = text;
            oldText = text;
            this->label = label;
            fontSize = size;
            if (!font.loadFromFile(fontPath)) cout << "Error: Couldn't load font from file path:" << fontPath << " in box class" << endl;
            isSelected = false;
        }

        /**
         * Initializer without coords or size
        */
        Box(sf::Color color, string label, string text, int size, string fontPath) {
            x1 = 0;
            y1 = 0;
            x2 = 0;
            y2 = 0;
            this->color = color;
            this->text = text;
            oldText = text;
            this->label = label;
            fontSize = size;
            if (!font.loadFromFile(fontPath)) cout << "Error: Couldn't load font from file path:" << fontPath << " in box class" << endl;
            isSelected = false;
        }
        

        void setSelected(bool toggle) {
            isSelected = toggle;
            if(!toggle) oldText = text;
        }


        void cancelSelect() {
            text = oldText;
            setSelected(false);
        }


        bool clickedScan(sf::Vector2i mousePos) {
            int mouseX = mousePos.x;
            int mouseY = mousePos.y;
            if(x1 <= mouseX && mouseX <= x2 && y1 <= mouseY && mouseY <= y2) {
                isSelected = true;
                oldText = text;
                return true;
            }
            return false;
        }

        // Processes keyboard input from the user.
        // Returns 0 if the box is unselected, 1 if a character is entered, 2 if it's not selected, and 3 if input is invalid
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
                setSelected(false);
                return 0;
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
        // Returns time in seconds
        int timeInSec() {
            return time(NULL);
        }
        // Returns time in miliseconds
        long long timeInMSec() {
            return chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
        }


        /**
         * Draws the box onto the screen
         * @param window The GUI
        */
        void draw(sf::RenderWindow &window) {
            const int TEXT_PADDING_L = 2;
            sf::RectangleShape rect(sf::Vector2f(x2-x1, y2-y1));
            rect.setPosition(x1, y1);
            rect.setFillColor(color);
            window.draw(rect);
            // Text part
            sf::Text msg(label + text, font, fontSize);
            if(isSelected) { // when selected blink
                //if(timeInMSec() % 20 >= 10) // <- Doesn't look good, maybe because my system is slow
                if(timeInSec() % 2) msg.setString(msg.getString() + "_"); // <- slightly too slow. . .
            }
            msg.setPosition(x1 + TEXT_PADDING_L,(y1+y2)/2);
            msg.setFillColor(sf::Color::Black);
            window.draw(msg);
        }

        void destory() {
            // Do stuff here once code is refactored to use sf::Retangle and sf::Text
        }
};

#endif