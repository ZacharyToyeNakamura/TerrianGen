// A file that contains utility functions that are sometimes useful.

#ifndef UTILFUNCTIONS_H
#define UTILFUNCTIONS_H

#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <queue>

#include "constants.h"

using namespace std;

/**
 * Returns if num1 is closer to zero than num2
 * 
 * @param num1 A number to be compared
 * @param num2 A second number to be compared
 * @return true if num1 is closer to zero than num2, otherwise false.
*/
bool closerToZero(double num1, double num2) {
    return abs(num1) < abs(num2);
}

/**
 * Generates a random double between lowerLim and upperLim
 * @param upperLim The upper limit for the random double
 * @param lowerLim The lower limit for the random double
 * @return A random double.
 * 
 * TODO: Change the constant that divides it to a prime that is > 1e9
*/
double randDouble(double lowerLim, double upperLim) {
    // The % number and / number must be the same for best results.
    // Can't be 1e5 because it just breaks probably due to not enough space (no long long)
    // If there is a way change to ~1e9 or smt (long long didn't work)
    return lowerLim + ((upperLim - lowerLim) * (rand() % 19354)) / 19354.0; // All brackets are necessary
}


/**
 * Generates a random int between lowerLim and upperLim
 * @param upperLim The upper limit for the random int
 * @param lowerLim The lower limit for the random int
 * @return A random int because the 2 limits, or 0 if the lower lim is greater than the upper limit
*/
double randInt(int lowerLim, int upperLim) {
    if(lowerLim > upperLim) return 0;
    if(upperLim - lowerLim == 0) return upperLim;
    return lowerLim + (rand() % (upperLim - lowerLim)); 
}


/**
 * Checks if a value n is inside of an interval
 * 
 * @return if n is between lowerBound and upperBound (Note that it's inclusive).
*/
bool inRange(int n, int lowerBound, int upperBound) {
    return lowerBound <= n && n <= upperBound;
}


/**
 * Checks if a coordinate is within certain bounds.
 * 
 * @param x, y A coordinate to check if it's between [0, 0] and (limitX and limitY).
 * @param limitX, limitY The maximum value exclusively x and y can be.
 * @return true if x or y are out of bounds 0 <= x < maxX && 0 <= y < maxY
*/
bool outOfBounds(int x, int y, int limitX, int limitY) {
    return 0 > x || x >= limitX || 0 > y || y >= limitY;
}

/**
 * Calculates the euclidean distances between 2 sets of coordinates
 * 
 * @param x1, y1 One set of coordinates
 * @param x2, y2 A second set of coordinates
 * @return The euclidean distance between (x1, y1) and (x2, y2)
*/
double getDist(double x1, double y1, double x2, double y2) {
    return sqrt(pow(x1-x2,2)+pow(y1-y2,2));
}


/**
 * Given a value x, and an upper and lower bound. It returns a value that is between the lower and upper
 * bounds.
 * 
 * @param x The value being checked
 * @param lowerBound The lowest value x can be
 * @param upperBound The maximum value x can be
 * 
 * @returns lowerBound if x is less than lowerBound, upperBound if x is greater than upperBound, otherwise x
*/
int keepInBounds(int x, int lowerBound, int upperBound) {
    return min(max(x, lowerBound), upperBound);
}


/**
 * 
 * @return (m, b) and gets the line equation y = mx + b
*/ 
pair<double, double> getLineEquation(double x, double y, double x2, double y2) {
    double slope = (x2 - x)/(y2 - y);
    double yInt = y - slope*x;
    return {slope, yInt};
}


#endif