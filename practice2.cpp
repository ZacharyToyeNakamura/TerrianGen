#include <iostream>
#include <stdio.h>
#include <cstdlib> // rand()
#include <vector> // dyanamically sized arrays for my population
using namespace std;

const int MAPROWS = 10;
const int MAPCOLUMNS = 10;
const int growth_chance = 50; // chance for childern out of 100%
const int max_childern = 1; // random number of childern
const int movement_chance = 1; // chance for the population to move
const int percentage_movement = 50; // How much each portion of the population wants to move out of 100%. 100% Means that the population likes to move together
const int movement_options[8][2] = {    {-1, -1}, {-1, 0}, {-1, 1},
                                        { 0, -1},          { 0, 1},
                                        { 1, -1}, { 1, 0}, { 1, 1},}; // They can move in any of the 8 directions
// predeclare func
struct blob{
    int life;
    int growth_chance;
    int max_childern;
    int movement_chance;
};
void print_population_map(vector<vector<vector<blob>>> population_map);
blob create_blob(int life, int growth_chance, int max_childern, int movement_chance);



int main() {
    blob next_test;
    next_test.growth_chance = 10;
    cout << "growth " << next_test.growth_chance << endl;
    blob test_blob = create_blob(10, 100, 2, 50);
    cout << "test " << test_blob.life << endl;
    // a 2d list of lists that contain each blob, eg at population[i][j] there is a list of all the blobs
    vector<vector<vector<blob>>> population (blob, vector<vector<blob>>(blob, vector<blob>(blob)));
    cout << "Pop created" << endl;
    population.push_back(vector<vector<blob>>()); // why 
    population[0].push_back(vector<blob>()); // like this
    population[0][0].push_back(test_blob); // Why does this crash it ?
    cout << "Population Map" << population[0][0][0].life << endl;
    print_population_map(population);
    // cout << "Hello World\n" << endl;
    return 0;
}



void print_population_map(vector<vector<vector<blob>>> population_map){
    for (int i = 0; i < MAPROWS; i++){
        for (int j = 0; j < MAPCOLUMNS; j++){
            cout << population_map[i][j].size() << " ";
        }
        cout << endl;
    }
}

// Creates a blob structure and returns it to a variable
blob create_blob(int life = 0, int growth_chance = 0, int max_childern = 0, int movement_chance = 0){
    cout << "Creating Blob" << endl;
    blob temp_struct; // Why does making this struct a pointer to a struct make if just crash or smt
    cout << "Why crash" << endl;
    temp_struct.life = life; // Crashs here
    cout << "1 " << endl;
    temp_struct.growth_chance = growth_chance;
    cout << "2 " << endl;
    temp_struct.max_childern = max_childern;
    cout << "3 " << endl;
    temp_struct.movement_chance = movement_chance;
    cout << "Finished making blob" << endl;

    return temp_struct;
};