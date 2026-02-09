#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <cstdint>
#include "iostream"
#include "cmath"
#include "string"
#include "algorithm"

using namespace std;

enum class GameState{
    mainMenu,
    playing,
    paused,
    XWinScreen,
    OWinScreen,
};


class game{
    private:
        GameState gameState = GameState::playing;

        // consts & declarations
        SDL_Window *gameWindow = nullptr;
        SDL_Renderer *gameRenderer = nullptr;
        SDL_WindowID gameWindowID;

        const int screenWidth = 900;
        const int screenHeight = 900;

        uint64_t framerate = 60;
        
        int move = 0;
        int gameData[9][9];

        bool running = false;
            
        
        // priv functions
        
        SDL_Texture* loadTexture(string path){
            // load texture from img
            SDL_Surface* img = nullptr;
            img = SDL_LoadPNG(path.c_str());
            if (img == nullptr){
                SDL_Log("couldn't load texture: %s; SDL_Error: %s \n", path.c_str(), SDL_GetError());
                img = SDL_LoadPNG("../img_error.png");
            }
            SDL_Texture *img_T = SDL_CreateTextureFromSurface(gameRenderer,img);

            SDL_DestroySurface(img);
            // remember to free the textrure memory as well;
            
            return img_T;
        }

        int winChecker(){
            // -1 for no one has won, 0 for O victory, 1 for X victory
            
            return -1;
        }

        void ninetothree(){
            
        }

        void renderData(){
            // check win status everytime
            // WIN CONDITION LOGIC:
            
            if (winChecker() == 0){
                gameState = GameState::OWinScreen;
                SDL_Log("O Victory! Congratulations");
            }else if (winChecker() == 1){
                gameState = GameState::XWinScreen;
                SDL_Log("X Victory! Congratulations");
            }

            // renderer:
            SDL_SetRenderDrawColor(gameRenderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
            SDL_RenderClear(gameRenderer);
            
            // Draw the grid
            
            // Base drawing
            // add SUPER TIC TAC TOE Rules later
            
            // load both textures
            string pathX = "../assets/x.png";
            SDL_Texture *obj_X = loadTexture(pathX);

            string pathO = "../assets/o.png";
            SDL_Texture *obj_O = loadTexture(pathO);

            // scale it down
            SDL_FRect oRect;
            oRect.h = (float) 100;
            oRect.w = (float) 100;


            for (int i=0; i<9; i++){
                for (int j=0; j<9; j++){

                    oRect.x = 100*j;
                    oRect.y = 100*i;

                    if (gameData[i][j] == 1){
                        SDL_RenderTexture(gameRenderer, obj_X, nullptr, &oRect);
                    }else if(gameData[i][j] == 0){
                        SDL_RenderTexture(gameRenderer, obj_O, nullptr, &oRect);
                    }else continue;
                }
            }

            // draw the matrix (debugging)
            for(int i=0; i<9; i++){
                for(int j=0; j<9; j++){
                    cout<<gameData[i][j]<<" ";
                    if (j == 8) cout<<"\n";
                }
            }
            SDL_RenderPresent(gameRenderer);
        }

        SDL_Event eventLogger(){
            SDL_Event event;
            
            while (SDL_PollEvent(&event)){
                switch(event.type){
                    case SDL_EVENT_QUIT:
                        SDL_Log("event: terminating game...");
                        running = false;
                        close();
                        break;

                    case SDL_EVENT_MOUSE_BUTTON_DOWN:
                        inputHandler(event);
                        // will only deal with left click
                        // might add keyboard input later for something like names
                        break;
                }
            }
            return event;
        }

        void inputHandler(SDL_Event event){
            if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN){
                SDL_Log("event: Mouse button down");
                if (event.button.windowID != gameWindowID) return;
                float posX = event.button.x;
                float posY = event.button.y;
                int indX = floor((posX*9)/900);
                int indY = floor((posY*9)/900);

                // add a checker to check if there's already a symbol there
                
                int turn = move%2 == 0 ? 1 : 0;
                if (gameData[indY][indX] == -1){
                    gameData[indY][indX] = turn;
                    move++;
                }
                renderData();
            }
        }
        
// implement error/exit codes


    public:
        bool init(){
            fill(&gameData[0][0], &gameData[0][0] + (9*9), -1);

            if(!SDL_CreateWindowAndRenderer("S3T", screenWidth, screenHeight, SDL_WINDOW_MOUSE_FOCUS, &gameWindow, &gameRenderer)){
                SDL_Log("couldn't initialize window and renderer! SDL_Error: %s\n", SDL_GetError());
                return false;
            }
            gameWindowID = SDL_GetWindowID(gameWindow);
            running = true;
            return true;
        }

        void run(){
            renderData(); // replace this with renderMainMenu
            while (running){
                if (gameState == GameState::playing){
                    SDL_Event event = eventLogger();
                }
            }
        }


        void close(){
            SDL_DestroyWindow(gameWindow);
            gameWindow = nullptr;
            SDL_DestroyRenderer(gameRenderer);
            gameRenderer = nullptr;
            // todo free memory an stuff
        }
};


int main(int argc, char* argv[]){
    game Game;
    Game.init();
    Game.run();
}

