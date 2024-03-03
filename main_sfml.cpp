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
#include "functions.cpp"
#include "terrianGen.cpp"
using namespace std;

const int INF = 0x3f3f3f3f;

// Change these variables
const int MAPROWS = 200;
const int MAPCOLUMNS = 200;
const int growth_chance = 50; // chance for childern out of 100%
const int max_childern = 1; // random number of childern
const int movement_chance = 1; // chance for the population_map to move
const int percentage_movement = 50; // How much each portion of the population_map wants to move out of 100%. 100% Means that the population_map likes to move together
const int movement_options[8][2] = {    {-1, -1}, {-1, 0}, {-1, 1},
                                        { 0, -1},          { 0, 1},
                                        { 1, -1}, { 1, 0}, { 1, 1},}; // They can move in any of the 8 directions

const int WINDOW_SIZE = 240;
const int ASPECT_RAITO_X = 4;
const int ASPECT_RAITO_Y = 3;
const int WINDOW_SIZE_X = ASPECT_RAITO_X * WINDOW_SIZE;
const int WINDOW_SIZE_Y = ASPECT_RAITO_Y * WINDOW_SIZE;

// int world_map[MAPROWS][MAPCOLUMNS] ={   {1, 1, 1, 1, 0, 0, 0, 0, 0, 0},
//                                         {1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
//                                         {1, 1, 0, 0, 0, 1, 1, 0, 0, 0},
//                                         {1, 1, 0, 1, 1, 1, 1, 0, 0, 0},
//                                         {1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
//                                         {0, 0, 1, 1, 1, 1, 1, 1, 1, 0},
//                                         {0, 0, 1, 1, 1, 1, 1, 1, 0, 0},
//                                         {0, 0, 1, 1, 1, 1, 1, 1, 0, 0},
//                                         {0, 0, 1, 1, 0, 0, 0, 0, 0, 0},
//                                         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},}; // hardcoded world_map
int world_map[MAPROWS][MAPCOLUMNS]; // non-hard coded
int food_map[MAPROWS][MAPCOLUMNS]; // Where all the food is


struct blob {
    int life; // How long the blob will live for
    int growth_chance; // Chance that the blob reproduces, creating anywhere from (1 - max_children) blobs on the tile
    int max_childern; // Maximum number of children it can have
    int speed; // How many tiles it will move if it does
    int movement_chance; // Perecentage chance to move
    pair<int, int> movement_direction; // Holds the direction it currently moving it, so it doesn't just spin in circles
    int movement_deviation_chance; // Chance to change paths
    int sense; // How far the blob can see
    int affiliation; // Which "team/group" the blob is part of
    
    // Overloads the == operator to compare this blob to another blob and return if they are exactly the same
    bool operator==(const blob& other_blob) {
        if (life == other_blob.life &&
            growth_chance == other_blob.growth_chance &&
            max_childern == other_blob.max_childern &&
            movement_chance == other_blob.movement_chance &&
            movement_direction == other_blob.movement_direction &&
            movement_deviation_chance == other_blob.movement_deviation_chance &&
            sense == other_blob.sense &&
            affiliation == other_blob.affiliation
            ) {
                return true;
            }
        return false;
    }
};
vector<pair<blob*,pair<int, int>>> all_blobs;
vector<blob*> location_map[MAPROWS][MAPCOLUMNS]; // stores each blob in a vecotr

// Takes in a vector of blobs (from location_map[x][y]) and removes a specific blob, Time Complexity: O(2N)
// Returns 0 for succecful detection and removal
// Returns 1 if it couldn't remove anything
int remove_by_value(vector<blob*>& list_of_blobs, struct blob blob_to_remove) {
    for (int i = 0; i < list_of_blobs.size(); i++) {
        if (&blob_to_remove == list_of_blobs[i]) {
            list_of_blobs.erase(list_of_blobs.begin() + i); // erases by index
            return 0; 
        }
    }
    return 1;
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
// Degraded
void print_population_map(vector<blob*> location_map[MAPROWS][MAPCOLUMNS]){
    for (int i = 0; i < MAPROWS; i++){
        for (int j = 0; j < MAPCOLUMNS; j++){
            std::cout << location_map[i][j].size() << " ";
        }
        std::cout << endl;
    }
}


// Creates a blob structure and returns it to a variable
struct blob create_blob(int life = 0, int growth_chance = 0, int max_childern = 0, int speed = 0, int movement_chance = 0, int movement_deviation_chance = 50, int sense = 0, int affiliation = 0){
    struct blob temp_struct; // Why can't this be a *blob?
    temp_struct.life = life; // Turns this into -> but then it crashes
    temp_struct.growth_chance = growth_chance;
    temp_struct.max_childern = max_childern;
    temp_struct.speed = speed;
    temp_struct.movement_chance = movement_chance;
    temp_struct.movement_deviation_chance = movement_deviation_chance;
    temp_struct.sense = sense;
    temp_struct.affiliation =  affiliation;

    return temp_struct;
};

// Just a template that may be changed
blob template_blob = create_blob(1000, 20, 3, 1, 99, 10, 10, 0);


// Does sensing for a blob, letting it move towards food optimally if it's within a diamond shaped range (of size blob.sense)
// All blobs have a minimum sense of 0, checking just (0, 0), 1 would check (+-1, 0) and (0, +- 1). Making a diamond
// It will move outwards until it finds the "ring" in the diamond that has 
pair<int, int> directional_sensing(pair<int, int> start_cords, blob* current_blob, int world_map[MAPROWS][MAPCOLUMNS], vector<blob*> location_map[MAPROWS][MAPCOLUMNS], int food_map[MAPROWS][MAPCOLUMNS]) {
    int start_x = start_cords.first;
    int start_y = start_cords.second;
    int ring_number = 1;
    vector<pair<int, int>> possibilities_for_movement; // Holds all the sqaures it could be towards
    // Always must check it's own cords first.
    if (food_map[start_x][start_y] > 0) {
        food_map[start_x][start_y]--;
        return make_pair(0, 0);
    }
    // Create a diamond with radius == sense, and stop when food is found
    while (ring_number < current_blob->sense && possibilities_for_movement.size() == 0) {
        pair<int, int> checking_tile = make_pair(start_x - ring_number, start_y); // Start the ring at the top
        int border_creation[4][2] = {{1, 1}, {1, -1}, {-1, -1}, {-1, 1}}; // Makes a diamond starting from the top
        for (int i = 0; i < 4; i++) {
            int change_x = border_creation[i][0];
            int change_y = border_creation[i][1];
            for (int next_movement = 0; next_movement < ring_number; next_movement++) {
                checking_tile.first += change_x;
                checking_tile.second += change_y;
                // make sure it's not out of range

                if (checking_tile.first < 0 || checking_tile.first >= MAPROWS || checking_tile.second < 0 || checking_tile.second >= MAPCOLUMNS) continue;
                if (food_map[checking_tile.first][checking_tile.second] > 0) {
                    possibilities_for_movement.push_back(checking_tile);
                }
            }
        }
        ring_number++;
    }
    // There is no food detected within it's sense range.
    if (!possibilities_for_movement.size()) return make_pair(-INF, -INF);

    int random_num = rand() % possibilities_for_movement.size();
    int towards_x = possibilities_for_movement[random_num].first;
    int towards_y = possibilities_for_movement[random_num].second;

    // Use trig to find out which direction to move in to most efficently get there
    int difference_x = start_x - towards_x;
    int difference_y = start_y - towards_y;

    // kinda a 3p dmoj problem: on a 2d grid, given a start and end point, which direction should you move to get to the end point the fastest
    if (difference_x == 0 && difference_y == 0) return make_pair(0, 0); // No need to move
    else if (abs(difference_x) == abs(difference_y)) { // its on a diagonal
        if (difference_x > 0 && difference_y > 0) { // it's x
            return make_pair(-1, -1);                 //      \'
        }
        else if (difference_x > 0 && difference_y < 0) { // it's  x
            return make_pair(-1, 1);                     //      / 
        }
        else if (difference_x < 0 && difference_y > 0) { // it's /
            return make_pair(1, -1);                     //     x
        }
        else if (difference_x < 0 && difference_y < 0) { // it's \'
            return make_pair(1, 1);                    //          x

        }
    } 
    else if (difference_x > 0 && abs(difference_y) > abs(difference_x)) {   // x
        return make_pair(-1, 0);                                             // |
    }
    else if (difference_x < 0 && abs(difference_y) > abs(difference_x)) {   // |
        return make_pair(1, 0);                                             // x
    }
    else if (difference_y > 0 && abs(difference_y) > abs(difference_x)) {   //  x-
        return make_pair(0, -1);                                             // 
    }
    else if (difference_y < 0 && abs(difference_y) > abs(difference_x)) {   // -x
        return make_pair(0, 1);                                             // 
    }

    // return make_pair(0, 0);
    return make_pair(INF, INF);
}


// Return is for signaling failure or success
// 0 - means successfully done
// 
// For each location_map, run random numbers to see if they spread, move or grow in number.
// Time Complexity Guess: O(N*M*blobs)
int population_spread_v2(int world_map[MAPROWS][MAPCOLUMNS], vector<blob*> location_map[MAPROWS][MAPCOLUMNS], int food_map[MAPROWS][MAPCOLUMNS], int &population) {
    // make a copy of the location_map to see where everything is moving
    // What the next gen of blobs look like.
    vector<blob*> next_gen_blobs[MAPROWS][MAPCOLUMNS] = {}; // The new blobs to add
    // std::cout << "running update" << endl;  
    for (int cur_row = 0; cur_row < MAPROWS; cur_row++){
    for (int cur_col = 0; cur_col < MAPCOLUMNS; cur_col++){
        for(blob* cur_blob: location_map[cur_row][cur_col]) {
            // reduce the life span of the blob by 1
            cur_blob->life -= 1;
            if (cur_blob->life == 0) {
                population--;
                // remove blob from world_map and list, so don't add it to the next gen
                continue;
            }
            if (world_map[cur_row][cur_col] == 1) { // It's land 
                int growth_seed = rand() % 100;                    
                if (growth_seed <= cur_blob->growth_chance) { // The growth chance succeeded
                    // random number of childern, could weight it later
                    // gets a random number between 1 and the max_children, then creates that many blobs
                    // currently new blobs can't move on the same tick they are born
                    int children_seed = rand() % cur_blob->max_childern + 1;
                    // std::cout << "Max children is " << cur_blob->max_childern << " num new children is " << children_seed << endl;
                    for (int new_child = 0; new_child < children_seed; new_child++) {
                        // CHANGE template_blob to something later.
                        // Add a mutation factor
                        next_gen_blobs[cur_row][cur_col].push_back(&template_blob);
                        // add move chance right to make them move if needed
                    }
                    population += children_seed;
                }
            }
            if (world_map[cur_row][cur_col] == 1 || world_map[cur_row][cur_col] == 0) {
                int movement_seed = rand() % 100;
                int curX = cur_row; // Where it's going to X Value
                int curY = cur_col; // Where it's going to Y Value
                // std::cout << "move seed " << movement_seed << " move chance " <<  cur_blob->movement_chance << endl;
                if (movement_seed <= cur_blob->movement_chance) { // movement chance succeeded
                    int movement_left = cur_blob->speed;

                    while (movement_left) { // While it still has movement
                        int deviation_seed = rand() % 100; // Chance to go off the path
                        bool new_direction = false;
                        if (deviation_seed <= cur_blob->movement_deviation_chance) new_direction = true; // It goes off it's path
                        if (cur_blob->movement_direction == make_pair(0, 0)) new_direction = true; // It doesn't have a path
                        int next_posX = curX + cur_blob->movement_direction.first; // Where is would go next it's not on a new path
                        int next_posY = curY + cur_blob->movement_direction.second;
                        if (next_posX < 0 || next_posX >= MAPROWS || next_posY < 0 || next_posY >= MAPCOLUMNS) new_direction = true; // Out of bounds
                        // checks for food, and if found returns the direction to move in to get as close to the food as possible
                        pair<int, int> direction_sensed = directional_sensing(make_pair(curX, curY), cur_blob, world_map, location_map, food_map);
                        int directional_x = direction_sensed.first;
                        int directional_y = direction_sensed.second;
                        if (directional_x != INF && directional_y != INF) new_direction = false; // there is food in range
                        if (directional_x == -INF && directional_y == -INF) new_direction = true; // there is food in range

                        if (new_direction) { // It needs to choose a direction to start moving in

                            // copy all movement options to possible moves in 2 lines
                            vector<pair<int, int>> possible_moves {}; // empty
                            for (auto possible_movement : movement_options) {
                                int position_x = possible_movement[0] + cur_row;
                                int position_y = possible_movement[1] + cur_col;
                                // If NOT (x or y is out of the world_map) add the movement to possible_moves
                                if (!(position_x < 0 || position_x >= MAPROWS || position_y < 0 || position_y >= MAPCOLUMNS)) {
                                    possible_moves.push_back(make_pair(possible_movement[0], possible_movement[1])); 
                                } // Pushs back only the movement step, not the new location
                            }
                            // Choose a random place to move to out of the valid moves
                            int new_tile = rand() % possible_moves.size();
                            curX += possible_moves[new_tile].first; // reassigne the x, and y coordinates of it
                            curY += possible_moves[new_tile].second;
                            cur_blob->movement_direction = possible_moves[new_tile];
                        }
                        else { // Follow the path
                            // There is no food in it's area
                            if (directional_x == INF && directional_y == INF) {
                                curX = next_posX;
                                curY = next_posY;
                            }
                            else {
                                curX += directional_x;
                                curY += directional_y;
                            }
                        }
                        movement_left--;
                    }
                }
                // Add it to the new position on the next gen location map
                // cout << "Pushed to " << curX << " " << curY << endl;
                next_gen_blobs[curX][curY].push_back(cur_blob);
            }
        }
        }
    }
    // cout << "Updates ending" << endl;

    // Now update all_blobs to reflect what happened this tick.
    // location_map = next_gen_blobs;  // absolutely useless
    // Set all the vectors, can't use 'location_map = next_gen_blobs' for some reason (updates in function, but not out of it)
    for(int i = 0; i < MAPROWS;i++) {
        for(int j = 0; j < MAPCOLUMNS;j++) {
            location_map[i][j] = next_gen_blobs[i][j];
        }
    }

    // This is debug for population count
    // int total_pop = 0;
    // for(int i = 0; i < MAPROWS;i++) {
    //     for(int j = 0; j < MAPCOLUMNS;j++) {
    //         total_pop += location_map[i][j].size();
    //     }
    // }
    // cout << "Population is " << total_pop << "\n";

    return 0;
}



void displayMapToGUI(){

}









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
sf::Texture land_tile, sea_tile, food_texture; // Load a sprite to display
sf::Font arial; // the font I will use for population numbers

if (!land_tile.loadFromFile("C:\\Users\\zacha\\Documents\\vsgit\\greenSquare.png")) return EXIT_FAILURE; // Failed to load texture
if (!sea_tile.loadFromFile("C:\\Users\\zacha\\Documents\\vsgit\\blueSquare.png")) return EXIT_FAILURE; // Failed to load texture
if (!food_texture.loadFromFile("C:\\Users\\zacha\\Documents\\vsgit\\yellowCircle.png")) return EXIT_FAILURE; // Failed to load texture
if (!arial.loadFromFile("C:\\Users\\zacha\\Documents\\vsgit\\arial.ttf")) return EXIT_FAILURE; // Load in the font
sf::Sprite land_tile_sprite(land_tile); // load the texture onto the sprite
sf::Sprite sea_tile_sprite(sea_tile);
sf::Sprite food_sprite(food_texture); // load the texture onto the sprite

// This is used to display the population number at world_map[i][j]
sf::Text pop_count_number_display("0", arial); // The "sprite" like object for text
pop_count_number_display.setFillColor(sf::Color::Black); // What colour is the font
pop_count_number_display.setStyle(sf::Text::Bold | sf::Text::Underlined); // Set styles (eg. bold, underlined, strikethrough)


randGen(world_map);


// Setup first blob
struct blob test_blob = template_blob;
location_map[5][5].push_back(&test_blob);
int population = 1; // A population tracker

food_map[3][9] = 10;
food_map[3][3] = 10;
food_map[9][9] = 1;
food_map[0][0] = 200;
// food_map[1][1] = 1;


// Start the loop
while (window.isOpen()) {
    sf::Event event; // Process events
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) { // Close window: exit
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
            // std::cout << "Right arrow key pressed!\n";
            population_spread_v2(world_map, location_map, food_map, population);
            std::cout << "Population is: " << population << endl;
        }
    }
    window.clear();
    // Draw the sprite
    // divide factor
    float divide_factor = 1;
    float square_scale = (float)(min(WINDOW_SIZE_X, WINDOW_SIZE_Y) / min(MAPROWS, MAPCOLUMNS)) / 500 / divide_factor; // How much to scale the squares by
    float square_len = min(WINDOW_SIZE_X, WINDOW_SIZE_Y) / min(MAPROWS, MAPCOLUMNS) / divide_factor; // Where the next square should start
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
                std::cout << "ERROR !!! world_map corrupted !!!" << endl;
                throw std::invalid_argument( "Received a invalid value drawing the world_map" );
            }

            // Render all the food in random position, "Notice: it may look like the food is teleporting"
            int lower_x = (float)i * square_len;
            int lower_y = (float)j * square_len;
            for (int k = 0; k < food_map[i][j]; k++) {
                int rand_x = (float)(rand() % (int)(square_len*100) + (lower_x*100)) / 100; // anywhere from lower_bound to the next square is valid 
                int rand_y = (float)(rand() % (int)(square_len*100) + (lower_y*100)) / 100; // (*100 is because rand doesn't work with decimals)
                food_sprite.setPosition(rand_x, rand_y);
                window.draw(food_sprite);
            }

            // Print the population at the location
            pop_count_number_display.setPosition((float)i*square_len, (float)j*square_len);
            pop_count_number_display.setString(to_string(location_map[i][j].size())); // to_string makes the int a string
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

