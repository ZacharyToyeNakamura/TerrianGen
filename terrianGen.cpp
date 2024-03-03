// all of the terrian generation algorithms (all garbage)


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

const int MAPROWS1 = 200;
const int MAPCOLUMNS1 = 200;

// random gen
/**
 * Takes in a 2d array and randomly generates terrian
 * worldMap[][] The 2d array
 * numTypes     The number of types of terrian
*/
void randGen(int worldMap[MAPROWS1][MAPCOLUMNS1], int numTypes = 2) {
    if(MAPROWS1 == 0 || MAPCOLUMNS1 == 0) return; // 0 len array
    // std::cout << sizeof(worldMap) << " "<< sizeof(worldMap[0]) << " " << sizeof(worldMap[0][0]) << endl;
    // for(int i = 0; i < sizeof(worldMap)/sizeof(worldMap[0]); i++) { // for every array in worldMap
    for(int i = 0; i < MAPROWS1; i++) { // for every array in worldMap
        for(int j = 0; j < sizeof(worldMap[i])/sizeof(worldMap[0][i]); j++) { // for every element
            int randSeed = rand() % numTypes;
            worldMap[i][j] = randSeed;
        }
    }
}


// random gen
/**
 * Takes in a 2d array and randomly generates terrian
 * worldMap[][] The 2d array
 * numTypes     The number of types of terrian
*/
void lessRandGen(int worldMap[MAPROWS1][MAPCOLUMNS1], int numTypes = 2, int numCircles = 10, int circleRange = 10) {
    srand(time(0));
    if(MAPROWS1 == 0 || MAPCOLUMNS1 == 0) return; // 0 len array

    for(int circle = 0; circle < numCircles; circle++) {
        int randSeedX = rand() % MAPROWS1;
        int randSeedY = rand() % MAPCOLUMNS1;
        worldMap[randSeedX][randSeedY] = 1;
        for(int i = -circleRange; i < circleRange; i++) {
            for(int j = -circleRange; j < circleRange; j++) {
                if(randSeedX + i < 0 || randSeedY + j < 0 ||randSeedX + i >= MAPROWS1 || randSeedY + j >= MAPCOLUMNS1) continue;
                // cout << i+randSeedX << " " << j+randSeedY << endl;
                worldMap[i+randSeedX][j+randSeedY] = 1;
            }
        }
    }

}