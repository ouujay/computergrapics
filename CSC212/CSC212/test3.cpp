#include "test3.h"
#include <SDL.h>
#include <stdio.h>
#include <SDL_image.h>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int BALL_RADIUS = 100;
const int BALL_SPEED = 10;

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

struct Ball {
    int x, y;
    int dx, dy;
};

Ball ball;

SDL_Texture* loadTexture(const std::string& path) {
    // The final texture
    SDL_Texture* newTexture = NULL;

    // Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL) {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
    }
    else {
        // Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if (newTexture == NULL) {
            printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        }

        // Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }

    return newTexture;
}

bool initSDL()
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    // Create window
    gWindow = SDL_CreateWindow("2D Rendering", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (gWindow == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    // Create renderer
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (gRenderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    // Set renderer color
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

    // Allows for (Initializes) Image Loading abilities using SDL_image library
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return false;
    }

    return true;
}

void closeSDL()
{
    // Destroy window
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;

    // Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}

void updateBall() {
    // Move the ball
    ball.x += ball.dx;
    ball.y += ball.dy;

    // Bounce off the edges if the ball goes beyond the window boundaries
    if (ball.x < 0 || ball.x + BALL_RADIUS > SCREEN_WIDTH) {
        ball.dx *= -1;
    }
    if (ball.y < 0 || ball.y + BALL_RADIUS > SCREEN_HEIGHT) {
        ball.dy *= -1;
    }
}

int main(int argc, char* argv[]) {
    // Seed the random number generator
    srand(time(NULL));

    if (!initSDL()) {
        printf("Failed to initialize SDL!\n");
        return -1;
    }

    SDL_Texture* ballText = loadTexture("./Assets/ball.jpg");
    ball.x = SCREEN_WIDTH / 2;
    ball.y = SCREEN_HEIGHT / 2;
    ball.dx = (rand() % (2 * BALL_SPEED)) - BALL_SPEED; // Random initial velocity in x direction
    ball.dy = (rand() % (2 * BALL_SPEED)) - BALL_SPEED; // Random initial velocity in y direction

    // Main loop flag
    bool quit = false;

    // Event handler
    SDL_Event e;

    while (!quit) {
        // Handle events on queue
        while (SDL_PollEvent(&e) != 0) {
            // User requests quit
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        // Clear screen
        SDL_RenderClear(gRenderer);

        updateBall();

        SDL_Rect ballRect = { ball.x, ball.y, BALL_RADIUS, BALL_RADIUS };
        SDL_RenderCopy(gRenderer, ballText, NULL, &ballRect);

        // Update screen
        SDL_RenderPresent(gRenderer);
        SDL_Delay(10);
    }

    // Destroy textures
    SDL_DestroyTexture(ballText);

    // Close SDL
    closeSDL();

    return 0;
}
