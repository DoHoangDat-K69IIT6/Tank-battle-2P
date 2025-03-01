#pragma once
#include <SDL.h>
#include <string>
#include <SDL_ttf.h>

const int SCREEN_WIDTH = 1366;
const int SCREEN_HEIGHT = 768;

const int BUTTON_WIDTH = 350;
const int BUTTON_HEIGHT = 90;
const int BUTTON_SPACE = 100;

enum GameState { //
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



private:
    SDL_Window* window;
    bool isRunning;

    GameState gameState;

    // Menu-related
    TTF_Font* font;
    TTF_Font* titlefont;
    SDL_Color textColor;
    SDL_Color buttonTextColor;
    SDL_Texture* menuBackgroundTexture; // background image

    SDL_Texture* playButtonNormalTexture; // texture cua button
    SDL_Texture* highScoreButtonNormalTexture;
    SDL_Texture* creditsButtonNormalTexture;

    SDL_Rect playButtonRect;
    SDL_Rect highScoreButtonRect;
    SDL_Rect creditsButtonRect;

    void renderMenu();
    void handleMenuEvents();

    void renderHighScore(); // Placeholder
    void renderCredits();   // Placeholder

    bool playButtonHovered;
    bool highScoreButtonHovered;
    bool creditsButtonHovered;


};

