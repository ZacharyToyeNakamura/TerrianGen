**This project is still in progress.** 


noise.cpp is my attempt at some simple terrian generation without perlin noise.
main_sfml.cpp is a old attempt at simulating a population.
I hope to one day simulate a population on generated terrain.
<br/>
Generating terrian
Terrian is generated using the sum of random 2d sinusoidal functions that are rotated.
Rivers are being generated using A* with heuristics for height change, distance travelled and distance to the end. (A lot of constants need adjustment before it looks good)
The time complexity is horrible, so it takes a while to load.


This requires SFML library to work.
Why is the whole sfml library, dll files and other things here?
I didn't set up it up properly so it only compiles when the libraries and dll files are in the same folder.
I'll attempt to set up sfml compiling properly and remove the library later perhaps.