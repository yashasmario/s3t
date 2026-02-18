#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <cstdint>
#include <iostream>
#include "cmath"
#include "string"
#include "utility"

enum class GameState{
    mainMenu,
    playing,
    scoreScreen,
};

struct Board{
    int xMask = 0;
    int oMask = 0;
    int winner = -1;

    int getSquare(int idx) const{
        if (xMask & (1 << idx)) return 1;
        if (oMask & (1 << idx)) return 0;
        return -1;
    }
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
        std::pair<int, int>currBoard = {-1, -1};

        uint64_t framerate = 60;
            
        // store data in BITWISE
        int move = 0;
        Board subBoards[3][3];
        Board globalBoard;

        // load both x and o textures
        std::string pathX = "../assets/x.png";
        SDL_Texture *obj_X = loadTexture(pathX);
        std::string pathO = "../assets/o.png";
        SDL_Texture *obj_O = loadTexture(pathO);

        bool running = false;

        // priv functions
        SDL_Texture* loadTexture(std::string path){
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
        
        bool checkWin(int mask){
            // possible won states
            static const int winStates[] = {7, 56, 448, 73, 146, 292, 273, 84};

            for (int winPattern : winStates){
                if ((mask & winPattern) == winPattern) return true;
            }
            return false;
        }

        void renderBoard(Board board, float size){
            // scale it down
            SDL_FRect rect;
            rect.h = (float) size;
            rect.w = (float) size;
            rect.x = 0;
            rect.y = 0;
            
            for (int i=0; i<9; i++){
                if (board.getSquare(i) == 1){
                    SDL_RenderTexture(gameRenderer, obj_X, nullptr, &rect);
                }else if(board.getSquare(i) == 0){
                    SDL_RenderTexture(gameRenderer, obj_O, nullptr, &rect);
                }
            }

        }

        void renderData(){
            // check win status everytime
            // WIN CONDITION LOGIC:
            
            if (checkWin(globalBoard.xMask)){
                SDL_Log("O Victory! Congratulations");
            }else if (checkWin(globalBoard.oMask)){
                SDL_Log("X Victory! Congratulations");
            }

            // renderer:
            SDL_SetRenderDrawColor(gameRenderer, 200, 200, 200, SDL_ALPHA_OPAQUE);
            SDL_RenderClear(gameRenderer);

            // highlighting currBoard
            if (currBoard.first != -1 && currBoard.second != -1){
                SDL_SetRenderDrawColor(gameRenderer, 220, 220, 220, SDL_ALPHA_OPAQUE);
                SDL_FRect currGameShader;
                currGameShader.h = currGameShader.w = 300;
                currGameShader.x = (int) currBoard.first * 300;
                currGameShader.y = (int) currBoard.second * 300;
                
                SDL_RenderFillRect(gameRenderer, &currGameShader);
                SDL_RenderRect(gameRenderer, &currGameShader);
            }

            for (int i=0; i<9; i++){
                for (int j=0; j<9; j++){
                    // drawing grid
                    SDL_SetRenderDrawColor(gameRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
                    SDL_RenderLine(gameRenderer, j*100, 0, j*100, 900); // vertical
                    SDL_RenderLine(gameRenderer, 0, j*100, 900, j*100); // horizontal
                }
            }

            for (int i=0; i<3; i++){
                for (int j=0; j<3; j++){
                    renderBoard(subBoards[i][j], 100);

                    // for debugging
                    //for (int k=0; k<9; k++){
                        //std::cout<<subBoards[i][j].getSquare(k);
                    //}
                    //std::cout<<"\n";
                }
            }
            renderBoard(globalBoard, 300);

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
                int indY = floor((posX*9)/900);
                //int indY = floor(posY/100);
                int sub_indX = indX/3;
                int sub_indY = indY/3;
                int bitPosition = ((indY%3)*3 + indX%3);

                int turn = move%2 == 0 ? 1 : 0;

                // subBoards[indX][indY] gives subBoard
                Board subB = subBoards[sub_indX][sub_indY];
                if (turn){
                    subB.xMask |= (1 << bitPosition);
                    SDL_Log("masking x");
                    std::cout<<subB.xMask<<"\n";
                }else{
                    subB.oMask |= (1 << bitPosition);
                    SDL_Log("masking o");
                    std::cout<<subB.oMask<<"\n";
                }
                move++;
                renderData();
            }
        }
        
    public:
        bool init(){
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
            SDL_DestroyTexture(obj_O);
            SDL_DestroyTexture(obj_X);
            obj_X = nullptr;
            obj_O = nullptr;
        }
};


int main(int argc, char* argv[]){
    game Game;
    Game.init();
    Game.run();
}

