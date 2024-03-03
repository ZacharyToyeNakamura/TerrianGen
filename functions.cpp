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

// Given the chance and (a max)
bool random(float chance, float max = 100) {
    // more precision
    if (chance * 100 >= (float)(rand() % (int)(max * 100))) return true;
    return false;
}

