#include "Player.h"
#include "TextureManager.h"
#include "Game.h"
#include "Bullet.h"
#include <iostream>

const int SPEED = 5;

using namespace std;


Player::Player(int x, int y, const char* texturePath) : startX(x), startY(y), speed(SPEED), x(x), y(y), fireRate(700), facingDirection(UP) {
    playerTexture = TextureManager::LoadTexture(texturePath);
    if (!playerTexture) {
        cout << "Failed to load player texture!" << endl;
    }

    rect.x = x; 
    rect.y = y;
    rect.w = TILE_SIZE;
    rect.h = TILE_SIZE;

    // Initialize source rectangle (for the UP sprite initially)
    srcRect.x = 0; // Assuming UP sprite is at the first position (x=0) in the sheet
    srcRect.y = 0; // And at the top row (y=0)
    srcRect.w = TILE_SIZE;
    srcRect.h = TILE_SIZE;

}

Player::~Player() {
    SDL_DestroyTexture(playerTexture);
}

void Player::update(int mapWidth, int mapHeight, vector<vector<int>> map) {
    // You might have player updates here later

}

void Player::render() {
    switch (facingDirection) {
        case UP:    srcRect.x = 0 * TILE_SIZE; break;     // Assuming UP is at index 0 in row
        case RIGHT:  srcRect.x = 1 * TILE_SIZE; break;     // Assuming DOWN is at index 1
        case DOWN:  srcRect.x = 2 * TILE_SIZE; break;     // Assuming LEFT is at index 2
        case LEFT: srcRect.x = 3 * TILE_SIZE; break;    // Assuming RIGHT is at index 3
    }

    SDL_RenderCopy(Game::renderer, playerTexture, &srcRect, &rect);
}

void Player::move(int dx, int dy, const int mapWidth, const int mapHeight, vector<vector<int>> map, const Player* otherPlayer) {
    cout << "Player::move - Before: x=" << x << ", y=" << y << ", rect.x=" << rect.x << ", rect.y=" << rect.y << std::endl;

    int newX = x + dx * SPEED;
    int newY = y + dy * SPEED;

    // --- Collision Detection (AABB) ---
    SDL_Rect newRect;
    newRect.x = newX;
    newRect.y = newY;
    newRect.w = rect.w;
    newRect.h = rect.h;

    if (dx < 0) facingDirection = LEFT;
    else if (dx > 0) facingDirection = RIGHT;
    else if (dy < 0) facingDirection = UP;
    else if (dy > 0) facingDirection = DOWN;

    //// 1. Check for collisions with walls (tile type '1' or '2')

    bool collision = false;

    // Iterate through TILES based on PLAYER's POTENTIAL NEW PIXEL POSITION
    int startTileRow = max(0, newY / TILE_SIZE - 1);  // Calculate starting tile row in pixel space
    int endTileRow = std::min(mapHeight / TILE_SIZE - 1, newY / TILE_SIZE + 1); // Calculate ending tile row in pixel space
    int startTileCol = max(0, newX / TILE_SIZE - 1);  // Calculate starting tile col in pixel space
    int endTileCol = std::min(mapWidth / TILE_SIZE - 1, newX / TILE_SIZE + 1); // Calculate ending tile col in pixel space


    for (int row = startTileRow; row <= endTileRow; ++row) { // Iterate through tile rows
        for (int col = startTileCol; col <= endTileCol; ++col) { // Iterate through tile cols
            if (map[row][col] == 1 || map[row][col] == 2 || map[row][col] == 3 || map[row][col] == 4) {
                SDL_Rect wallRect;
                wallRect.x = col * TILE_SIZE; // Wall rect in PIXEL coordinates
                wallRect.y = row * TILE_SIZE; // Wall rect in PIXEL coordinates
                wallRect.w = TILE_SIZE;
                wallRect.h = TILE_SIZE;

                if (newRect.x < wallRect.x + wallRect.w &&
                    newRect.x + newRect.w > wallRect.x &&
                    newRect.y < wallRect.y + wallRect.h &&
                    newRect.y + newRect.h > wallRect.y)
                {
                    collision = true;
                    break;
                }
            }
        }
        if (collision) {
            break;
        }
    }

    // 2. Check for collision with the other player (if otherPlayer is valid)
    if (!collision && otherPlayer != nullptr) { // Important: Check if otherPlayer is not null
        SDL_Rect otherRect = otherPlayer->getRect(); // Get the other player's rect
        if (newRect.x < otherRect.x + otherRect.w &&
            newRect.x + newRect.w > otherRect.x &&
            newRect.y < otherRect.y + otherRect.h &&
            newRect.y + newRect.h > otherRect.y)
        {
            collision = true; // Collision with other player!
        }
    }

    // 3. Only move the player if there was NO collision
    if (!collision) {
        x = newX;
        y = newY;
        rect.x = x;
        rect.y = y;

        

    }

    //std::cout << "Player::move - After:  x=" << x << ", y=" << y << ", rect.x=" << rect.x << ", rect.y=" << rect.y << std::endl; // Debug print
}

void Player::respawn() {
    x = startX;
    y = startY;
    rect.x = x * TILE_SIZE;
    rect.y = y * TILE_SIZE;
}

