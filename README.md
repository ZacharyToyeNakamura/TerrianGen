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


## To Run on Windows
Git clone this repository and run noise.exe Or download the noise folder from https://drive.google.com/drive/folders/1m-Mem0uGOqExzRxV54LXimDjegm22UmH?usp=sharing and run noise.exe 


## To Run on Mac
Uhhhh yea no... you can't (for now)

The commands are as follows:
You can click on the boxes and edit the values, hit Enter to confirm. 
V - Changes view mode (Normal, Biomes, and gray scale height map)
R - Creates a river (Constants are based on the one set in the boxes on the right)
M - Generates a new map

S - Creates a prototype river (Not good)


## TODO
- Waiting for backgamemon to find good constants for river generation
