// To compile
// g++ -c main_sfml.cpp -I src/include -DSFML_STATIC
// g++ main_sfml.o -o main_sfml -L src/lib -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lopengl32 -lwinmm -lgdi32
// main_sfml

#include <SFML/Window.hpp>
// #include <RenderWindow.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>

#include <bits/stdc++.h> // the everything library
using namespace std;

const int MAPROWS = 10;
const int MAPCOLUMNS = 10;
int population_map[MAPROWS][MAPCOLUMNS] = {};
int world_map[MAPROWS][MAPCOLUMNS] = {  {1, 1, 1, 1, 0, 0, 0, 0, 0, 0},
                                        {1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
                                        {1, 1, 0, 0, 0, 1, 1, 0, 0, 0},
                                        {1, 1, 0, 1, 1, 1, 1, 0, 0, 0},
                                        {1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
                                        {0, 0, 1, 1, 1, 1, 1, 1, 1, 0},
                                        {0, 0, 1, 1, 1, 1, 1, 1, 0, 0},
                                        {0, 0, 1, 1, 1, 1, 1, 1, 0, 0},
                                        {0, 0, 1, 1, 0, 0, 0, 0, 0, 0},
                                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},}; // hardcoded world_map
const int WINDOW_SIZE = 240;
const int ASPECT_RAITO_X = 4;
const int ASPECT_RAITO_Y = 3;
const int WINDOW_SIZE_X = ASPECT_RAITO_X * WINDOW_SIZE;
const int WINDOW_SIZE_Y = ASPECT_RAITO_Y * WINDOW_SIZE;

int main () {
    std::cout << "Working!" << endl; // debug
    // create window
    sf::RenderWindow window(sf::VideoMode(WINDOW_SIZE_X, WINDOW_SIZE_Y), "Population Sim", sf::Style::Default);
    window.setActive(); // activate
    // Set the color and depth clear values
    glClearDepth(1.f);
    glClearColor(0.f, 0.f, 0.f, 1.f); // color
    // Configure the viewport (the same size as the window)
    glViewport(0, 0, window.getSize().x, window.getSize().y);

    // Set up textures for the land and sea.
    sf::Texture red_circle, land_tile, sea_tile; // Load a sprite to display
    sf::Font arial; // the font I will use for population numbers
    
    if (!red_circle.loadFromFile("C:\\Users\\zacha\\Documents\\vsgit\\redcircle.png")) return EXIT_FAILURE; // Failed to load texture
    if (!land_tile.loadFromFile("C:\\Users\\zacha\\Documents\\vsgit\\greenSquare.png")) return EXIT_FAILURE; // Failed to load texture
    if (!sea_tile.loadFromFile("C:\\Users\\zacha\\Documents\\vsgit\\blueSquare.png")) return EXIT_FAILURE; // Failed to load texture
    if (!arial.loadFromFile("C:\\Users\\zacha\\Documents\\vsgit\\arial.ttf")) return EXIT_FAILURE; // Load in the font
    sf::Sprite circle_sprite(red_circle);   
    sf::Sprite land_tile_sprite(land_tile); // load the texture onto the sprite
    sf::Sprite sea_tile_sprite(sea_tile);
    // This is used to display the population number at world_map[i][j]
    sf::Text pop_count_number_display("0", arial); // The "sprite" like object for text
    pop_count_number_display.setFillColor(sf::Color::Black); // What colour is the font
    pop_count_number_display.setStyle(sf::Text::Bold | sf::Text::Underlined); // Set styles (eg. bold, underlined, strikethrough)

    circle_sprite.setPosition(10, 50); // absolute position
    circle_sprite.setScale(2, 2); 
    // Start the loop
    while (window.isOpen()) {
        // Process events
        sf::Event event;
        while (window.pollEvent(event)) {
            // Close window: exit
            if (event.type == sf::Event::Closed)
                window.close();

            // Escape key: exit
            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
                window.close();

            // Resize event: adjust the viewport
            if (event.type == sf::Event::Resized)
                glViewport(0, 0, event.size.width, event.size.height);
        }
        window.clear();
        // Draw the sprite
        float square_scale = (float)(min(WINDOW_SIZE_X, WINDOW_SIZE_Y) / min(MAPROWS, MAPCOLUMNS)) / 500; // How much to scale the squares by
        float square_len = min(WINDOW_SIZE_X, WINDOW_SIZE_Y) / min(MAPROWS, MAPCOLUMNS); // Where the next square should start
        sea_tile_sprite.setScale(square_scale, square_scale); // set the squares to scale
        land_tile_sprite.setScale(square_scale, square_scale);
        pop_count_number_display.setCharacterSize(12); // TODO: Update this to a equation so it scales
        for (int i = 0; i < MAPROWS; i++) {
            for(int j = 0; j < MAPCOLUMNS; j++) { 
                if (world_map[i][j] == 0) { // it's a sea tile
                    sea_tile_sprite.setPosition((float)i*square_len, (float)j*square_len); // set it's position and draw
                    window.draw(sea_tile_sprite);
                }
                else if (world_map[i][j] == 1) { // it's a land tile
                    land_tile_sprite.setPosition((float)i*square_len, (float)j*square_len);
                    window.draw(land_tile_sprite);
                }
                else { // Shouldn't happen but incase it does
                    cout << "ERROR !!! world_map corrupted !!!" << endl;
                    throw std::invalid_argument( "received a non 1/0 value drawing the map" );
                }
                // Print the population at the location
                pop_count_number_display.setPosition((float)i*square_len, (float)j*square_len);
                pop_count_number_display.setString(to_string(population_map[i][j])); // to_string makes the int a string
                window.draw(pop_count_number_display);
            }
        }
        // window.draw(circle_sprite);
        // window.draw(land_tile_sprite);
        // window.draw(sea_tile_sprite);

        // Finally, display the rendered frame on screen
        window.display();
    }
    std::cout << "size of square is " << (float) (max(WINDOW_SIZE_X, WINDOW_SIZE_Y) / min(MAPROWS, MAPCOLUMNS)) / 500<< endl;

    std::cout << "Proccess finished" << endl;
    return EXIT_SUCCESS;
}