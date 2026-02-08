#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <cstdint>

static SDL_Window *gameWindow = nullptr;
static SDL_Renderer *gameRenderer = nullptr;

constexpr int screenWidth = 640;
constexpr int screenHeight = 480;

uint64_t framerate = 60;

// functions


// runs on startup
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]){
    // initialize sdl
    if (!SDL_Init(SDL_INIT_VIDEO)){
        SDL_Log("could not initialize sdl! SDL Error: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    
    // create window and renderer
    if(!SDL_CreateWindowAndRenderer("s3t - super tic tac toe", screenWidth, screenHeight, SDL_WINDOW_MOUSE_FOCUS, &gameWindow, &gameRenderer)){
        SDL_Log("could not create window and renderer! SDL Error: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // retireive framerate info
    const SDL_DisplayMode* displayMode = SDL_GetCurrentDisplayMode(SDL_GetPrimaryDisplay());
    if (displayMode->refresh_rate > 0){
        framerate = displayMode->refresh_rate;
    }

    return SDL_APP_CONTINUE;
}

// handles events
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event){
    // close event
    if (event->type == SDL_EVENT_QUIT) return SDL_APP_SUCCESS;

    // mouse input



    return SDL_APP_CONTINUE;
}

// main game loop runs once per frame
SDL_AppResult SDL_AppIterate(void *appstate){
    // Drawing the tic tac toe lines
    SDL_SetRenderDrawColor(gameRenderer, 240, 240, 240, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(gameRenderer);

    SDL_SetRenderDrawColor(gameRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderLine(gameRenderer, 20, 20, 20, 100);
    SDL_RenderLine(gameRenderer, 20, 20, 100, 20);
    SDL_RenderLine(gameRenderer, 100, 20, 100, 100);
    SDL_RenderLine(gameRenderer, 100, 100, 20, 100);

    SDL_RenderPresent(gameRenderer);

    return SDL_APP_CONTINUE;
}

// cleanup
void SDL_AppQuit(void *appstate, SDL_AppResult){
    // sdl destroys window and renderer for us
}

int main(int argc, char *argv[]){
    return SDL_EnterAppMainCallbacks(argc, argv, SDL_AppInit, SDL_AppIterate, SDL_AppEvent, SDL_AppQuit);
}

