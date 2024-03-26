// A file of constants
#ifndef CONSTANTS_H // If it's not defined yet, then define it
#define CONSTANTS_H

#define ll long long


// #if !defined(MYLIB_CONSTANTS_H)
// #define MYLIB_CONSTANTS_H 1

// Define the max number
const int INF = 0x3f3f3f3f;
const long long INFL = 0x3f3f3f3f3f3f3f3f;

// The max size of the simulation allowed (for memory reasons)
const int MN = (int)1e4;

// Starting size of the window
const int WINDOW_SIZE_X = 500;
const int WINDOW_SIZE_Y = 500;


// Biome numbering
const int PEAK_NUMBER = 1;
const int LAND_NUMBER = 2;
const int BEACH_NUMBER = 3;
const int LAKE_NUMBER = 4;
const int OCEAN_NUMBER = 5;


// First biome seperation heights.
const double WATER_LEVEL = 0.475; // Inclusively
const double SAND_MAX_HEIGHT = 0.525; // sand is between (0.475, 0.525]
const double LAND_MAX_HEIGHT = 0.97; // sand is between (0.525, 0.97]
const double PEAK_MAX_HEIGHT = 1; // sand is between (0.97, 1]

// Constant related to the height map
// All of these constants are bounds for the sine wave 
const int COUNT = 100; // higher -> more detail but longer gen times
const int aBound = 1000; // a value should remain closer to 0
const int bBound = 1000; // mid - high for variability (shift horizontally)
const int kBound = 1000; // should remain high for variability
const int cBound = 1000; // mid - high for variablity (shift vertically)
const int midX = 0, midY = 0; // doesn't matter except for showing off 1 wave


const int MOVEMENT_OPTIONS[8][2] = {    {-1, -1}, {-1, 0}, {-1, 1},
                                        { 0, -1},          { 0, 1},
                                        { 1, -1}, { 1, 0}, { 1, 1},}; // They can move in any of the 8 directions
const int CARDINAL_DIRECTIONS[4][2] = {{1,0},{-1,0},{0,1},{0,-1}}; // Only the 4 cardinal directions.


const double STEP_SIZE_X = 2; // There are window_size_x / step_size_x cells in the x directions
const double STEP_SIZE_Y = 2; // how many pixels each cell takes up
const int SIZEX = (int)(WINDOW_SIZE_X / STEP_SIZE_X);
const int SIZEY = (int)(WINDOW_SIZE_Y / STEP_SIZE_Y);



#define DELETE_KEY 8
#define ENTER 13
#define ESCAPE 27

#endif
