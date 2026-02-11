#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <cstdint>
#include "iostream"
#include "cmath"
#include "string"
#include "algorithm"
#include "utility"

using namespace std;

enum class GameState{
    mainMenu,
    playing,
    scoreScreen,
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
        
        // index of the current game
        pair<int, int>currGame = {-1, -1};

        uint64_t framerate = 60;
        
        int move = 0;
        int gameData9[9][9];
        int gameData3[3][3];

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

        // winChecker(int game[3][3])
        int winChecker(){
            // -1 for no one has won, 0 for O victory, 1 for X victory, 2 for draw

            return -1;
        }
            
        // no clue how to do this more effieciently
        void drawThickL(float x1, float y1, float x2, float y2){
            SDL_FRect tl;
            
        }

        void renderData(){
            // check win status everytime
            // WIN CONDITION LOGIC:
            
            if (winChecker() == 0){
                SDL_Log("O Victory! Congratulations");
            }else if (winChecker() == 1){
                SDL_Log("X Victory! Congratulations");
            }

            // renderer:
            SDL_SetRenderDrawColor(gameRenderer, 200, 200, 200, SDL_ALPHA_OPAQUE);
            SDL_RenderClear(gameRenderer);

            // highlighting currGame
            if (currGame.first != -1 && currGame.second != -1){
                SDL_SetRenderDrawColor(gameRenderer, 220, 220, 220, SDL_ALPHA_OPAQUE);
                SDL_FRect currGameShader;
                currGameShader.h = currGameShader.w = 300;
                currGameShader.x = (int) currGame.first * 300;
                currGameShader.y = (int) currGame.second * 300;
                
                SDL_RenderFillRect(gameRenderer, &currGameShader);
                SDL_RenderRect(gameRenderer, &currGameShader);
            }

            // load both x and o textures
            string pathX = "../assets/x.png";
            SDL_Texture *obj_X = loadTexture(pathX);

            string pathO = "../assets/o.png";
            SDL_Texture *obj_O = loadTexture(pathO);

            // scale it down
            SDL_FRect oRect;
            oRect.h = (float) 100;
            oRect.w = (float) 100;

            // thick lines
            drawThickL(300, 0, 300, 0);
            drawThickL(600, 0, 600, 0);
            drawThickL(0, 300, 0, 300);
            drawThickL(0, 600, 0, 600);

            for (int i=0; i<9; i++){
                for (int j=0; j<9; j++){

                    // drawing grid
                    SDL_SetRenderDrawColor(gameRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
                    //vertical
                    SDL_RenderLine(gameRenderer, j*100, 0, j*100, 900);
                    // horizontal
                    SDL_RenderLine(gameRenderer, 0, j*100, 900, j*100);

                    // rendering o's and x's
                    oRect.x = 100*j;
                    oRect.y = 100*i;

                    if (gameData9[i][j] == 1){
                        SDL_RenderTexture(gameRenderer, obj_X, nullptr, &oRect);
                        SDL_Log("rendering X \n");
                    }else if(gameData9[i][j] == 0){
                        SDL_RenderTexture(gameRenderer, obj_O, nullptr, &oRect);
                        SDL_Log("rendering O \n");
                    }else continue;
                }
            }

            SDL_RenderLine(gameRenderer, 300-1, 0, 300-1, 0);
            SDL_RenderLine(gameRenderer, 300+1, 0, 300+1, 0);
            SDL_RenderLine(gameRenderer, 600-1, 0, 600-1, 0);
            SDL_RenderLine(gameRenderer, 300+1, 0, 600+1, 0);

            SDL_RenderLine(gameRenderer, 0, 300-1, 0, 300-1);
            SDL_RenderLine(gameRenderer, 0, 300+1, 0, 300+1);

            // draw the matrix (debugging)
            //for(int i=0; i<9; i++){
                //for(int j=0; j<9; j++){
                    //cout<<gameData9[i][j]<<" ";
                    //if (j == 8) cout<<"\n";
                //}
            //}

            SDL_DestroyTexture(obj_O);
            SDL_DestroyTexture(obj_X);

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
            if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN && gameState == GameState::playing){
                SDL_Log("event: Mouse button down");
                if (event.button.windowID != gameWindowID) return;
                float posX = event.button.x;
                float posY = event.button.y;
                int indX = floor((posX*9)/900);
                int indY = floor((posY*9)/900);

                int turn = move%2 == 0 ? 1 : 0;

                // if (currGame.first == -1 && currGame.second == -1){
                //    currGame.first = indX%3;
                //    currGame.second = indY%3;
                //}
                
                
                // add conditionals to restrict move to currGame
                if (gameData9[indY][indX] == -1){
                    gameData9[indY][indX] = turn;
                    move++;
                    currGame.first = indX%3;
                    currGame.second = indY%3;
                }

                renderData();
            }
        }
        
// implement error/exit codes


    public:
        bool init(){
            fill(&gameData9[0][0], &gameData9[0][0] + (9*9), -1);
            fill(&gameData3[0][0], &gameData3[0][0] + (3*3), -1);

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
                SDL_Event event = eventLogger();
                switch(gameState){
                    case GameState::mainMenu:
                        break;
                    case GameState::playing:
                        break;
                    case GameState::scoreScreen:
                        break;
                }
            }
        }

        void close(){
            SDL_DestroyRenderer(gameRenderer);
            gameRenderer = nullptr;
            SDL_DestroyWindow(gameWindow);
            gameWindow = nullptr;
        }
};


int main(int argc, char* argv[]){
    game Game;
    Game.init();
    Game.run();
}

