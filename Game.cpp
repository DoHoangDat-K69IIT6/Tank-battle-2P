#include "Game.h"
#include <iostream>
#include <SDL_ttf.h>
#include "Player.h"
#include "TextureManager.h"
#include <fstream>
#include "Bullet.h"


using namespace std;


SDL_Renderer* Game::renderer = nullptr;

std::vector<std::vector<int>> Game::map;      // Initialize static map
int Game::mapWidth = 0;                       // Initialize static mapWidth
int Game::mapHeight = 0;

GameState Game::gameState = MENU;
int Game::winner = 0; // Initialize static winner

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
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); // xanh mau pastel (179,235,242)
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

    wall2Texture_damaged1 = TextureManager::LoadTexture("assets/wall2_damaged1.png"); // Replace with your actual filename
    if (!wall2Texture_damaged1) {
        std::cerr << "Failed to load wall2_damaged1 texture!" << std::endl;
        return false; // Or handle the error appropriately
    }

    wall2Texture_damaged2 = TextureManager::LoadTexture("assets/wall2_damaged2.png"); // Replace with your actual filename
    if (!wall2Texture_damaged2) {
        std::cerr << "Failed to load wall2_damaged2 texture!" << std::endl;
        return false; // Or handle the error appropriately
    }

    targetP1Texture = TextureManager::LoadTexture("assets/targetP1.jpeg"); // Replace with your actual filename
    if (!targetP1Texture) {
        std::cerr << "Failed to load target 1 texture!" << std::endl;
        return false; // Or handle the error appropriately
    }

    targetP2Texture = TextureManager::LoadTexture("assets/targetP2.jpeg"); // Replace with your actual filename
    if (!targetP2Texture) {
        std::cerr << "Failed to load target 2 texture!" << std::endl;
        return false; // Or handle the error appropriately
    }

    buff3x3Texture = TextureManager::LoadTexture("assets/buff3x3.png"); // Replace with your actual filename
    if (!buff3x3Texture) {
        std::cerr << "Failed to load star3 texture!" << std::endl;
        return false; // Or handle the error appropriately
    }

    buff5x5Texture = TextureManager::LoadTexture("assets/buff5x5.png"); // Replace with your actual filename
    if (!buff5x5Texture) {
        std::cerr << "Failed to load star5 texture!" << std::endl;
        return false; // Or handle the error appropriately
    }

    // khoi tao map
    //loadMap("assets/map.txt");

    // khoi tao nguoi choi

    player1 = new Player(175, 350, "assets/green_tank_spritesheet.png");
    player2 = new Player(1155, 350, "assets/red_tank_spritesheet.png");

    // khoi tao map
    Game::copyFileContent("assets/scr_map.txt", "assets/map.txt");
    loadMap("assets/map.txt");

    // --- NEW: Hardcode Target TILE Coordinates ---
    targetP1TileCol = 37; // Replace XX with the actual TILE COLUMN of Target P1 (tile type 8)
    targetP1TileRow = 10; // Replace YY with the actual TILE ROW of Target P1 (tile type 8)
    std::cout << "Target P1 Tile Coordinates (Hardcoded): (" << targetP1TileCol << ", " << targetP1TileRow << ")" << std::endl;

    targetP2TileCol = 2; // Replace AA with the actual TILE COLUMN of Target P2 (tile type 5)
    targetP2TileRow = 10; // Replace BB with the actual TILE ROW of Target P2 (tile type 5)
    std::cout << "Target P2 Tile Coordinates (Hardcoded): (" << targetP2TileCol << ", " << targetP2TileRow << ")" << std::endl;
    // --- End Hardcoded Target Tile Coordinates ---

    // --- Initialize SDL_mixer ---
    if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) == 0) { // Initialize support for MP3 and OGG (common formats)
        std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
        return false; // Return false to indicate initialization failure
    }

    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048) < 0) { // Open audio device
        std::cerr << "SDL_mixer could not open audio! SDL_mixer Error: " << Mix_GetError() << std::endl;
        return false; // Return false if audio opening fails
    }
    cout << "SDL_mixer Initialized!..." << endl;
    // --- End SDL_mixer Initialization ---

    // --- Load Sound Effects ---
    fireSound = Mix_LoadWAV("assets/sound/fire_sound.wav"); // Replace "assets/fire_sound.wav" with your actual file path
    if (!fireSound) {
        std::cerr << "Failed to load fire sound effect! SDL_mixer Error: " << Mix_GetError() << std::endl;
        return false;
    }

    hitSound = Mix_LoadWAV("assets/sound/hit_sound.wav");   // Replace "assets/hit_sound.wav" with your actual file path
    if (!hitSound) {
        std::cerr << "Failed to load hit sound effect! SDL_mixer Error: " << Mix_GetError() << std::endl;
        return false;
    }

    winSound = Mix_LoadWAV("assets/sound/win_sound.wav");   // Replace "assets/win_sound.wav" with your actual file path
    if (!winSound) {
        std::cerr << "Failed to load win sound effect! SDL_mixer Error: " << Mix_GetError() << std::endl;
        return false;
    }

    // ... load other sound effects if needed ...
    cout << "Sound Effects Loaded!..." << std::endl;
    // --- End Load Sound Effects ---

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
        case SDL_KEYUP:
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
            } else if (gameState == GAME_OVER) { // <--- NEW: Handle mouse click in GAME_OVER
                resetGame(); // Reset game and return to menu
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

            // --- Bullet-Player Collision Detection ---
            SDL_Rect bulletRect = bullets[i]->getRect();
            SDL_Rect player1Rect = player1->getRect();
            SDL_Rect player2Rect = player2->getRect();

            // Bullet vs Player 1
            if (bullets[i]->isActive() && // Check if bullet is still active after update()
                bulletRect.x < player1Rect.x + player1Rect.w &&
                bulletRect.x + bulletRect.w > player1Rect.x &&
                bulletRect.y < player1Rect.y + player1Rect.h &&
                bulletRect.y + bulletRect.h > player1Rect.y)
            {
                bullets[i]->deactivate(); // Deactivate bullet
                Mix_PlayChannel(-1, hitSound, 0); // Play hit sound
                player1->respawn();      // Player 1 respawns (you can change this to health decrease etc.)
                cout << "Player 1 hit!" << endl;
            }
            // Bullet vs Player 2
            if (bullets[i]->isActive() && // Check if bullet is still active after update()
                bulletRect.x < player2Rect.x + player2Rect.w &&
                bulletRect.x + bulletRect.w > player2Rect.x &&
                bulletRect.y < player2Rect.y + player2Rect.h &&
                bulletRect.y + bulletRect.h > player2Rect.y)
            {
                bullets[i]->deactivate(); // Deactivate bullet
                Mix_PlayChannel(-1, hitSound, 0); // Play hit sound
                player2->respawn();      // Player 2 respawns
                cout << "Player 2 hit!" << endl;
            }
            // --- End Bullet-Player Collision Detection ---

        }
        else {
            // If bullet is not active, delete it and remove from vector
            delete bullets[i];
            bullets.erase(bullets.begin() + i);
            --i; // Adjust index after erasing
        }
    }

    // --- NEW: Win condition check based on TILE coordinates ---
    if (Game::gameState == PLAYING) { // Only check win condition if game is PLAYING
        int p1TileCol = player1->getRect().x / TILE_SIZE; // Player 1 tile column 
        int p1TileRow = player1->getRect().y / TILE_SIZE; // Player 1 tile row
		/*cout << "Player 1 Tile Coordinates: (" << p1TileCol << ", " << p1TileRow << ")" << endl;
		cout << "Player 1 Tile destination (" << targetP1TileCol << ", " << targetP1TileRow << ")" << endl;*/

        int p2TileCol = player2->getRect().x / TILE_SIZE; // Player 2 tile column
        int p2TileRow = player2->getRect().y / TILE_SIZE; // Player 2 tile row
		/*cout << "Player 2 Tile Coordinates: (" << p2TileCol << ", " << p2TileRow << ")" << endl;
		cout << "Player 2 Tile destination (" << targetP2TileCol << ", " << targetP2TileRow << ")" << endl;*/

        if (p1TileCol == targetP1TileCol && p1TileRow == targetP1TileRow) {
            std::cout << "Player 1 Wins! (Tile Coordinates Match)" << std::endl;
            Mix_PlayChannel(-1, winSound, 0); // Play win sound
            Game::setGameState(GAME_OVER);
            Game::setWinner(1);
        }
        else if (p2TileCol == targetP2TileCol && p2TileRow == targetP2TileRow) {
            std::cout << "Player 2 Wins! (Tile Coordinates Match)" << std::endl;
			Mix_PlayChannel(-1, winSound, 0); // Play win sound
            Game::setGameState(GAME_OVER);
            Game::setWinner(2);
        }
    }
    // --- End Win Condition Check ---
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
    case GAME_OVER: // <--- NEW: Render Game Over screen
        renderGameOver();
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

    // --- SDL_mixer Cleanup ---
    if (fireSound) {
        Mix_FreeChunk(fireSound); // Free the fire sound chunk
        fireSound = nullptr;
    }
    if (hitSound) {
        Mix_FreeChunk(hitSound);  // Free the hit sound chunk
        hitSound = nullptr;
    }
    if (winSound) {
        Mix_FreeChunk(winSound);  // Free the win sound chunk
        winSound = nullptr;
    }

    Mix_CloseAudio(); // Close the audio device
    Mix_Quit();       // Quit SDL_mixer
    cout << "SDL_mixer Cleaned!..." << std::endl;
    // --- End SDL_mixer Cleanup ---

    TTF_Quit();
    SDL_Quit();
    std::cout << "Game Cleaned" << std::endl;

    SDL_Quit();
    std::cout << "Game Cleaned" << std::endl;
}


void Game::handlePlayingEvents(const SDL_Event& event) {
    const Uint8* keyboardState = SDL_GetKeyboardState(NULL);

    // Player 1 Movement (WASD) - Pass player2 as the otherPlayer
    if (keyboardState[SDL_SCANCODE_W]) {
        player1->move(0, -1, mapWidth * TILE_SIZE, mapHeight * TILE_SIZE, map, player2); // Pass player2
    }
    if (keyboardState[SDL_SCANCODE_S]) {
        player1->move(0, 1, mapWidth * TILE_SIZE, mapHeight * TILE_SIZE, map, player2);  // Pass player2
    }
    if (keyboardState[SDL_SCANCODE_A]) {
        player1->move(-1, 0, mapWidth * TILE_SIZE, mapHeight * TILE_SIZE, map, player2); // Pass player2
    }
    if (keyboardState[SDL_SCANCODE_D]) {
        player1->move(1, 0, mapWidth * TILE_SIZE, mapHeight * TILE_SIZE, map, player2);  // Pass player2
    }


    // Player 2 Movement (Arrow Keys) - Pass player1 as the otherPlayer
    if (keyboardState[SDL_SCANCODE_UP]) {
        player2->move(0, -1, mapWidth * TILE_SIZE, mapHeight * TILE_SIZE, map, player1); // Pass player1
    }
    if (keyboardState[SDL_SCANCODE_DOWN]) {
        player2->move(0, 1, mapWidth * TILE_SIZE, mapHeight * TILE_SIZE, map, player1);  // Pass player1
    }
    if (keyboardState[SDL_SCANCODE_LEFT]) {
        player2->move(-1, 0, mapWidth * TILE_SIZE, mapHeight * TILE_SIZE, map, player1); // Pass player1
    }
    if (keyboardState[SDL_SCANCODE_RIGHT]) {
        player2->move(1, 0, mapWidth * TILE_SIZE, mapHeight * TILE_SIZE, map, player1); // Pass player1
    }

    if (keyboardState[SDL_SCANCODE_SPACE]) { // Player 1 shoots with SPACE
        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - player1->getLastShotTime() >= player1->getFireRate()) { // Fire rate check

            player1->setLastShotTime(currentTime); // Update last shot time

            FacingDirection p1Facing = player1->getFacingDirection();
            int bulletDirX = 0;
            int bulletDirY = 0;
            int spawnOffsetX = 0;
            int spawnOffsetY = 0;

            switch (p1Facing) {
                case UP:    bulletDirY = -1; spawnOffsetY = -20; break;
                case DOWN:  bulletDirY = 1;  spawnOffsetY = 20;  break;
                case LEFT:  bulletDirX = -1; spawnOffsetX = -20; break;
                case RIGHT: bulletDirX = 1;  spawnOffsetX = 20;  break;
            }

            Bullet* bullet = new Bullet(
                player1->getRect().x + player1->getRect().w / 2 - 8 + spawnOffsetX,
                player1->getRect().y + player1->getRect().h / 2 - 8 + spawnOffsetY,
                bulletDirX, bulletDirY,
                "assets/bullet_spritesheet.png"
            );
            bullets.push_back(bullet);

			// --- Play Fire Sound Effect ---
			Mix_PlayChannel(-1, fireSound, 0); // Play fire sound effect
        }
    }
    if (keyboardState[SDL_SCANCODE_SLASH]) { // Player 2 shoots with '/' (slash key)
        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - player2->getLastShotTime() >= player2->getFireRate()) { // Fire rate check

            player2->setLastShotTime(currentTime); // Update last shot time

            FacingDirection p2Facing = player2->getFacingDirection();
            int bulletDirX = 0;
            int bulletDirY = 0;
            int spawnOffsetX = 0;
            int spawnOffsetY = 0;

            switch (p2Facing) {
            case UP:    bulletDirY = -1; spawnOffsetY = -20; break;
            case DOWN:  bulletDirY = 1;  spawnOffsetY = 20;  break;
            case LEFT:  bulletDirX = -1; spawnOffsetX = -20; break;
            case RIGHT: bulletDirX = 1;  spawnOffsetX = 20;  break;
            }

            Bullet* bullet = new Bullet(
                player2->getRect().x + player2->getRect().w / 2 - 8 + spawnOffsetX,
                player2->getRect().y + player2->getRect().h / 2 - 8 + spawnOffsetY,
                bulletDirX, bulletDirY,
                "assets/bullet_spritesheet.png"
            );
            bullets.push_back(bullet);

			// --- Play Fire Sound Effect ---
			Mix_PlayChannel(-1, fireSound, 0); // Play fire sound effect
        }
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

    // --- DEBUGGING: Print Map Data ---
    std::cout << "Map Width: " << mapWidth << ", Map Height: " << mapHeight << std::endl;
    std::cout << "Map Data:" << std::endl;
    for (int row = 0; row < mapHeight; ++row) {
        for (int col = 0; col < mapWidth; ++col) {
            std::cout << map[row][col];
        }
        std::cout << std::endl;
    }
    // --- DEBUGGING END ---

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
            case 3:
                SDL_RenderCopy(renderer, wall2Texture_damaged1, NULL, &destRect);  // Draw wall texture
                break;
            case 4:
                SDL_RenderCopy(renderer, wall2Texture_damaged2, NULL, &destRect);  // Draw wall texture
                break;
            case 8:
                SDL_RenderCopy(renderer ,targetP1Texture, NULL, &destRect); // Draw target1 texture
                break;
            case 5:
                SDL_RenderCopy(renderer, targetP2Texture, NULL, &destRect); // Draw target2 texture
                break;
            case 7:
                SDL_RenderCopy(renderer, buff3x3Texture, NULL, &destRect); // Draw target2 texture
                break;
            case 9:
                SDL_RenderCopy(renderer, buff5x5Texture, NULL, &destRect); // Draw target2 texture
                break;
            }
        }
    }
}

#include <string>

void Game::copyFileContent(const std::string& sourceFilePath, const std::string& destinationFilePath) {
    std::ofstream destFileClear(destinationFilePath, std::ios::trunc);
    destFileClear.close();

    std::ifstream sourceFile(sourceFilePath);
    if (!sourceFile.is_open()) {
        std::cerr << "Khong the mo tep nguon " << sourceFilePath << std::endl;
        return;
    }

    std::ofstream destFile(destinationFilePath, std::ios::app);
    if (!destFile.is_open()) {
        std::cerr << "Khong the mo tep dich " << destinationFilePath << std::endl;
        sourceFile.close(); 
        return;
    }

    std::string line;
    while (std::getline(sourceFile, line)) {
        destFile << line << std::endl;
    }

    sourceFile.close();
    destFile.close();

    std::cout << "Noi dung cua '" << sourceFilePath << "' da duoc sao chep vao '" << destinationFilePath << "'" << std::endl;
}

void Game::renderGameOver() {
    // Black background for Game Over screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black
    SDL_RenderClear(renderer);

    SDL_Color winTextColor = { 255, 255, 255, 255 }; // White text for win message
    SDL_Color menuTextColor = { 200, 200, 200, 255 }; // Grayish for "click to menu"

    // Determine win message based on winner
    std::string winMessage;
    if (Game::getWinner() == 1) {
        winMessage = "Player 1 Wins!";
    }
    else if (Game::getWinner() == 2) {
        winMessage = "Player 2 Wins!";
    }
    else {
        winMessage = "It's a Tie! (How did that happen?)"; // Should not happen in your win condition, but good to have
    }

    // 1. Render "Player X Wins!" Text
    SDL_Surface* winSurface = TTF_RenderText_Solid(titlefont, winMessage.c_str(), winTextColor);
    SDL_Texture* winTexture = SDL_CreateTextureFromSurface(renderer, winSurface);
    SDL_Rect winRect;
    winRect.w = winSurface->w;
    winRect.h = winSurface->h;
    winRect.x = SCREEN_WIDTH / 2 - winSurface->w / 2; // Center horizontally
    winRect.y = SCREEN_HEIGHT / 2 - winSurface->h / 2 - 50; // Position a bit above center
    SDL_RenderCopy(renderer, winTexture, NULL, &winRect);
    SDL_FreeSurface(winSurface);
    SDL_DestroyTexture(winTexture);

    // 2. Render "Click anywhere to return to menu" Text
    SDL_Surface* menuSurface = TTF_RenderText_Solid(font, "Click anywhere to return to menu", menuTextColor);
    SDL_Texture* menuTexture = SDL_CreateTextureFromSurface(renderer, menuSurface);
    SDL_Rect menuRect;
    menuRect.w = menuSurface->w;
    menuRect.h = menuSurface->h;
    menuRect.x = SCREEN_WIDTH / 2 - menuSurface->w / 2; // Center horizontally
    menuRect.y = winRect.y + winRect.h + 30; // Position below win message
    SDL_RenderCopy(renderer, menuTexture, NULL, &menuRect);
    SDL_FreeSurface(menuSurface);
    SDL_DestroyTexture(menuTexture);
}

void Game::resetGame() {
    gameState = MENU; // Set game state back to MENU
    winner = 0;      // Reset winner

    player1->respawn(); // Respawn players to starting positions
    player2->respawn();

    bullets.clear();    // Clear all bullets

    Game::copyFileContent("assets/scr_map.txt", "assets/map.txt"); // Reload map from source
    loadMap("assets/map.txt"); // Actually load the map data again

    std::cout << "Game Reset and Back to Menu" << std::endl;
}
