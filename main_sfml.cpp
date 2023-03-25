// To compile
// g++ -c main_sfml.cpp -I src/include -DSFML_STATIC
// g++ main_sfml.o -o main_sfml -L src/lib -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lopengl32 -lwinmm -lgdi32
// main_sfml

#include <SFML/Window.hpp>
// #include <RenderWindow.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdio.h>
#include <cstdlib> // rand()
#include <vector> // dyanamically sized arrays for my population_map
#include <algorithm>
#include <ctime> // srand(time(0)) for true random
using namespace std;

const int MAPROWS = 10;
const int MAPCOLUMNS = 10;
const int growth_chance = 50; // chance for childern out of 100%
const int max_childern = 1; // random number of childern
const int movement_chance = 1; // chance for the population_map to move
const int percentage_movement = 50; // How much each portion of the population_map wants to move out of 100%. 100% Means that the population_map likes to move together
const int movement_options[8][2] = {    {-1, -1}, {-1, 0}, {-1, 1},
                                        { 0, -1},          { 0, 1},
                                        { 1, -1}, { 1, 0}, { 1, 1},}; // They can move in any of the 8 directions

struct blob{
    int life;
    int growth_chance;
    int max_childern;
    int movement_chance;
};

// predeclare func
void print_map(int[MAPROWS][MAPCOLUMNS]);
int population_spread(int[MAPROWS][MAPCOLUMNS],int [MAPROWS][MAPCOLUMNS], vector<pair<blob*,pair<int, int>>> &all_blobs);
void print_population_map(int[MAPROWS][MAPCOLUMNS]);
struct blob create_blob(int life, int growth_chance, int max_childern, int movement_chance);
// Just a template that may be changed
blob template_blob = create_blob(10, 20, 3, 75);

int population_map[MAPROWS][MAPCOLUMNS] = {}; // population_map[i][j] has the amount of blobs at (i, j)
int world_map[MAPROWS][MAPCOLUMNS] ={ {1, 1, 1, 1, 0, 0, 0, 0, 0, 0},
                                {1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
                                {1, 1, 0, 0, 0, 1, 1, 0, 0, 0},
                                {1, 1, 0, 1, 1, 1, 1, 0, 0, 0},
                                {1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
                                {0, 0, 1, 1, 1, 1, 1, 1, 1, 0},
                                {0, 0, 1, 1, 1, 1, 1, 1, 0, 0},
                                {0, 0, 1, 1, 1, 1, 1, 1, 0, 0},
                                {0, 0, 1, 1, 0, 0, 0, 0, 0, 0},
                                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},}; // hardcoded world_map
vector<pair<blob*,pair<int, int>>> all_blobs;

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
    sf::Texture land_tile, sea_tile; // Load a sprite to display
    sf::Font arial; // the font I will use for population numbers
    
    if (!land_tile.loadFromFile("C:\\Users\\zacha\\Documents\\vsgit\\greenSquare.png")) return EXIT_FAILURE; // Failed to load texture
    if (!sea_tile.loadFromFile("C:\\Users\\zacha\\Documents\\vsgit\\blueSquare.png")) return EXIT_FAILURE; // Failed to load texture
    if (!arial.loadFromFile("C:\\Users\\zacha\\Documents\\vsgit\\arial.ttf")) return EXIT_FAILURE; // Load in the font
    sf::Sprite land_tile_sprite(land_tile); // load the texture onto the sprite
    sf::Sprite sea_tile_sprite(sea_tile);
    // This is used to display the population number at world_map[i][j]
    sf::Text pop_count_number_display("0", arial); // The "sprite" like object for text
    pop_count_number_display.setFillColor(sf::Color::Black); // What colour is the font
    pop_count_number_display.setStyle(sf::Text::Bold | sf::Text::Underlined); // Set styles (eg. bold, underlined, strikethrough)


    // Setup first blob
    struct blob test_blob = template_blob;
    population_map[5][5] += 1;
    all_blobs.push_back(make_pair(&test_blob,make_pair(5, 5)));



    // Start the loop
    while (window.isOpen()) {
        // Process events
        sf::Event event;
        while (window.pollEvent(event)) {
            // Close window: exit
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            // Escape key: exit
            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape)) {
                window.close();
            }
            // Resize event: adjust the viewport
            if (event.type == sf::Event::Resized) {
                glViewport(0, 0, event.size.width, event.size.height);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                cout << "Right arrow key pressed!\n";
                population_spread(world_map, population_map, all_blobs);
            }
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
                    throw std::invalid_argument( "Received a non 1/0 value drawing the world_map" );
                }
                // Print the population at the location
                pop_count_number_display.setPosition((float)i*square_len, (float)j*square_len);
                pop_count_number_display.setString(to_string(population_map[i][j])); // to_string makes the int a string
                window.draw(pop_count_number_display);
            }
        }

        // Finally, display the rendered frame on screen
        window.display();
    }
    std::cout << "size of square is " << (float) (max(WINDOW_SIZE_X, WINDOW_SIZE_Y) / min(MAPROWS, MAPCOLUMNS)) / 500<< endl;

    std::cout << "Proccess finished" << endl;
    return EXIT_SUCCESS;
}


// This function iterates through the world_map and prints it. . .
void print_map(int world_map[MAPROWS][MAPCOLUMNS]){
    for (int i = 0; i < MAPROWS; i++){
        for (int j = 0; j < MAPCOLUMNS; j++){
            std::cout << world_map[i][j] << " ";
        }
        std::cout << endl;
    }
}
// Iterates through the populationmap and prints it
void print_population_map(int population_map[MAPROWS][MAPCOLUMNS]){
    for (int i = 0; i < MAPROWS; i++){
        for (int j = 0; j < MAPCOLUMNS; j++){
            std::cout << population_map[i][j] << " ";
        }
        std::cout << endl;
    }
}


// Creates a blob structure and returns it to a variable
struct blob create_blob(int life = 0, int growth_chance = 0, int max_childern = 0, int movement_chance = 0){
    struct blob temp_struct; // Why can't this be a *blob?
    temp_struct.life = life; // Turns this into -> but then it crashes
    temp_struct.growth_chance = growth_chance;
    temp_struct.max_childern = max_childern;
    temp_struct.movement_chance = movement_chance;

    return temp_struct;
};


// Return is for signaling failure or success
// 0 - means successfully done
//
// For each population_map, run random numbers to see if they spread, move or grow in number.
int population_spread(int world_map[MAPROWS][MAPCOLUMNS],int population_map[MAPROWS][MAPCOLUMNS], vector<pair<blob*,pair<int, int>>> &all_blobs) {
    // make a copy of the population_map to see where everything is moving
    // vector<blob*> new_population[MAPROWS][MAPCOLUMNS];
    // copy(&population_map[0][0], &population_map[0][0] + MAPROWS * MAPCOLUMNS, &new_population[0][0]); // copy a 2d array
    // What the next gen of blobs look like.
    vector<pair<blob*,pair<int, int>>> next_gen_blobs; // The new blobs to add
    cout << "running update" << endl;
    for (int i = 0; i < all_blobs.size(); i++){
        // cout << "Blob" << i << endl;
        int cur_row = all_blobs[i].second.first;
        int cur_col = all_blobs[i].second.second;
        blob* cur_blob = all_blobs[i].first;
        // reduce the life span of the blob by 1
        cur_blob->life -= 1;
        // printf("Life span\n");
        // std::cout << "life span" << cur_blob->life << endl;
        if (cur_blob->life == 0) {
            // remove blob from world_map and list, so don't add it to the next gen
            population_map[cur_row][cur_col] -= 1;
            continue;
        }
        int movement_seed = rand() % 100;
        if (world_map[cur_row][cur_col] == 1) { // It's land 
            // std::cout << "On land" << endl;
            int growth_seed = rand() % 100;
            // std::cout << "growth seed " << growth_seed << " growth chance " <<  cur_blob->growth_chance << endl;
            
            if (growth_seed <= cur_blob->growth_chance) { // The growth chance succeeded
                // random number of childern, could weight it later
                // gets a random number between 1 and the max_children, then creates that many blobs
                // currently new blobs can't move on the same tick they are born
                int children_seed = rand() % cur_blob->max_childern + 1;
                // std::cout << "Max children is " << cur_blob->max_childern << " num new children is " << children_seed << endl;
                for (int new_child = 0; new_child < children_seed; new_child++) {
                    // CHANGE template_blob to something later.
                    next_gen_blobs.push_back(make_pair(&template_blob, make_pair(cur_row, cur_col)));
                    population_map[cur_row][cur_col] += 1;
                    // add move chance right to make them move if needed
                }
            }
            // std::cout << "move seed " << movement_seed << " move chance " <<  cur_blob->movement_chance << endl;
            if (movement_seed <= cur_blob->movement_chance) { // movement chance succeeded
                // std::cout << "Movement Success" << endl;
                // copy all movement options to possible moves in 2 lines
                vector<pair<int, int>> possible_moves {}; // empty
                for (auto possible_movement : movement_options) {
                    int position_x = possible_movement[0] + cur_row;
                    int position_y = possible_movement[1] + cur_col;
                    // If NOT (x or y is out of the world_map) add the movement to possible_moves
                    if (!(position_x < 0 || position_x >= MAPROWS || position_y < 0 || position_y >= MAPCOLUMNS)) {
                        possible_moves.push_back(make_pair(position_x, position_y));
                    }
                }
                // Choose a random place to move to out of the valid moves
                int new_tile = rand() % possible_moves.size();
                all_blobs[i].second.first = possible_moves[new_tile].first; // reassigne the x, and y coordinates of it
                all_blobs[i].second.second = possible_moves[new_tile].second;
                // ERASE IT FROM THE world_map
                population_map[cur_row][cur_col] -= 1;
                // Add it to the new position on the world_map
                population_map[possible_moves[new_tile].first][possible_moves[new_tile].second] += 1;
                

            }
        }
        // Movement on water, can't reproduce on water.
        // Change later to use bfs to find all "continents / island ", then make them move towards a random shore that's not connected to their on continent.
        if (world_map[cur_row][cur_col] == 0) {
            if (movement_seed <= cur_blob->movement_chance) { // movement chance succeeded
                // copy all movement options to possible moves in 2 lines
                vector<pair<int, int>> possible_moves;
                for (auto possible_movement : movement_options) {
                    int position_x = possible_movement[0] + cur_row;
                    int position_y = possible_movement[1] + cur_col;
                    // If NOT (x or y is out of the world_map) add the movement to possible_moves
                    if (!(position_x < 0 || position_x >= MAPROWS || position_y < 0 || position_y >= MAPCOLUMNS)) 
                        possible_moves.push_back(make_pair(position_x, position_y));
                }
                // Choose a random place to move to out of the valid moves
                int new_tile = rand() % possible_moves.size();
                all_blobs[i].second.first = possible_moves[new_tile].first; // reassigne the x, and y cordnites of it
                all_blobs[i].second.second = possible_moves[new_tile].second;
                // ERASE IT FROM THE world_map
                population_map[cur_row][cur_col] -= 1;
                // Add it to the new position on the world_map
                population_map[possible_moves[new_tile].first][possible_moves[new_tile].second] += 1;
            }
        }
        // Add it to the next generation (eg it survived)
        next_gen_blobs.push_back(all_blobs[i]);
    }
    // Now update all_blobs to reflect what happened this tick.
    all_blobs = next_gen_blobs;
    // for (auto old_blob: next_gen_blobs) free(old_blob.first); // free the old blobs that are no longer neccassary (having memory leak issuses)
    cout << "Population is " << all_blobs.size() << "\n";

    return 0;
}