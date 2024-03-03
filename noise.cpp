#include <SFML/Window.hpp>
// #include <RenderWindow.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>

#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <queue>

using namespace std;

#define ll long long
// map(x, y) = (k)sin(a(x - b)) + (k)sin(a(y - b)) + c
// K is it's influence / stretch
// b is shift l/r
// c is shift up down
const int MN = (int)1e4;
const int COUNT = 100; // higher -> more detail but longer gen times
const int aBound = 1000; // a value should remain closer to 0
const int bBound = 1000; // mid - high for variability (shift horizontally)
const int kBound = 1000; // should remain high for variability
const int cBound = 1000; // mid - high for variablity (shift vertically)
const int midX = 0, midY = 0; // doesn't matter except for showing off 1 wave
const int sizeX = 100, sizeY = 100;
const int INF = 0x3f3f3f3f;

const int movement_options[8][2] = {    {-1, -1}, {-1, 0}, {-1, 1},
                                        { 0, -1},          { 0, 1},
                                        { 1, -1}, { 1, 0}, { 1, 1},}; // They can move in any of the 8 directions


const int WINDOW_SIZE_X = 500;
const int WINDOW_SIZE_Y = 500;

const double stepSizeX = 2;
const double stepSizeY = 2;
// const double stepper = 0.00018; // How scaled it is.
const double stepper = 0.0003; // How scaled it is.

// const double stepper = 1; // How scaled it is.



double a[COUNT], b[COUNT], b2[COUNT], k[COUNT], c[COUNT];  
double rotation[COUNT]; // testing
double graph[MN][MN];
int biome[MN][MN];

// Used for a custom sort that sorts based on numbers closest to 0
bool closerToZero(double num1, double num2) {
    return abs(num1) < abs(num2);
}

/**
 * Takes in a num and returns a color, always a shade of white/gray/black.
 * The closer to 1 the lighter color
 * The closer to 0 the darker the color
 * 
 * @param num A double between [0, 1]
 * @return A color object, that is used to color sprites on the GUI
*/
sf::Color grayScale(double num) {
    return sf::Color(255*num, 255*num, 255*num);
}


sf::Color worldlyColor(double num) {
    if(num > 0.97) {
        // Snow caps
        return sf::Color(255, 255, 255);
    }
    else if(num > 0.525) {
        // green 
        return sf::Color(50*num, 255*num, 50*num);
    }
    else if(num > 0.475 && num < 0.525) {
        // sandy?
        num += 0.5;
        return sf::Color(194*num, 178*num, 128*num);
    }
    else {
        // Makes sure that lower numbers aren't too dark
        num = num + 0.5; // this makes lower numbers darker
        return sf::Color(50*num, 50*num, 255*num);
    }
    return sf::Color(0, 0, 0);
}

sf::Color continentalColor(double num) {
    if(num > 0.90) {
        // Snow caps
        return sf::Color(255,255,255);
    }
    else if(num > 0.425) {
        // green
        return sf::Color(50*num, 255*num, 50*num);
    }
    else if(num > 0.375 && num < 0.425) {
        // sandy?
        num += 0.5;
        return sf::Color(194*num, 178*num, 128*num);
    }
    else {
        num = 1 - num; // this makes lower numbers darker
        return sf::Color(50*num, 50*num, 255*num);
    }
    return sf::Color(0, 0, 0);
}

sf::Color biomeColoring(int num) {
    if(num == 1) return sf::Color(255, 255, 255); // white
    if(num == 2) return sf::Color(4, 255, 0); // green
    if(num == 3) return sf::Color(241, 255, 82); // Sandy/yellow
    if(num == 4) return sf::Color(0, 170, 255); // very light blue
    if(num == 5) return sf::Color(0, 21, 201); // dark blue

    return sf::Color(0, 0, 0); // black
}

/**
 * Returns if n is inside of the interval [lowerBound, upperBound] (Note that it's inclusive).
*/
bool inRange(int n, int lowerBound, int upperBound) {
    return lowerBound <= n && n <= upperBound;
}

/**
 * Returns true if x or y are out of bounds 0 <= x < maxX && 0 <= y < maxY
*/
bool outOfBounds(int x, int y, int limitX, int limitY) {
    return 0 > x || x >= limitX || 0 > y || y >= limitY;
}

/**
 * BFS to find an enclosed space surrounded by pixels of height maxHeight.
 * 
 * @param visited Stores which pixels have been visited and sets them all to 
 * 
*/
int defineBodyOfWater(int startX, int startY, double maxHeight, int visited[MN][MN]) {
    const int DEFINE_WATER = -0x3f3f3f;
    const int MIN_OCEAN_SIZE = 500; // anything above or equal to it is an ocean, below is a lake
    const int LAKE_NUMBER = 4, OCEAN_NUMBER = 5;

    // The size of the body of water
    int waterSize = 1;
    visited[startX][startY] = DEFINE_WATER;
    priority_queue<pair<int, int>> qu;
    qu.push({startX, startY});
    while(!qu.empty()) {
        auto [currentX, currentY] = qu.top();
        qu.pop();
        for(auto change: movement_options) {
            int nextX = change[0] + currentX;
            int nextY = change[1] + currentY;
            // If it's out of bounds continue
            // cout << nextX << ", " << nextY << endl;
            if(outOfBounds(nextX, nextY, ((int)WINDOW_SIZE_X / stepSizeX), ((int)WINDOW_SIZE_Y / stepSizeY))) continue;
            // cout << "h" << endl;
            // inRange(visited[nextX][nextY], 1, 5) || 
            if(inRange(visited[nextX][nextY], 1, 5) || visited[nextX][nextY] == DEFINE_WATER) continue; // if it's already define or is going to be defined don't check
            if(graph[nextX][nextY] > maxHeight) continue; // if it's not water continue
            waterSize++;
            visited[nextX][nextY] = DEFINE_WATER;
            qu.push({nextX, nextY});
        }
    }
    // Set all of the found tiles to an ocean or lake depending the size of the body of water.
    for (int i = 0; i < ((int)WINDOW_SIZE_X / stepSizeX); i++) {
        for (int j = 0; j < ((int)WINDOW_SIZE_Y / stepSizeY); j++) {
            if (visited[i][j] == DEFINE_WATER) {
                // cout << "Defined " << waterSize << endl; // DEBUG
                if (waterSize >= MIN_OCEAN_SIZE) visited[i][j] = OCEAN_NUMBER;
                else visited[i][j] = LAKE_NUMBER;
                // cout << "now " << visited[i][j] << endl;
            }
        }
    }
    cout << "Just defined a body of water with size " << waterSize << endl; // DEBUG 
    
    return waterSize;
}

/**
 * Defines the biomes, (eg which pixels are part of a lake / ocean or mountain peak).
 * It using bfs to define lakes and oceans.
 * 0 is not known yet
 * 1 is peaks
 * 2 is land
 * 3 is beaches
 * 4 is lakes
 * 5 is oceans
 * 6 is rivers
 * 
 * !!!BASED ON WORLDLY GEN!!!
*/
void defineBiomes(int maxX, int maxY) {
    // Sinusoidal noise gen end
    for(int x = 0; x < maxX; x++) {
        for(int y = 0; y < maxY; y++) {
            if(biome[x][y] == 4 || biome[x][y] == 5) continue;
            // If it's not water then continue
            if(graph[x][y] > 0.97) biome[x][y] = 1;
            else if(graph[x][y] > 0.525) biome[x][y] = 2;
            else if(graph[x][y] > 0.475) biome[x][y] = 3;
            else {
                defineBodyOfWater(x, y, 0.475, biome);
            }
        }
    }
}

/**
 * Generates a random double between lowerLim and upperLim
 * @param upperLim The upper limit for the random double
 * @param lowerLim The lower limit for the random double
 * @return A random double.
*/
double randDouble(double lowerLim, double upperLim) {
    // The % number and / number must be the same for best results.
    // Can't be 1e5 because it just breaks probably due to not enough space (no long long)
    // If there is a way change to ~1e9 or smt
    return lowerLim + ((upperLim - lowerLim) * (rand() % 19354)) / 19354.0; // All brackets are necessary
}


int main() {
    // Sinusoidal noise gen func start

    cout << "Starting " << endl;
    srand(time(0));

    // Generate random numbers for the sin functions
    for(int i  = 0; i < COUNT; i++) {
        // a[i] = rand() % aBound+1;
        // if(!(rand() % 2)) a[i] = 1.0/a[i]; // randomly inverse a value
        a[i] = randDouble(0, 500);
        // a[i] = 1.0/a[i];
        // b[i] = rand() % bBound;
        // if(rand() % 2) b[i] *= -1; // randomly negative b value
        // b2[i] = rand() % bBound;
        // if(rand() % 2) b2[i] *= -1; // randomly negative b2 value
        // c[i] = rand() % cBound;
        b[i] = randDouble(0, 1000); // Only needs to be 0 to 2pi ?
        b2[i] = randDouble(0, 1000); // Only needs to be 0 to 2pi ?
        c[i] = randDouble(0, 1000);
        // c[i] = randDouble(0, 1);
        // k[i] = rand() % kBound+1;
        k[i] = randDouble(0, 1000);
        // if(rand() % 2) k[i] = 1.0/k[i]; // randomly inverse k value
        rotation[i] = randDouble(0, 90);
    }

    // std::sort(k, k+COUNT, closerToZero);
    std::sort(k, k+COUNT);
    // Debug: prints all waves
    // for(int i = 0; i < COUNT; i++) cout << k[i]<< "sin("<<a[i]<<"(x -"<<b[i]<<")) + "<< k[i]<< "(sin("<<a[i]<<"(y -"<<b2[i]<<")) + "<<c[i]<<endl;

    


    double minVal = INF, maxVal = -INF; 
    // Testing a large amount of values (not all) to find the min and max to normalize vales to between 0 and 1
    for(int x = 0; x < (int)(WINDOW_SIZE_X / stepSizeX); x++) {
        for(int y = 0; y < (int)(WINDOW_SIZE_Y / stepSizeY); y++) {
            double tot = 0;
            for(int i = 0; i < COUNT; i++) {
                // tot += (k[i] * sin(a[i] * (x - midX - b[i])) + (k[i] * sin(a[i] * (y - midY - b2[i])) + c[i])) / (double)COUNT;
                // tot += (k[i] * sin(a[i] * (x*stepper - midX - b[i])) + (k[i] * sin(a[i] * (y*stepper - midY - b2[i])) + c[i])) / (double)(i + 1);
                // graph[x][y] += (k[i] * sin(a[i] * (x - midX - b[i])) + (k[i] * sin(a[i] * (y - midY - b[i])) + c[i]));
                tot += (k[i]*sin(a[i]*(x*stepper*cos(rotation[i])-y*stepper*sin(rotation[i])-b[i])) + k[i]*sin(a[i]*(x*stepper*sin(rotation[i])+y*stepper*cos(rotation[i])-b2[i]))) / (double)(i + 1);
            }
            if(tot < minVal) minVal = tot;
            if(tot > maxVal) maxVal = tot;
        }
    }

    

    // Normalize numbers from 0 to 1 from now on after getting the range using
    // val = (((generated value - minVal)) / (maxVal - minVal))
    for(int x = 0; x < (int)(WINDOW_SIZE_X/ stepSizeX); x++) {
        for(int y = 0; y < (int)(WINDOW_SIZE_Y / stepSizeY); y++) {
            double tot = 0;
            for(int i = 0; i < COUNT; i++) {
                // tot += (k[i] * sin(a[i] * (x - midX - b[i])) + (k[i] * sin(a[i] * (y - midY - b2[i])) + c[i])) / (double)COUNT;
                // tot += (k[i] * sin(a[i] * (x*stepper - midX - b[i])) + (k[i] * sin(a[i] * (y*stepper - midY - b2[i])) + c[i])) / (double)COUNT;
                // tot += (k[i] * sin(a[i]*(x*stepper - midX - b[i] + a[i]*(y*stepper - midY - b2[i]))) + c[i]) / (double)COUNT;
                // tot += (k[i] * sin(a[i]*(x*stepper - midX - b[i] + a[i]*(y*stepper - midY - b2[i]))) + c[i]) / (double)(i + 1);
                
                // graph[x][y] += (k[i] * sin(a[i] * (x - midX - b[i])) + (k[i] * sin(a[i] * (y - midY - b[i])) + c[i]));
                tot += (k[i]*sin(a[i]*(x*stepper*cos(rotation[i])-y*stepper*sin(rotation[i])-b[i])) + k[i]*sin(a[i]*(x*stepper*sin(rotation[i])+y*stepper*cos(rotation[i])-b2[i]))) / (double)(i + 1);
            }
            // tot = (int)tot % 255;
            // tot = abs(tot);
            graph[x][y] = (tot - minVal) / (maxVal - minVal);
            // graph[x][y] = abs((int)tot % 255);
        }
    }

    

    // Biome gen start
    defineBiomes((int)(WINDOW_SIZE_X / stepSizeX),(int)(WINDOW_SIZE_Y / stepSizeY));



    // Number output
    // for(int i = 35; i < sizeX-30; i++) {
    //     for(int j = 35; j < sizeY-30; j++) {
    //         cout << round(graph[i][j]) << " ";
    //     }
    //     cout << endl;
    // }

    // create window
    sf::RenderWindow window(sf::VideoMode(WINDOW_SIZE_X, WINDOW_SIZE_Y), "Noise", sf::Style::Default);
    window.setActive(); // activate
    // Set the color and depth clear values
    glClearDepth(1.f);
    glClearColor(0.f, 0.f, 0.f, 1.f); // color
    // Configure the viewport (the same size as the window)
    glViewport(0, 0, window.getSize().x, window.getSize().y);

    // Declaring the square
    sf::RectangleShape square(sf::Vector2f(120.0, 50.0)); // sets (x, y)
    square.setSize(sf::Vector2f(100, 100)); // changes the size
    sf::Color squareColor(100, 100, 100);
    square.setFillColor(squareColor); // compress these 2 lines to square.setFillColor(sf::Color(100, 100, 100)); 


    int TOPO_MODE = 0;
    int BIOME_MODE = 1;
    int currentMode = 0, numModes = 2;

    // Start the loop
    while (window.isOpen()) {
        sf::Event event; // Process events
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) { // Close window: exit
                window.close();
            }
            // Change view
            if ((event.type == sf::Event::MouseButtonPressed) && (event.mouseButton.button == sf::Mouse::Left)) {
                currentMode = (currentMode + 1) % numModes;
            }
            // Escape key: exit
            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape)) {
                window.close();
            }
            // Resize event: adjust the viewport
            if (event.type == sf::Event::Resized) {
                glViewport(0, 0, event.size.width, event.size.height);
            }
        }
        // Reset the frame
        window.clear();

        square.setSize(sf::Vector2f(stepSizeX, stepSizeY));
        // square.setSize(sf::Vector2f(25, 25));
        // cout << (int)(window.getSize().x / stepSizeX) << " " << (int)(window.getSize().y / stepSizeY) <<endl;
        for(int x = 0; x < (int)(window.getSize().x / stepSizeX); x++) {
            for(int y = 0; y < (int)(window.getSize().y / stepSizeY); y++) {
                square.setPosition(sf::Vector2f(x * stepSizeX, y*stepSizeY));
                // square.setFillColor(grayScale(graph[x][y]));
                if (currentMode == TOPO_MODE) square.setFillColor(worldlyColor(graph[x][y]));
                if (currentMode == BIOME_MODE) square.setFillColor(biomeColoring(biome[x][y]));
                // square.setFillColor(sf::Color(graph[x][y], graph[x][y], graph[x][y]));
                window.draw(square);
            }
        }


        // end the current frame
        window.display();
    }


    return 0;
}