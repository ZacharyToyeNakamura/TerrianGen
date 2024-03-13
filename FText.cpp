#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;

/**
 * Just text in a class form. May be useful later.
 * 
*/
class FText {
    public: // Public variables for now (bad practice though)
        int x;
        int y;
        string text;
        sf::Font font;
        int fontSize;
        sf::Color textColor;

        /**
         * Basic Initializer
        */
        FText(int x, int y, string text, string fontPath) {
            this->x = x;
            this->y = y;
            this->text = text;
            if (!font.loadFromFile(fontPath)) cout << "Error: Couldn't load font from file path:" << fontPath << " in box class" << endl;
            fontSize = 8;
            textColor = sf::Color::White;
        }

        // Draws the text to the window given
        void draw(sf::RenderWindow &window) {
            sf::Text msg(text, font, fontSize);
            msg.setPosition(x, y);
            msg.setFillColor(textColor);
            window.draw(msg);
        }
};