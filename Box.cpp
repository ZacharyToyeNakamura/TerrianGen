#include <SFML/Graphics.hpp>
#include <iostream>

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
        sf::Font font;
        int fontSize;


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
            if (!font.loadFromFile("arial.ttf")) cout << "Error: Couldn't load arial.ttf in box class" << endl;
            fontSize = 10;
        }

        /**
         * Initializer with everything
        */
        Box(int x1, int y1, int x2, int y2, sf::Color color, string text, int size, string fontPath) {
            this->x1 = x1;
            this->y1 = y1;
            this->x2 = x2;
            this->y2 = y2;
            this->color = color;
            this->text = text;
            fontSize = size;
            if (!font.loadFromFile(fontPath)) cout << "Error: Couldn't load font from file path:" << fontPath << " in box class" << endl;
        }

        void setX1(int val) {
            x1 = val;
        }
        int getX1() {
            return x1;
        }

        /**
         * Draws the box onto the screen
         * @param window The GUI
        */
        void draw(sf::RenderWindow &window) {
            sf::RectangleShape rect(sf::Vector2f(x2-x1, y2-y1));
            rect.setPosition(x1, y1);
            rect.setFillColor(color);
            window.draw(rect);
            // Text part
            sf::Text msg(text, font, fontSize);
            msg.setPosition(x1,(y1+y2)/2);
            msg.setFillColor(sf::Color::Black);
            window.draw(msg);
        }
};
