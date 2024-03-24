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
#include "RestrictBox.cpp"
#include "RestrictBoxGroup.cpp"

using namespace std;


// map(x, y) = (k)sin(a(x - b)) + (k)sin(a(y - b)) + c
// K is it's influence / stretch
// b is shift l/r
// c is shift up down


// const double stepper = 0.00018; // How scaled it is.
// const double stepper = 0.0003; // How scaled it is.
// const double stepper = 1; // How scaled it is.

const double stepper = 0.0001; // zoomed in


double a[COUNT], b[COUNT], b2[COUNT], k[COUNT], c[COUNT];  
double rotation[COUNT]; // testing
double graph[MN][MN];
int biome[MN][MN], river[MN][MN];


/**
 * Takes in a num and returns a color, always a shade of white/gray/black.
 * The closer to 1 the lighter color
 * The closer to 0 the darker the color
 * 
 * @param num A double between [0, 1]
 * @return A color object, that is used to color sprites on the GUI
*/
sf::Color grayScale(int x, int y) {
    if(river[x][y] == 1) return sf::Color(34, 21, 150);
    double num = graph[x][y];
    return sf::Color(255*num, 255*num, 255*num);
}

/**
 * A color scheme for the GUI, this make it look like a more typical map.
 * 
 * @param x, y the coordinates of the pixel being colored.
 * @return A color object containing the color of the pixel based on height and biome.
 * 
 * TODO: Replace sf::Color with constants
*/
sf::Color worldlyColor(int x, int y) {
    double num = graph[x][y];
    if(river[x][y] == 1) return sf::Color(34, 21, 150);

    if(num > PEAK_MAX_HEIGHT) return sf::Color(255, 255, 255); // Snow caps
    else if(num > SAND_MAX_HEIGHT) return sf::Color(50*num, 255*num, 50*num); // green
    else if(num > WATER_LEVEL && num <= SAND_MAX_HEIGHT) {
        num += 0.5;
        return sf::Color(194*num, 178*num, 128*num); // sandy?
    }
    else {
        num = num + 0.5; // this makes lower numbers darker
        return sf::Color(50*num, 50*num, 255*num);
    }
    return sf::Color(0, 0, 0);
}


/**
 * A color scheme for the gui, this makes each biome a distinct color
 * 
 * @param x, y An x y coordinate between [0, SIZEX) or [0, SIZEY)
 * @return The color of the pixel at the coordinate (x, y) based on biome and river.
 * 
 * TODO: Replace sf::Color with constants
*/
sf::Color biomeColoring(int x, int y) {
    int num = biome[x][y];
    // Later move oceans and lakes above the rivers.
    if(river[x][y] == 1) return sf::Color(34, 21, 150);
    if(num == 1) return sf::Color(255, 255, 255); // white
    if(num == 2) return sf::Color(4, 255, 0); // green
    if(num == 3) return sf::Color(241, 255, 82); // Sandy/yellow
    if(num == 4) return sf::Color(0, 170, 255); // very light blue
    if(num == 5) return sf::Color(0, 21, 201); // dark blue

    return sf::Color(0, 0, 0); // black
}




/**
 * BFS to find an enclosed space surrounded by pixels of height maxHeight.
 * 
 * @param startX The starting x-coordinate for the body of water.
 * @param startY The starting y-coordinate for the body of water.
 * @param maxHeight The height at which the water shouldn't exceed.
 * @param visited Stores which pixels have been visited and sets them all to DEFINE_WATER, then sets the biome of all 
 * 
*/
int defineBodyOfWater(int startX, int startY, double maxHeight, int visited[MN][MN]) {
    const int DEFINE_WATER = -0x3f3f3f;
    // anything above or equal to it is an ocean, below is a lake
    const int MIN_OCEAN_SIZE = (SIZEX * SIZEY) * 0.004; // 4% of total size 

    // The size of the body of water
    int waterSize = 1;
    visited[startX][startY] = DEFINE_WATER;
    priority_queue<pair<int, int>> pq;
    pq.push({startX, startY});
    while(!pq.empty()) {
        auto [currentX, currentY] = pq.top();
        pq.pop();
        for(auto change: MOVEMENT_OPTIONS) {
            int nextX = change[0] + currentX;
            int nextY = change[1] + currentY;
            // If it's out of bounds continue
            if(outOfBounds(nextX, nextY, ((int)SIZEX), ((int)WINDOW_SIZE_Y / STEP_SIZE_Y))) continue;
            if(inRange(visited[nextX][nextY], 1, 5) || visited[nextX][nextY] == DEFINE_WATER) continue; // if it's already define or is going to be defined as part of the water don't check
            if(graph[nextX][nextY] > maxHeight) continue; // if it exceeds the max height allowed for water.
            waterSize++;
            visited[nextX][nextY] = DEFINE_WATER;
            pq.push({nextX, nextY});
        }
    }
    // Set all of the found tiles to an ocean or lake depending the size of the body of water.
    for (int i = 0; i < SIZEX; i++) {
        for (int j = 0; j < SIZEY; j++) {
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
 * O(N*M) where N is sizeX and M is sizeY
 * 
 * @param highMin The minimum value that is "high" (inclusively)
 * @return randomly choosen x, y cords where graph[x][y] >= highMin
*/
pair<int, int> randHighPoint(double highMin) {
    
    vector<pair<int,int>> highPoints;
    highPoints.clear();
    for(int i = 0; i < SIZEX; i++) {
        for(int j = 0; j < SIZEY; j++) {
            if (graph[i][j] >= highMin) {
                highPoints.push_back({i, j});
            }
        }
    }
    
    return highPoints[randInt(0, highPoints.size())];
}

/**
 * O(N*M) where N is sizeX and M is sizeY
 * 
 * @param lowMin The minimum value that is "low"
 * @return randomly choosen x, y cords where graph[x][y] <= highMin
*/
pair<int, int> randLowPoint(double lowMin, int SIZEX, int SIZEY) {
    
    vector<pair<int,int>> lowPoints;
    lowPoints.clear();
    for(int i = 0; i < SIZEX; i++) {
        for(int j = 0; j < SIZEY; j++) {
            if (graph[i][j] <= lowMin) {
                lowPoints.push_back({i, j});
            }
        }
    }
    
    return lowPoints[randInt(0, lowPoints.size())];
}

/**
 * Time complexity of O(N) Where N is the total number of pixels
 * Doesn't include (x, y) if (x, y) is a local min
 * 
 * @param x, y Finds a local minimum close to (x, y) 
 * @param range Chooses a point that is between the 1st and rangeth closest point.
 * @param SIZEX The x size of the simulation
 * @param SIZEY The y size of the simulation
 * @param maximumMin The maximum value of the min
 * @return randomly choosen x, y cords where graph[x][y] <= highMin
 *         -1, -1 if there is no valid points
*/

pair<int, int> closeLocMin(int x, int y, int range, int SIZEX, int SIZEY, double maximumMin) {
    // Distance to (x, y), and the cords of the local min
    vector<pair<double, pair<int,int>>> locMin;
    locMin.clear();
    for(int i = 0; i < SIZEX; i++) {
        for(int j = 0; j < SIZEY; j++) {
            if(graph[i][j] > maximumMin) continue;
            bool isLocalMin = true;
            for(auto[cx, cy]: CARDINAL_DIRECTIONS) { // 4 directional movement
                if(outOfBounds(i + cx, j + cy, SIZEX, SIZEY)) continue;
                if(graph[i][j] >= graph[i + cx][j + cy]) isLocalMin = false;
            }
            if (isLocalMin && (i != x && j != y)) {
                locMin.push_back({getDist(x,y,i,j),{i, j}});
            }
        }
    }
    sort(locMin.begin(),locMin.end());
    if(!locMin.size()) return {-1, -1};
    // DEBUGGING
    // cout << "Local mins for " << x << ", " << y << " max height of min " << maximumMin << endl;
    // for(int i = 0; i < locMin.size(); i++) {
    //     cout << "Dist " << locMin[i].first << " x: " <<  locMin[i].second.first << " y: " << locMin[i].second.second << endl;
    // }
    return locMin[randInt(0, min(range, (int)locMin.size()))].second;
}


/**
 * Time complexity of O(N) Where N is the total number of pixels
 * Does include (x, y) if (x, y) is a local max (because of how I'm currently using it)
 * 
 * @param x, y Finds a local maximum close to (x, y) 
 * @param range Chooses a point that is between the 1st and rangeth closest point.
 * @param SIZEX The x size of the simulation
 * @param SIZEY The y size of the simulation
 * @param minimumMax The minimum value of the max
 * @return randomly choosen x, y cords where graph[x][y] <= highMin
*/

pair<int, int> closeLocMax(int x, int y, int range, int SIZEX, int SIZEY, double minimumMax) {
    // Distance to (x, y), and the cords of the local min
    vector<pair<double, pair<int,int>>> locMax;
    locMax.clear();
    for(int i = 0; i < SIZEX; i++) {
        for(int j = 0; j < SIZEY; j++) {
            bool isLocalMax = true;
            if(graph[i][j] < minimumMax) continue;
            for(auto[cx, cy]: CARDINAL_DIRECTIONS) { // 4 directional movement
                if(outOfBounds(i + cx, j + cy, SIZEX, SIZEY)) continue;
                if(graph[i][j] <= graph[i + cx][j + cy]) isLocalMax = false;
            }
            if (isLocalMax) {
                locMax.push_back({getDist(x,y,i,j),{i, j}});
            }
        }
    }
    sort(locMax.begin(),locMax.end());
    if(!locMax.size()) return {-1, -1};
    
    return locMax[randInt(0, min(range, (int)locMax.size()))].second;
}


/**
 * Not in use, not finished, depreciated!!!
 * Makes a lake at the end of rivers.
 * @param x, y The starting coordinates of the lake
*/
void createLake(int x, int y) {
    const int MAX_LAKE_SIZE = 100;
    const int MAX_CHANGE_DECREASE = 0.05;
    const int MAX_CHANGE_INCREASE = 0.01;
    const int LAKE_SINKAGE = 0.3;
    int lakeSize = 0;
    queue<pair<int, int>> qu;
    qu.push({x, y});
    while(!qu.empty()) {
        if(lakeSize >= MAX_LAKE_SIZE) break;
        auto [x, y] = qu.front();
        qu.pop();
        for (auto [changeX, changeY]: MOVEMENT_OPTIONS) {
            int x2 = x + changeX, y2 = y + changeY;
            if(outOfBounds(x2, y2, SIZEX, SIZEY)) continue;
            if(!inRange(graph[x2][y2], graph[x][y] - MAX_CHANGE_DECREASE, graph[x][y] + MAX_CHANGE_INCREASE)) continue;
            if(inRange(biome[x2][y2], 4, 5)) continue; // If it's a lake or a ocean stop
            biome[x2][y2] = LAKE_NUMBER;
            lakeSize++;
            qu.push({x2, y2});
        }
        graph[x][y] = max(graph[x][y] * 0.4, graph[x][y] - LAKE_SINKAGE);
    }
    cout << "Created lake at the end of a river with size " << lakeSize << endl;
    
}


/**
 * Creates a river at a random place.
 * It uses a "flow down" type algorithm, going down the path until it can't go down anymore
*/
void stream() {
    auto [x, y] = randHighPoint(0.8);
    // cout << "Starting " << graph[x][y] << endl;
    while(biome[x][y] != 4 && biome[x][y] != 5) { 
        river[x][y] = 1;
        double lowest = graph[x][y];
        int lowestX = -INF, lowestY = -INF;
        for (auto move: MOVEMENT_OPTIONS) {
            int x2 = x + move[0], y2 = y + move[1];
            if(outOfBounds(x2, y2, SIZEX, SIZEY)) continue;
            if(graph[x2][y2] < lowest) {
                lowestX = x2;
                lowestY = y2;
                lowest = graph[x2][y2];
            }
        }
        if(lowest == graph[x][y]) {
            cout << "Error!!!! " << "wtf" << endl;
            createLake(x, y);
            return;
        }
        if (river[lowestX][lowestY] == 1) {
            cout << "error " << endl;
            break;
        }
        river[lowestX][lowestY] = 1;
        x = lowestX;
        y = lowestY;
        
    }
}

/**
 * A custom struct to hold values for creating a river, using in stream2()'s prio queue.
 * 
 * @param x,y The coordinates
 * @param dist The distance travelled so far (1 for each )
 * @param cost
 * 
*/
struct queueItem {
    int x; // x coordinate
    int y; // y coordinate
    double dist; // Distance travelled so far
    double cost; // Distance + Heuristic value
    bool operator<(const queueItem &other) const {
        return cost > other.cost;
    }
};


/**
 * Using A* search to generate rivers, heuristics being height change, and distance to end point
 * 
 * @param startXPos, startYPos The starting coordinates of the river, enter -1, -1 to start at a random high point.
 * @param endXPos, endYPos The ending coordinates of the river, enter -1, -1 to end at the closest low point.
*/
// Debugging
void stream2(int startXPos, int startYPos, int endXPos, int endYPos, double TRAVELLED_WEIGHT = 0.1, double DIST_WEIGHT = 1, double MAX_HEIGHT_CLIMB = 1) {
    // cout << "Creating River "<< startXPos<< " " << startYPos << " to " << endXPos << " " << endYPos << endl;
    // Log file | LOGGING
    ofstream riverLogs;
    riverLogs.open("riverLogs.txt");

    riverLogs << "Creating River " << endl; // LOGGING

    // DEBUGGING
    cout << "attempt to make a river " << endl;
    
    // const double TRAVELLED_WEIGHT = 0.1; // The weight value of the distance the path has travelled
    // const double DIST_WEIGHT = 1; // The weight value of the distance to the goal
    // const double MAX_HEIGHT_CLIMB = 1; // The maximum amount the river can go up.

    bool vis[1000][1000]; // change to a constant/vector and make it a parameter later.
    memset(vis, false, sizeof vis); // Important

    // auto[startX, startY] = randHighPoint(0.95); // TODO: Change to constants
    int startX = startXPos, startY = startYPos;
    if(startXPos < 0 && startYPos < 0) {
        pair<int, int> cords = closeLocMax(0, 0, 1e6, SIZEX, SIZEY, SAND_MAX_HEIGHT); // Gets any local maximum
        startX = cords.first;
        startY = cords.second;
    }

    vis[startX][startY] = true;
    
    int endX = endXPos, endY = endYPos;
    if(endXPos < 0 && endYPos < 0) {
        pair<int, int> cords = closeLocMin(startX, startY, 1, SIZEX, SIZEY, graph[startX][startY]); // Gets the closest local min
        endX = cords.first;
        endY = cords.second;
    }

    // The start is invalid 
    if(startX < 0 || startY < 0 || startX >= SIZEX || startY >= SIZEY) {
        cout << "Error: couldn't find a proper start. Start: (" << startX << ", " << startY << ") End: (" << endX << ", " << endY << ")" << endl;
        return;
    }

    // There is no min
    if(endX < 0 || endY < 0 || endX >= SIZEX || endY >= SIZEY) {
        cout << "Error: out of bounds on end point: END = " << endX << ", " << endY << endl;
        // cout << "Nothing lower than " << graph[startX][startY] << endl;
        return;
    }

    // DEBUGGING
    cout << "Passed validilty checks! start = " << startX << ", " << startY << " end: " << endX << ", " << endY << endl;  


    priority_queue<queueItem> pq;
    pq.push((queueItem){startX, startY, 0, getDist(startX, startY, endX, endY)});
    int riverSize = 1;
    river[startX][startY] = 1;

    riverLogs << "Start " << startX << " " << startY << " Height: "; riverLogs << graph[startX][startY] << endl; // LOGGING
    riverLogs << "End " << endX << " " << endY <<  " Height: "; riverLogs << graph[endX][endY] << endl; // LOGGING

    if(startX == endX && startY == endY) {
        cout << "Error (stream2()): Start is the same as end\n" << endl;
        return;
    }

    while(!pq.empty()) {
        auto[x,y,dist,cost] = pq.top();
        riverLogs <<"(" << x << ", " << y << ") dist: " << dist << " cost: " << cost << endl; // LOGGING
        // cout << x << " " << y << " cost factor: " << cost << endl; 
        if(x == endX && y == endY) break; // ended
        pq.pop();
        
        for(auto [changeX, changeY]: CARDINAL_DIRECTIONS) {
            int x2 = changeX + x;
            int y2 = changeY + y;
            if(outOfBounds(x2,y2,SIZEX,SIZEY)) continue;
            if(vis[x2][y2]) continue;
            // Testing // CHANGE LATER
            // if(graph[x2][y2] - graph[x][y] > MAX_HEIGHT_CLIMB) continue;

            vis[x2][y2] = true;
            river[x2][y2] = 1;
            riverSize++;
            double newDist = dist + abs(graph[x][y] - graph[x2][y2]) + 1;
            // Increase distance dramatically if it goes up instead of down, based on distance // CHANGE LATER
            if(graph[x2][y2] - graph[x][y] < 0) newDist += abs(graph[x2][y2] - graph[x][y]) * getDist(x2,y2,endX,endY) * 15;
            double newCost = newDist * TRAVELLED_WEIGHT + getDist(x2,y2,endX,endY) * DIST_WEIGHT;
            pq.push((queueItem){x2, y2, newDist, newCost});
        }
        if(riverSize > 25000) { // Rivers should never get this big.
            cout << "Flooded!" << endl;
            cout << "Start " << startX << " " << startY << " Height: " << graph[startX][startY] << endl;
            cout << "End " << endX << " " << endY <<  " Height: " << graph[endX][endY] << endl;
            cout << "qu top " << pq.top().x << " " << pq.top().y << " " << pq.top().dist << " " << pq.top().cost << endl;
            break;
        }
    }
    cout << "Created a river with size " << riverSize << endl;
    if(!vis[endX][endY]) cout << "Failed to end the river" << endl;

    riverLogs.close(); // LOGGING

    // If the river didn't go into water, continue it
    // if(graph[endX][endY] > WATER_LEVEL) {
        // cout << "Continuing river " << graph[endX][endY] << " at: " << endX << " " << endY << endl;
        // Continuing the river crashes sometimes
        // stream2(endX, endY, -1, -1, TRAVELLED_WEIGHT, DIST_WEIGHT, MAX_HEIGHT_CLIMB);
    // }
    
}

/**
 * 
 * @param startX, startY The starting pos of the river
*/
// Change constants and remove them from params once constants look good
void riverHelper(int startX, int startY, int endX, int endY, int DIST_PER_BEND = 20, double BEND_NOISE = .3, double TRAVELLED_WEIGHT = 0.1, double DIST_WEIGHT = 1, double MAX_HEIGHT_CLIMB = 1) {
    if (startX <= -1 && startY <= -1) {
        pair<int, int> cords = closeLocMax(0, 0, 1e6, SIZEX, SIZEY, SAND_MAX_HEIGHT); // Gets any local maximum
        startX = cords.first;
        startY = cords.second;
    } 
    if (endX <= -1 && endY <= -1) {
        // Closest
        pair<int, int> cords = closeLocMin(startX, startY, 3, SIZEX, SIZEY, SAND_MAX_HEIGHT); // Gets any local minimum
        endX = cords.first;
        endY = cords.second;
    }
    // const int DIST_PER_BEND = 20; // there will be 1 turn per DIST_PER_BEND distance, floored
    // const double BEND_NOISE = .30; // The amount the river can shift
    double distance = getDist(startX, startY, endX, endY);  // The edulicean distance from start to end
    int numBends = floor(distance / DIST_PER_BEND); // The number of bends in the river
    int maxNoiseVal = floor(distance * BEND_NOISE); // The max amount the river can shift in any direction.
    cout << "\nRIVER HELPER (" << startX << ", " << startY << "): " << graph[startX][startY] << " to (" << endX << ", " << endY << "): " << graph[endX][endY] << endl;

    auto[slope, intercept] = getLineEquation(startX, startY, endX, endY);
    int direction = 1;
    if(startX > endX) direction = -1;
    double percentage = 1 / (double)numBends; // This formula was provided by Backgamemon, implemented by me
    vector<pair<int, int>> points; // all the points where the river will touch
    
    cout << "NUM BENDS " << numBends << endl;
    points.push_back({endX, endY});
    for(int i = 1; i < numBends; i++) {
        double nextX = i * percentage * startX + (1-(percentage * i)) * endX;
        double nextY = i * percentage * startY + (1-(percentage * i)) * endY;
        // DEBUGGING
        // cout << percentage*i << " * " << startY << " + 1-" <<  percentage*i << " *  " << endY << endl;
        // cout << (percentage * i * startY) << " + " << ((1-(percentage*i)) * endY) << endl;
        // cout << "NEXT " << nextX << ", " << nextY << endl;

        int noiseX = randInt(-DIST_PER_BEND * BEND_NOISE, DIST_PER_BEND * BEND_NOISE);
        int noiseY = randInt(-DIST_PER_BEND * BEND_NOISE, DIST_PER_BEND * BEND_NOISE);
        nextX = keepInBounds(round(nextX + noiseX), 0, SIZEX);
        nextY = keepInBounds(round(nextY + noiseY), 0, SIZEY);
        cout << "NEXT FINAL " << nextX << ", " << nextY << endl;
        points.push_back({nextX, nextY});
    }
    points.push_back({startX, startY});

    for(int i = 1; i < points.size(); i++) {
        auto [x1, y1] = points[i-1];
        auto [x2, y2] = points[i]; 
        stream2(x1, y1, x2, y2, TRAVELLED_WEIGHT, DIST_WEIGHT, MAX_HEIGHT_CLIMB);
    }
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
 * @param sizeX The length of the map (x value)
 * @param sizeY The height of the map (y value)
*/
void defineBiomes(int sizeX, int sizeY) {
    // Sinusoidal noise gen end
    for(int x = 0; x < sizeX; x++) {
        for(int y = 0; y < sizeY; y++) {
            if(biome[x][y] == 4 || biome[x][y] == 5) continue;
            // If it's not water then continue
            if(graph[x][y] > LAND_MAX_HEIGHT) biome[x][y] = 1;
            else if(graph[x][y] > SAND_MAX_HEIGHT) biome[x][y] = 2;
            else if(graph[x][y] > WATER_LEVEL) biome[x][y] = 3;
            else {
                defineBodyOfWater(x, y, WATER_LEVEL, biome);
            }
        }
    }
    cout << "Finished defining biomes. " << endl;
}


void updDebuggingGroup() {

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
    std::sort(k, k+COUNT);
    // Debug: prints all waves
    // for(int i = 0; i < COUNT; i++) cout << k[i]<< "sin("<<a[i]<<"(x -"<<b[i]<<")) + "<< k[i]<< "(sin("<<a[i]<<"(y -"<<b2[i]<<")) + "<<c[i]<<endl;



    double minVal = INF, maxVal = -INF; 
    // Testing a large amount of values (not all) to find the min and max to normalize vales to between 0 and 1
    for(int x = 0; x < SIZEX; x++) {
        for(int y = 0; y < SIZEY; y++) {
            double tot = 0;
            for(int i = 0; i < COUNT; i++) {
                // Previous attempts at getting noise
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
    for(int x = 0; x < SIZEX; x++) {
        for(int y = 0; y < SIZEY; y++) {
            double tot = 0;
            for(int i = 0; i < COUNT; i++) {
                // Previous attempts at getting noise
                // tot += (k[i] * sin(a[i] * (x - midX - b[i])) + (k[i] * sin(a[i] * (y - midY - b2[i])) + c[i])) / (double)COUNT;
                // tot += (k[i] * sin(a[i] * (x*stepper - midX - b[i])) + (k[i] * sin(a[i] * (y*stepper - midY - b2[i])) + c[i])) / (double)COUNT;
                // tot += (k[i] * sin(a[i]*(x*stepper - midX - b[i] + a[i]*(y*stepper - midY - b2[i]))) + c[i]) / (double)COUNT;
                // tot += (k[i] * sin(a[i]*(x*stepper - midX - b[i] + a[i]*(y*stepper - midY - b2[i]))) + c[i]) / (double)(i + 1);
                
                // graph[x][y] += (k[i] * sin(a[i] * (x - midX - b[i])) + (k[i] * sin(a[i] * (y - midY - b[i])) + c[i]));
                tot += (k[i]*sin(a[i]*(x*stepper*cos(rotation[i])-y*stepper*sin(rotation[i])-b[i])) + k[i]*sin(a[i]*(x*stepper*sin(rotation[i])+y*stepper*cos(rotation[i])-b2[i]))) / (double)(i + 1);
            }
            // tot = abs(tot);
            graph[x][y] = (tot - minVal) / (maxVal - minVal);
        }
    }

    

    // Biome gen start
    defineBiomes(SIZEX, SIZEY);

    const int DEBUG_SIZE = 200;
    // create window
    sf::RenderWindow window(sf::VideoMode(WINDOW_SIZE_X + DEBUG_SIZE, WINDOW_SIZE_Y), "Noise", sf::Style::Default);
    window.setActive(); // activate
    // Set the color and depth clear values
    glClearDepth(1.f);
    glClearColor(0.f, 0.f, 0.f, 1.f); // color
    // Configure the viewport (the same size as the window)
    glViewport(0, 0, window.getSize().x, window.getSize().y);

    Box boxTest(WINDOW_SIZE_X, 0, WINDOW_SIZE_X + 150, WINDOW_SIZE_Y/2, sf::Color(255,0,0), "Testing: ", "123.3462", 10, "arial.ttf");
    FText textTest(WINDOW_SIZE_X,WINDOW_SIZE_Y/2, "Testing 123.3462", "arial.ttf");
    

    int DIST_PER_BEND = 20;
    double BEND_NOISE = 0.3;
    double TRAVELLED_WEIGHT = 0.1;
    double DIST_WEIGHT = 1;
    double MAX_HEIGHT_CLIMB = 1;
    BoxGroup debuggingGroup(WINDOW_SIZE_X+3, 3, 200, 40);
    vector<Box> temp = {
        Box(sf::Color(240, 231, 230), "Distance per Bend: ", to_string(DIST_PER_BEND), 10, "arial.ttf"),
        Box(sf::Color(240, 231, 230), "Noise Percentage: ",  to_string(BEND_NOISE), 10, "arial.ttf"),
        Box(sf::Color(240, 231, 230), "River Height Weight: ",  to_string(TRAVELLED_WEIGHT), 10, "arial.ttf"),
        Box(sf::Color(240, 231, 230), "River Distance Weight: ",  to_string(DIST_WEIGHT), 10, "arial.ttf"),
        Box(sf::Color(240, 231, 230), "River Max Height increase: ", to_string(MAX_HEIGHT_CLIMB), 10, "arial.ttf")
    };
    for(int i = 0; i < temp.size(); i++) {
        debuggingGroup.add(&temp[i]);
    }

    // Declaring the square that is used to draw the pixels
    sf::RectangleShape square(sf::Vector2f(100, 100)); // sets (x, y)
    // square.setSize(); // changes the size

    // different viewing modes
    int TOPO_MODE = 0, BIOME_MODE = 1, HEIGHT_MAP_MODE = 2;
    int currentMode = 0, numModes = 3;
    Box *smtSelected = NULL; // The text box that is currently selected

    // Start the loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    debuggingGroup.destory(); // To not leak memory
                    window.close();
                    break;

                case sf::Event::Resized:
                    glViewport(0, 0, event.size.width, event.size.height);
                    break;

                case sf::Event::KeyPressed:
                    if(smtSelected != NULL) break; // if nothing is selected
                    switch(event.key.code) {
                        case sf::Keyboard::Escape: // Escape key: exit
                            debuggingGroup.destory(); // To not leak memory
                            window.close();
                            break;
                        
                        case sf::Keyboard::V: // Iterate viewing modes
                            currentMode = (currentMode + 1) % numModes;
                            break;

                        case sf::Keyboard::S: // DEBUG: Create a stream (just use R)
                            stream2(-1,-1, -1, -1, TRAVELLED_WEIGHT, DIST_WEIGHT, MAX_HEIGHT_CLIMB);
                            break;

                        case sf::Keyboard::R: // Create a river
                            riverHelper(-1,-1, -1, -1, DIST_PER_BEND, BEND_NOISE);
                            break;

                    }
                break;
            }
            // Put all keyboardInput functions here
            if(event.type == sf::Event::TextEntered) {
                if(!debuggingGroup.keyboardInput(event)) smtSelected = NULL;
                // boxTest.keyboardInput(event);
            }
            // Put all selected functions here 
            if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                smtSelected = debuggingGroup.clickedScan(sf::Mouse::getPosition(window));
                // boxTest.clickedScan(sf::Mouse::getPosition(window));
            }
        }
        // Reset the frame
        window.clear();

        square.setSize(sf::Vector2f(STEP_SIZE_X, STEP_SIZE_Y));
        // cout << (int)(window.getSize().x / STEP_SIZE_X) << " " << (int)(window.getSize().y / STEP_SIZE_Y) <<endl;

        for(int x = 0; x < SIZEX; x++) {
            for(int y = 0; y < SIZEY; y++) {
                square.setPosition(sf::Vector2f(x * STEP_SIZE_X, y*STEP_SIZE_Y));

                if (currentMode == TOPO_MODE) square.setFillColor(worldlyColor(x, y));
                if (currentMode == BIOME_MODE) square.setFillColor(biomeColoring(x, y));
                if (currentMode == HEIGHT_MAP_MODE)square.setFillColor(grayScale(x, y));

                window.draw(square);
            }
        }
        // textTest.draw(window);
        debuggingGroup.draw(window);
        // boxTest.draw(window);

        // end the current frame
        window.display();
    }


    return 0;
}