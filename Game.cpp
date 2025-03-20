#include "Game.h"
#include <iostream>
#include <SDL_ttf.h>
#include "Player.h"
#include "TextureManager.h"
#include <fstream>
#include "Bullet.h"

using namespace std;


SDL_Renderer* Game::renderer = nullptr;

Game::Game() {
    window = nullptr;
    font = nullptr; 
    titlefont = nullptr; 
    isRunning = false;

    gameState = MENU;
    font = nullptr;
    textColor = { 0, 0, 0, 255 }; // Black
    buttonTextColor = { 255, 255, 255, 255 }; // white
    
    // khoi tao cac bien
    playButtonRect.x = 0;
    playButtonRect.y = 0;
    playButtonRect.w = 0;
    playButtonRect.h = 0;
    highScoreButtonRect.x = 0;
    highScoreButtonRect.y = 0;
    highScoreButtonRect.w = 0;
    highScoreButtonRect.h = 0;
    creditsButtonRect.x = 0;
    creditsButtonRect.y = 0;
    creditsButtonRect.w = 0;
    creditsButtonRect.h = 0;
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

    //initialize ttf
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

    if (!playButtonNormalTexture ||
        !highScoreButtonNormalTexture ||
        !creditsButtonNormalTexture)
    {
        std::cerr << "Failed to load button textures!" << std::endl;
        return false;
    }

    // Load wall textures
    wallTexture = TextureManager::LoadTexture("assets/wall.png"); // Replace with your actual filename
    if (!wallTexture) {
        std::cerr << "Failed to load wall texture!" << std::endl;
        return false; // Or handle the error appropriately
    }

    wall2Texture = TextureManager::LoadTexture("assets/wall2.png"); // Replace with your actual filename
    if (!wall2Texture) {
        std::cerr << "Failed to load wall2 texture!" << std::endl;
        return false; // Or handle the error appropriately
    }

    //star3Texture = TextureManager::LoadTexture("assets/star3.png"); // Replace with your actual filename
    //if (!star3Texture) {
    //    std::cerr << "Failed to load star3 texture!" << std::endl;
    //    return false; // Or handle the error appropriately
    //}

    //flagTexture = TextureManager::LoadTexture("assets/flag.png"); // Replace with your actual filename
    //if (!flagTexture) {
    //    std::cerr << "Failed to load flag texture!" << std::endl;
    //    return false; // Or handle the error appropriately
    //}

    //star5Texture = TextureManager::LoadTexture("assets/star5.png"); // Replace with your actual filename
    //if (!star5Texture) {
    //    std::cerr << "Failed to load star5 texture!" << std::endl;
    //    return false; // Or handle the error appropriately
    //}

    // khoi tao map
    //loadMap("assets/map.txt");

    // khoi tao nguoi choi

    player1 = new Player(175, 350, "assets/green_tank_test.png");
    player2 = new Player(1155, 350, "assets/green_tank_test.png");

    // khoi tao map
    loadMap("assets/map.txt");

    return true;
}

void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            isRunning = false;
            break;
        case SDL_KEYDOWN: // Just call handlePlayingEvents for KEYDOWN in PLAYING state
            if (gameState == PLAYING) {
                handlePlayingEvents(event); // Pass event to playing event handler
            }
            else if (gameState == MENU) {
                handleMenuEvents(event);   // Pass event to menu event handler
            }
            else if (event.key.keysym.sym == SDLK_q) {
                isRunning = false;
            }
            break;
        case SDL_MOUSEBUTTONDOWN: // For menu clicks (only handled in MENU state)
            if (gameState == MENU) {
                handleMenuEvents(event);
            }
            break;
        default:
            break;
        }
    }
}

void Game::update() {
    // Update game logic here (e.g., player movement, enemy AI).  For now, empty.

    for (int i = 0; i < bullets.size(); ++i) {
        if (bullets[i]->isActive()) {
            bullets[i]->update(); // Call update on active bullets
        }
        else {
            // If bullet is not active, delete it and remove from vector
            delete bullets[i];
            bullets.erase(bullets.begin() + i);
            --i; // Adjust index after erasing
        }
    }
    
}

void Game::render() {
    SDL_RenderClear(renderer);

    // kiem tra trang thai game truoc khi render
    switch (gameState) {
    case MENU:
        renderMenu();
        break;
    case PLAYING:
        renderMap();
        player1->render();
        player2->render();
        for (Bullet* bullet : bullets) { // Loop through ALL bullets, update and render
            bullet->render(); //The bullet check if it is active.
        }
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

    //SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Green color for button
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

void Game::handleMenuEvents(const SDL_Event& event) {
    switch (event.type) {
    case SDL_QUIT: 
        isRunning = false;
        break;
    case SDL_MOUSEBUTTONDOWN: {
        int mouseX = event.button.x;
        int mouseY = event.button.y;

        std::cout << "Mouse click at: (" << mouseX << ", " << mouseY << ")" << std::endl;

        // ... (rest of your mouse button down logic) ...
        //Check if PLAY button was clicked
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

void Game::clean() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);

    if (menuBackgroundTexture) { 
        SDL_DestroyTexture(menuBackgroundTexture);
        menuBackgroundTexture = nullptr; 
    }
    if (playButtonNormalTexture) { 
        SDL_DestroyTexture(playButtonNormalTexture);
        playButtonNormalTexture = nullptr; 
    }
    if (highScoreButtonNormalTexture) { 
        SDL_DestroyTexture(highScoreButtonNormalTexture);
        highScoreButtonNormalTexture = nullptr; 
    }
    if (creditsButtonNormalTexture) { 
        SDL_DestroyTexture(creditsButtonNormalTexture);
        creditsButtonNormalTexture = nullptr; 
    }

    // huy ttf
    if (font) { // Check if font is loaded before closing and quitting
        TTF_CloseFont(font);
        font = nullptr;
    }
    if (titlefont) { // Check if font is loaded before closing and quitting
        TTF_CloseFont(titlefont);
        titlefont = nullptr;
    }
    TTF_Quit();

    for (Bullet* bullet : bullets) { // Very, very important to delete allocated bullets!
        delete bullet;
    }
    bullets.clear(); // Clear the bullets vector

    SDL_Quit();
    std::cout << "Game Cleaned" << std::endl;
}


void Game::handlePlayingEvents(const SDL_Event& event) {
    const Uint8* keyboardState = SDL_GetKeyboardState(NULL);

    // Player 1 Movement (WASD) - Pass player2 as the otherPlayer
    if (keyboardState[SDL_SCANCODE_W]) {
        player1->move(0, -1, mapWidth, mapHeight, map, player2); // Pass player2
    }
    if (keyboardState[SDL_SCANCODE_S]) {
        player1->move(0, 1, mapWidth, mapHeight, map, player2);  // Pass player2
    }
    if (keyboardState[SDL_SCANCODE_A]) {
        player1->move(-1, 0, mapWidth, mapHeight, map, player2); // Pass player2
    }
    if (keyboardState[SDL_SCANCODE_D]) {
        player1->move(1, 0, mapWidth, mapHeight, map, player2);  // Pass player2
    }
    if (keyboardState[SDL_SCANCODE_SPACE]) {
        // Player 1 shoot
    }

    // Player 2 Movement (Arrow Keys) - Pass player1 as the otherPlayer
    if (keyboardState[SDL_SCANCODE_UP]) {
        player2->move(0, -1, mapWidth, mapHeight, map, player1); // Pass player1
    }
    if (keyboardState[SDL_SCANCODE_DOWN]) {
        player2->move(0, 1, mapWidth, mapHeight, map, player1);  // Pass player1
    }
    if (keyboardState[SDL_SCANCODE_LEFT]) {
        player2->move(-1, 0, mapWidth, mapHeight, map, player1); // Pass player1
    }
    if (keyboardState[SDL_SCANCODE_RIGHT]) {
        player2->move(1, 0, mapWidth, mapHeight, map, player1); // Pass player1
    }

    if (keyboardState[SDL_SCANCODE_SPACE]) { // Player 1 shoots with SPACE
        // Create a bullet for Player 1, moving upwards (you can adjust direction)
        Bullet* bullet = new Bullet(player1->getRect().x + player1->getRect().w / 2 - 8, player1->getRect().y, 0, -1, "assets/bullet.png"); // Adjust starting position and direction
        bullets.push_back(bullet);
    }
    if (keyboardState[SDL_SCANCODE_SLASH]) { // Player 2 shoots with '/' (slash key)
        // Create a bullet for Player 2, moving upwards (you can adjust direction)
        Bullet* bullet = new Bullet(player2->getRect().x + player2->getRect().w / 2 - 8, player2->getRect().y, 0, -1, "assets/bullet.png"); // Adjust starting position and direction
        bullets.push_back(bullet);
    }
}

bool Game::loadMap(const char* filePath) {
    std::ifstream mapFile(filePath);
    if (!mapFile.is_open()) {
        std::cerr << "Failed to open map file: " << filePath << std::endl;
        return false;
    }

    mapFile >> mapWidth >> mapHeight;  // Read width and height

    map.resize(mapHeight, std::vector<int>(mapWidth)); // Resize the map vector

    for (int row = 0; row < mapHeight; ++row) {
        for (int col = 0; col < mapWidth; ++col) {
            char tileChar;
            mapFile >> tileChar; // Read the tile as a character
            map[row][col] = tileChar - '0'; // Convert character '0', '1', etc., to integer 0, 1, etc.
        }
    }

    mapFile.close();
    return true;
}

void Game::renderMap() {
    for (int row = 0; row < mapHeight; ++row) {
        for (int col = 0; col < mapWidth; ++col) {
            int tileType = map[row][col];

            SDL_Rect srcRect; // Source rectangle (within the texture)
            srcRect.x = 0;    // For now, assume we use the whole texture
            srcRect.y = 0;
            srcRect.w = TILE_SIZE;
            srcRect.h = TILE_SIZE;

            SDL_Rect destRect; // Destination rectangle (on the screen)
            destRect.x = col * TILE_SIZE;  // Calculate x position based on column and tile width
            destRect.y = row * TILE_SIZE; // Calculate y position based on row and tile height
            destRect.w = TILE_SIZE;
            destRect.h = TILE_SIZE;

            switch (tileType) {
            case 0: // Empty space - do nothing (just background will show)
                break;
            case 1: // Indestructible wall
                SDL_RenderCopy(renderer, wallTexture, NULL, &destRect); // Draw wall texture
                break;
            case 2:
                SDL_RenderCopy(renderer, wall2Texture, NULL, &destRect);  // Draw wall texture
                break;
            //case 7:
            //    TextureManager::Draw(star3Texture, srcRect, destRect); // Draw wall texture
            //    break;
            //case 8:
            //    TextureManager::Draw(flagTexture, srcRect, destRect); // Draw wall texture
            //    break;
            //case 9:
            //    TextureManager::Draw(star5Texture, srcRect, destRect); // Draw wall texture
            //    break;
            }
        }
    }
}