// Run using 
// g++ -I src/include -L src/lib -o main test.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
// g++ -I src/include -L src/lib -o main test.cpp -lmingw32 -lSDL2main -lSDL2 
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <stdio.h>
#include <iostream>

// render
void blit (SDL_Texture *texture, int x, int y) {
	SDL_Rect dest;

	dest.x = x;
	dest.y = y;
	SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);

	// SDL_RenderCopy(rend, texture, NULL, &dest);
}


int main(int argc, char *argv[]) {
	int WIDTH = 400;
	int HEIGHT = 500;
	std::cout << "Staring";
	// returns zero on success else non-zero
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("error initializing SDL: %s\n", SDL_GetError());
	}
	SDL_Window* win = SDL_CreateWindow("Population Simulation", // creates a window
									SDL_WINDOWPOS_CENTERED, // center the window
									SDL_WINDOWPOS_CENTERED, // center the window
									WIDTH, HEIGHT, 0); // size

	// triggers the program that controls, your graphics hardware and sets flags
	Uint32 render_flags = SDL_RENDERER_ACCELERATED;

	// creates a renderer to render our images
	SDL_Renderer* rend = SDL_CreateRenderer(win, -1, render_flags);

	// creates a surface to load an image into the main memory
	SDL_Surface* surface;

	// please provide a path for your image
	surface = IMG_Load("C:\\Users\\zacha\\Documents\\vsgit\\redcircle.png");

	// loads image to our graphics hardware memory.
	SDL_Texture* tex = SDL_CreateTextureFromSurface(rend, surface);

	// clears main-memory
	SDL_FreeSurface(surface);

	// let us control our image position
	// so that we can move it with our keyboard.
	SDL_Rect dest;

	// connects our texture with dest to control position
	SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);

	// adjust height and width of our image box.
	dest.w /= 6;
	dest.h /= 6; 

	// sets initial x-position of object
	dest.x = (WIDTH - dest.w) / 2;

	// sets initial y-position of object
	dest.y = (HEIGHT - dest.h) / 2;

	// controls animation loop
	int close = 0;

	// speed of box
	int speed = 300;
	std::cout << "animation";
	// animation loop
	while (!close) {
		SDL_Event event;

		// Events management
		while (SDL_PollEvent(&event)) {
			switch (event.type) {

			case SDL_QUIT:
				// handling of close button
				close = 1;
				break;

			case SDL_KEYDOWN:
				// keyboard API for key pressed
				switch (event.key.keysym.scancode) {
				case SDL_SCANCODE_W:
				case SDL_SCANCODE_UP:
					std::cout << "up" << std::endl;
					dest.y -= speed / 30;
					break;
				case SDL_SCANCODE_A:
				case SDL_SCANCODE_LEFT:
					std::cout << "left" << std::endl;
					dest.x -= speed / 30;
					break;
				case SDL_SCANCODE_S:
				case SDL_SCANCODE_DOWN:
					std::cout << "down" << std::endl;
					dest.y += speed / 30;
					break;
				case SDL_SCANCODE_D:
				case SDL_SCANCODE_RIGHT:
					std::cout << "right" << std::endl;
					dest.x += speed / 30;
					break;
				default:
					break;
				}
			}
		}

		// right boundary
		if (dest.x + dest.w > WIDTH)
			dest.x = WIDTH - dest.w;

		// left boundary
		if (dest.x < 0)
			dest.x = 0;

		// bottom boundary
		if (dest.y + dest.h > HEIGHT)
			dest.y = HEIGHT - dest.h;

		// upper boundary
		if (dest.y < 0)
			dest.y = 0;

		// clears the screen
		SDL_RenderClear(rend);
		SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);
		SDL_RenderCopy(rend, tex, NULL, &dest);

		// triggers the double buffers
		// for multiple rendering
		SDL_RenderPresent(rend);

		// calculates to 60 fps
		SDL_Delay(WIDTH / 60);
	}

	// destroy texture
	SDL_DestroyTexture(tex);

	// destroy renderer
	SDL_DestroyRenderer(rend);

	// destroy window
	SDL_DestroyWindow(win);
	
	// close SDL
	SDL_Quit();

	return 0;
}



/*
// Run using 
// g++ -I src/include -L src/lib -o main test.cpp -lmingw32 -lSDL2main -lSDL2
// how to run cpp files
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>

int main(int argc, char *argv[]){

    // returns zero on success else non-zero
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("error initializing SDL: %s\n", SDL_GetError());
    }
    SDL_Window* win = SDL_CreateWindow("Population sim",
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       1000, 1000, 0);
    while (1); 

    return 0;
}

*/