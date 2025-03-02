#include "Game.h"
#include <iostream>
#include <SDL_ttf.h>
#include "TextureManager.h"

using namespace std;


SDL_Renderer* Game::renderer = nullptr;

Game::Game() {
    window = nullptr;
    isRunning = false;

    gameState = MENU;
    font = nullptr;
    textColor = { 0, 0, 0, 255 }; // Black
    buttonTextColor = { 255, 255, 255, 255 }; // white

    playButtonHovered = false;
    highScoreButtonHovered = false;
    creditsButtonHovered = false;
}

Game::~Game() {
    if (font) {
        TTF_CloseFont(font);
    }
    if (titlefont) {
        TTF_CloseFont(titlefont);
    }
}

bool Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen) {
    int flags = 0;
    if (fullscreen) {
        flags = SDL_WINDOW_FULLSCREEN;
    }

    if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
        cout << "Subsystems Initialized!..." << endl;

        window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
        if (window) {
            cout << "Window created!" << endl;
        }

        renderer = SDL_CreateRenderer(window, -1, 0);
        if (renderer) {
            SDL_SetRenderDrawColor(renderer, 179, 235, 242, 0); // xanh mau pastel (179,235,242)
            cout << "Renderer created!" << endl;
            isRunning = true;
        }
        else {
            cout << "Renderer creation failed!" << endl;
            return false;
        }
    }
    else {
        cout << "SDL Init failed!" << endl;
        return false;
    }

    /////////// initialize ttf
    if (TTF_Init() == -1) {
        std::cerr << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << std::endl;
        return false;
    }

    // Load a font (Make sure "assets/arial.ttf" exists in your build directory)
    font = TTF_OpenFont("assets/PressStart2P-Regular.ttf", 24);
    titlefont = TTF_OpenFont("assets/PressStart2P-Regular.ttf", 30);
    if (!font || !titlefont) {
        std::cerr << "Failed to load font! SDL_ttf Error: " << TTF_GetError() << std::endl;
        return false;
    }

    // Load menu background
    menuBackgroundTexture = TextureManager::LoadTexture("assets/menu_background.jpg");
    if (!menuBackgroundTexture) {
        std::cerr << "Failed to load menu background texture!" << std::endl;
        return false;
    }

    // Load button textures
    playButtonNormalTexture = TextureManager::LoadTexture("assets/button_texture.jpg");
    highScoreButtonNormalTexture = TextureManager::LoadTexture("assets/button_texture.jpg");
    creditsButtonNormalTexture = TextureManager::LoadTexture("assets/button_texture.jpg");

    //Load all of the button first, before check if it is null or not
    if (!playButtonNormalTexture ||
        !highScoreButtonNormalTexture ||
        !creditsButtonNormalTexture)
    {
        std::cerr << "Failed to load button textures!" << std::endl;
        return false;
    }

    return true;
}

void Game::handleEvents() {
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type) {
    case SDL_QUIT:
        isRunning = false;
        break;
    case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_q) { // Check if 'Q' key is pressed -> Close the window
            isRunning = false;
        }
        break;
    default:
        if (gameState == MENU) { // neu trang thai game o menu thi handle theo kieu menu
            handleMenuEvents();
        }

        break;
    }
}

void Game::update() {
    // Update game logic here (e.g., player movement, enemy AI).  For now, empty.

}

void Game::render() {
    SDL_RenderClear(renderer);

    // kiem tra trang thai game truoc khi render
    switch (gameState) {
    case MENU:
        renderMenu();
        break;
    case PLAYING:
        // (Game rendering will go here later)




        break;
    case HIGH_SCORE:
        //renderHighScore();
        break;
    case CREDITS:
        // renderCredits();
        break;
    }

    SDL_RenderPresent(renderer);
}

void Game::clean() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(menuBackgroundTexture);

    SDL_DestroyTexture(playButtonNormalTexture);
    SDL_DestroyTexture(highScoreButtonNormalTexture);
    SDL_DestroyTexture(creditsButtonNormalTexture);

    // huy ttf
    if (font) { // Check if font is loaded before closing and quitting
        TTF_CloseFont(font);
    }
    if (titlefont) { // Check if font is loaded before closing and quitting
        TTF_CloseFont(titlefont);
    }
    TTF_Quit();

    SDL_Quit();
    std::cout << "Game Cleaned" << std::endl;
}

void Game::renderMenu() {

    // back ground
    SDL_Rect backgroundRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
    SDL_RenderCopy(renderer, menuBackgroundTexture, NULL, &backgroundRect);

    // Title
    SDL_Surface* titleSurface = TTF_RenderText_Solid(titlefont, "Tank battle 2P", textColor);
    SDL_Texture* titleTexture = SDL_CreateTextureFromSurface(renderer, titleSurface);
    SDL_Rect titleRect;
    titleRect.w = titleSurface->w;
    titleRect.h = titleSurface->h;
    titleRect.x = SCREEN_WIDTH / 2 - titleSurface->w / 2; // center to horizontal
    titleRect.y = 150; // set to 100 from top
    SDL_RenderCopy(renderer, titleTexture, NULL, &titleRect);
    SDL_FreeSurface(titleSurface);
    SDL_DestroyTexture(titleTexture);

    // Button1: PLAY
    playButtonRect.x = SCREEN_WIDTH / 2 - BUTTON_WIDTH / 2; // horizontal center - half of button with
    playButtonRect.y = 300; // titleRect.y + BUTTON_SPACE + 50; // spawn cách title = 1 but_space + 50 (+ 50 vao cho dep)
    playButtonRect.w = BUTTON_WIDTH; // button width
    playButtonRect.h = BUTTON_HEIGHT; // button height

    SDL_RenderCopy(renderer, playButtonNormalTexture, NULL, &playButtonRect); // render texture

    //SDL_RenderFillRect(renderer, &playButtonRect);

    SDL_Surface* playSurface = TTF_RenderText_Solid(font, "PLAY", buttonTextColor);
    SDL_Texture* playTexture = SDL_CreateTextureFromSurface(renderer, playSurface);
    SDL_Rect playTextRect;
    playTextRect.w = playSurface->w;
    playTextRect.h = playSurface->h;
    playTextRect.x = playButtonRect.x + (playButtonRect.w - playSurface->w) / 2; // Center text horizontally
    playTextRect.y = playButtonRect.y + (playButtonRect.h - playSurface->h) / 2; // Center text vertically
    SDL_RenderCopy(renderer, playTexture, NULL, &playTextRect);
    SDL_FreeSurface(playSurface);
    SDL_DestroyTexture(playTexture);




    // Button2: HIGH SCORE
    highScoreButtonRect.x = SCREEN_WIDTH / 2 - BUTTON_WIDTH / 2;
    highScoreButtonRect.y = 450; //playButtonRect.y + BUTTON_SPACE; // tinh theo playbutton
    highScoreButtonRect.w = BUTTON_WIDTH;
    highScoreButtonRect.h = BUTTON_HEIGHT;
    SDL_RenderCopy(renderer, highScoreButtonNormalTexture, NULL, &highScoreButtonRect);
    //SDL_RenderFillRect(renderer, &highScoreButtonRect);

    SDL_Surface* highScoreSurface = TTF_RenderText_Solid(font, "HIGH SCORE", buttonTextColor);
    SDL_Texture* highScoreTexture = SDL_CreateTextureFromSurface(renderer, highScoreSurface);
    SDL_Rect highScoreTextRect;
    highScoreTextRect.w = highScoreSurface->w;
    highScoreTextRect.h = highScoreSurface->h;
    highScoreTextRect.x = highScoreButtonRect.x + (highScoreButtonRect.w - highScoreSurface->w) / 2; // Center text horizontally
    highScoreTextRect.y = highScoreButtonRect.y + (highScoreButtonRect.h - highScoreSurface->h) / 2; // Center text vertically
    SDL_RenderCopy(renderer, highScoreTexture, NULL, &highScoreTextRect);
    SDL_FreeSurface(highScoreSurface);
    SDL_DestroyTexture(highScoreTexture);

    // Button3: CREDITS
    creditsButtonRect.x = SCREEN_WIDTH / 2 - BUTTON_WIDTH / 2;
    creditsButtonRect.y = 600; //highScoreButtonRect.y + BUTTON_SPACE;
    creditsButtonRect.w = BUTTON_WIDTH;
    creditsButtonRect.h = BUTTON_HEIGHT;
    // SDL_SetRenderDrawColor(renderer, 120, 0, 255, 255); // Blue color for button

    SDL_RenderCopy(renderer, creditsButtonNormalTexture, NULL, &creditsButtonRect);
    //SDL_RenderFillRect(renderer, &creditsButtonRect);

    SDL_Surface* creditsSurface = TTF_RenderText_Solid(font, "CREDITS", buttonTextColor);
    SDL_Texture* creditsTexture = SDL_CreateTextureFromSurface(renderer, creditsSurface);
    SDL_Rect creditsTextRect;
    creditsTextRect.w = creditsSurface->w;
    creditsTextRect.h = creditsSurface->h;
    creditsTextRect.x = creditsButtonRect.x + (creditsButtonRect.w - creditsSurface->w) / 2; // Center text horizontally
    creditsTextRect.y = creditsButtonRect.y + (creditsButtonRect.h - creditsSurface->h) / 2; // Center text vertically
    SDL_RenderCopy(renderer, creditsTexture, NULL, &creditsTextRect);
    SDL_FreeSurface(creditsSurface);
    SDL_DestroyTexture(creditsTexture);

    // Initialize button rectangles *after* rendering them:
}


void Game::handleMenuEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            isRunning = false;
            break;
        case SDL_MOUSEBUTTONDOWN: {
            int mouseX = event.button.x;
            int mouseY = event.button.y;

            // Check if PLAY button was clicked
            if (mouseX >= playButtonRect.x && mouseX <= playButtonRect.x + playButtonRect.w &&
                mouseY >= playButtonRect.y && mouseY <= playButtonRect.y + playButtonRect.h) {
                gameState = PLAYING;
                std::cout << "PLAY button clicked!\n";
            }
            // Check if HIGH SCORE button was clicked
            else if (mouseX >= highScoreButtonRect.x && mouseX <= highScoreButtonRect.x + highScoreButtonRect.w &&
                mouseY >= highScoreButtonRect.y && mouseY <= highScoreButtonRect.y + highScoreButtonRect.h) {
                gameState = HIGH_SCORE;
                std::cout << "HIGH SCORE button clicked!\n";
            }
            // Check if CREDITS button was clicked
            else if (mouseX >= creditsButtonRect.x && mouseX <= creditsButtonRect.x + creditsButtonRect.w &&
                mouseY >= creditsButtonRect.y && mouseY <= creditsButtonRect.y + creditsButtonRect.h) {
                gameState = CREDITS;
                std::cout << "CREDITS button clicked!\n";
            }
            break;
        }
        default:
            break;
        }
    }
}