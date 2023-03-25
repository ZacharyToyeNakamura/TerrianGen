// Save code before compiling
// Use g++
// gcc main.cpp -lstdc++ -o main.o


// TODO:
// Convert code to opp, and create a class with function instead of a struct.

// Wave collapse function for land/sea generation
// AI for movement towards new lands probably using bfs or something
// for updates over like 100,000 it slows considerability
// Solution for if there are just a few types of blobs, life span is short, and no genentics
// Store a different population array for each type of blob, and the life spans of each blob at [i][j] (they should all have the same growth, max children, and movement)
// For [i][j] in the population array, remove shift the array to the left by 1

#include <iostream>
#include <stdio.h>
#include <cstdlib> // rand()
#include <vector> // dyanamically sized arrays for my population_map
#include <algorithm>
#include <ctime> // srand(time(0)) for true random

using namespace std;
// This converts the variable name to a string (needs to be a string)
#define GET_VARIABLE_NAME(Variable) (#Variable)
#define dbgArr (arr, i, l, r) for(int i = (l); i < (r); i++) std::cout << arr[i] << " "; std::cout << endl;

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
blob template_blob = create_blob(10, 50, 3, 75);

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


int main() {
    int seed_rand = time(0);
    srand(seed_rand); // pseudo random but different everytime
    // srand(1679778862);
    // int world_map[MAPROWS][MAPCOLUMNS] = {}; // all 0s!

    // A test blob for testing, it has random values
    struct blob test_blob = template_blob;
    // Simplified down. It's the numbe of blobs at each place, doesn't give addresses because I couldn't find a way to remove them
    
    // The address of each blob, also it's position (i, j)
     

    population_map[5][5] += 1;
    all_blobs.push_back(make_pair(&test_blob,make_pair(5, 5)));

    std::cout << "Land to sea world_map" << endl;
    print_map(world_map); // how to pass by address, it's already passed by address
    std::cout << "population_map world_map" << endl;
    print_population_map(population_map);
    std::cout << all_blobs[0].first->life << endl;

    for (int i = 0; i < 30; i ++) {
        population_spread(world_map, population_map, all_blobs);
        print_population_map(population_map);
        std::cout << "Tick number is " << i << endl;
        std::cout << "Total population is " << all_blobs.size() << endl;
        std::cout << "Random seed is currently " << seed_rand << endl;
        // int temp;
        // cin >> temp;
    }


    return 0;
}
// gcc test2.cpp -municode

// This function iterates through the world_map and prints it. . .
void print_map(int world_map[MAPROWS][MAPCOLUMNS]){
    for (int i = 0; i < MAPROWS; i++){
        for (int j = 0; j < MAPCOLUMNS; j++){
            std::cout << world_map[i][j] << " ";
        }
        std::cout << endl;
    }
}
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
        // std::cout << "Blob " << i << endl;
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
        if (world_map[cur_row][cur_col] == 1) { // It's land 
            // std::cout << "On land" << endl;
            int growth_seed = rand() % 100;
            int movement_seed = rand() % 100;
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
                // next_gen_blobs.push_back(all_blobs[i]);

            }
            // Add it to the next generation (eg it survived)
            next_gen_blobs.push_back(all_blobs[i]);
        }
        // Movement on water, can't reproduce on water.
        // Change later to use bfs to find all "continents / island ", then make them move towards a random shore that's not connected to their on continent.
        if (world_map[cur_row][cur_col] == 0) {
            int movement_seed = rand() % 100;
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
                // Add it to the next generation (eg it survived)
                next_gen_blobs.push_back(all_blobs[i]);
            }
        }
    }
    // std::cout << "End tick pop size is " << next_gen_blobs.size() << " ";
    // Now update all_blobs to reflect what happened this tick.
    all_blobs = next_gen_blobs;
    // cout << "Trying to free" << endl;
    // for (auto old_blob: next_gen_blobs) {
    //     cout << "Freeing???" << endl;
    //     free(old_blob.first);
    // } // free the old blobs that are no longer neccassary (having memory leak issuses)
    // std::cout << " but all_blobs size is now " << all_blobs.size() << endl;

    return 0;
}