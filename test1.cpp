#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_time.h>
#include <cstdint>
#include <string>

using namespace std;

constexpr int screenWidth {640};
constexpr int screenHeight {480};

SDL_Window* gameWindow = nullptr;
SDL_Renderer* gameRenderer = nullptr;
SDL_Surface* img = nullptr;
SDL_Texture* img_T = nullptr;

int framerate = 60;

bool init(){
    // Initializing SDL library
    if (!SDL_Init(SDL_INIT_VIDEO)){
        SDL_Log("could not initialize SDL! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    // Creating a window
    gameWindow = SDL_CreateWindow("img - super-tic-tac-toe", screenWidth, screenHeight, SDL_WINDOW_MOUSE_FOCUS);
    // add error handling for SDL_WINDOW_MOUSE_FOCUS 
    if (gameWindow == nullptr){
        SDL_Log("window could not be created! SDL Error: %s\n", SDL_GetError());
        return false;
    }
    
    // retireive framerate info
    const SDL_DisplayMode* displayMode = SDL_GetCurrentDisplayMode(SDL_GetPrimaryDisplay());
    if (displayMode->refresh_rate > 0){
        framerate = displayMode->refresh_rate;
    }

    // Creating the game renderer
    gameRenderer = SDL_CreateRenderer(gameWindow, nullptr);
    if (gameRenderer == nullptr){
        SDL_Log("renderer could not be created! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    return true;
}

bool loadMedia(){
    string media_path = "../img.bmp";

    img = SDL_LoadBMP(media_path.c_str());
    if (img == nullptr){
        SDL_Log("could not load image! SDL Error: %s\n", SDL_GetError());
        img = SDL_LoadPNG("img_error.png");
        return false;
    }

    img_T = SDL_CreateTextureFromSurface(gameRenderer, img);

    return true;
}

void close(){
    SDL_DestroySurface(img);
    img = nullptr;

    SDL_DestroyWindow(gameWindow);
    gameWindow = nullptr;
    SDL_DestroyRenderer(gameRenderer);
    gameRenderer = nullptr;

    SDL_Quit();
}

int main(int argc, char* argv[]) {
    int exitCode {0};
    
    if( !init() ) exitCode = 1;
    if( !loadMedia() ) exitCode = 2;

    bool running = true;
    
    SDL_Event event;
    SDL_zero( event );

    int frame_delay = (int) 1000/framerate;

    while (running){
        uint64_t frameS = SDL_GetTicks();
        while( SDL_PollEvent( &event ) ){
            if (event.type == SDL_EVENT_QUIT) running = false;
        }

        SDL_SetRenderDrawColor(gameRenderer, 255, 255, 255, 255);
        SDL_RenderClear(gameRenderer);
        SDL_RenderTexture(gameRenderer, img_T, nullptr, nullptr);

        SDL_RenderPresent(gameRenderer);
        
        uint64_t frameT = SDL_GetTicks() - frameS;
        if (frame_delay > frameT) SDL_Delay(frame_delay-frameT);
    }

    close();
}
