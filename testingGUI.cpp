// A simple GUI I can test code in

#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>

#include <iostream>
#include <fstream>

#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <queue>
#include <string.h>

#include "constants.h"
#include "utilFunctions.h"
#include "FText.cpp"
#include "BoxGroup.cpp"

using namespace std;

#define ll long long


int main () {
    cout << "Loading. . . " << endl;
    srand(time(0));

    const int DEBUG_SIZE = 200;
    // create window
    sf::RenderWindow window(sf::VideoMode(WINDOW_SIZE_X + DEBUG_SIZE, WINDOW_SIZE_Y), "Testing GUI", sf::Style::Default);
    window.setActive(); // activate
    glClearDepth(1.f);
    glClearColor(0.f, 0.f, 0.f, 1.f); // color
    // Configure the viewport (the same size as the window)
    glViewport(0, 0, window.getSize().x, window.getSize().y);


    // Declaring the square that is used to draw the pixels
    sf::RectangleShape square(sf::Vector2f(100, 100)); // sets (x, y)

    // Start the loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;

                case sf::Event::Resized:
                    glViewport(0, 0, event.size.width, event.size.height);
                    break;

                case sf::Event::KeyPressed:
                    switch(event.key.code) {
                        case sf::Keyboard::Escape: // Escape key: exit
                            window.close();
                            break;

                    }
                break;
            }
        }
        // Reset the frame
        window.clear();

        // Draw stuff

        // end the current frame
        window.display();
    }



    return 0;
}