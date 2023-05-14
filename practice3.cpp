// Messing with functions with vector<blob*> *var[][]
// trying to reassign

#include <iostream>
#include <stdio.h>
#include <cstdlib> // rand()
#include <vector> // dyanamically sized arrays for my population_map
#include <algorithm>
#include <ctime> // srand(time(0)) for true random
using namespace std;

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
            sense == other_blob.sense
            ) {
                return true;
            }
        return false;
    }
};
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

blob template_blob = create_blob(10, 20, 3, 1, 75, 50, 2, 0);

// Test starts here


int function1(vector<blob*> location_map[10][10]) {
    vector<blob*> next_gen[10][10];
    auto next_gen_blobs = next_gen;

    next_gen_blobs[0][0].push_back(&template_blob);
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            location_map[i][j] = next_gen_blobs[i][j];
        }
    }

    // swap(location_map, next_gen_blobs);
    cout << "final in function " << location_map[0][0].size() << endl;
    return 0;
}

vector<blob*> location_map[10][10]; // stores each blob in a vecotr


int function2(vector<int> wow_so_cool[10][10]) {
    wow_so_cool[0][0].push_back(24567);
    return 0;
}
vector<int> a_array[10][10];

int main() {
    function1(location_map);
    cout << "final " << location_map[0][0].size() << endl;
    function2(a_array);
    cout << "a_array[0][0] is " << a_array[0][0].size() << endl;

    return 0;
}