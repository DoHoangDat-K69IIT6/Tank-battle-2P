#pragma once
#include <SDL.h>
#include <string>
#include "Player.h"
#include <SDL_ttf.h>
#include <vector>
#include <SDL_mixer.h>

const int TILE_SIZE = 35;

const int SCREEN_WIDTH = 1366;
const int SCREEN_HEIGHT = 768;
const int BUTTON_WIDTH = 350;
const int BUTTON_HEIGHT = 90;
const int BUTTON_SPACE = 100;


const int PLAYER1_START_POS_X = 0;
const int PLAYER1_START_POS_Y = 0;
const int PLAYER1_W = 35; 
const int PLAYER1_H = 35; 

const int PLAYER2_START_POS_X = 500;
const int PLAYER2_START_POS_Y = 0;
const int PLAYER2_W = 35;  
const int PLAYER2_H = 35;  


enum GameState {
    MENU,
    PLAYING,
    GAME_OVER,
    HIGH_SCORE,
    CREDITS
};

class Game {
public:
    Game();
    ~Game();

    bool init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
    void handleEvents();
    void update();
    void render();
    bool running() { return isRunning; }
    void clean();

    static SDL_Renderer* renderer;

    static int getMapWidth() { return mapWidth; }  // Static getter for mapWidth
    static int getMapHeight() { return mapHeight; } // Static getter for mapHeight
    static vector<vector<int>>& getMap() { return map; } // Static getter for map

    static GameState getGameState() { return gameState; } // Static getter for gameState
    static void setGameState(GameState state) { gameState = state; } // Static setter for gameState
    static int getWinner() { return winner; } // Static getter for winner
    static void setWinner(int playerNum) { winner = playerNum; } // Static setter for winner

    TTF_Font* titlefont;

private:
    SDL_Window* window;
    bool isRunning;
    int countthu = 0;
    //GameState gameState;

    // Menu-related
    TTF_Font* font;
    
    SDL_Color textColor;
    SDL_Color buttonTextColor;
    SDL_Texture* menuBackgroundTexture; // background image

    SDL_Texture* playButtonNormalTexture; // texture cua button
    SDL_Texture* highScoreButtonNormalTexture;
    SDL_Texture* creditsButtonNormalTexture;
    SDL_Rect playButtonRect;
    SDL_Rect highScoreButtonRect;
    SDL_Rect creditsButtonRect;

    // khoi tao player pointer
    Player* player1;
    Player* player2;

    // texture cua map
    SDL_Texture* wallTexture;   // Texture for indestructible walls
    SDL_Texture* wall2Texture; // Texture for walls that can be destroyed (2 hits)
    SDL_Texture* wall2Texture_damaged1;
    SDL_Texture* wall2Texture_damaged2;

    SDL_Texture* targetP1Texture; // Texture for target 1
    SDL_Texture* targetP2Texture; // Texture for target 2
    SDL_Texture* buff5x5Texture; // Texture for 5x5 stars
    SDL_Texture* buff3x3Texture; // Texture for 3x3 stars

    //bullet
    vector<Bullet*> bullets;

    static GameState gameState; // <--- Make gameState static
    static int winner;          // <--- Add static winner variable

    static vector<vector<int>> map; // <--- Add static here
    static int mapWidth;                     // <--- Add static here
    static int mapHeight;                    // <--- Add static here

    int targetP1TileCol, targetP1TileRow; // Tile coordinates for Target P1
    int targetP2TileCol, targetP2TileRow; // Tile coordinates for Target P2

    bool loadMap(const char* filePath);
    void renderMap();

    void renderMenu();
    void handleMenuEvents(const SDL_Event& event);

    void handlePlayingEvents(const SDL_Event& event);

    void copyFileContent(const std::string& sourceFilePath, const std::string& destinationFilePath);

	void resetGame(); // Reset game state and player positions

	void renderGameOver();
    void renderHighScore(); // Placeholder
    void renderCredits();   // Placeholder

    Mix_Music* music;       // For background music (if you add music later)
    Mix_Chunk* fireSound;   // For player shooting sound
    Mix_Chunk* hitSound;    // For player hit sound
    Mix_Chunk* winSound;    // For win sound effect
};

