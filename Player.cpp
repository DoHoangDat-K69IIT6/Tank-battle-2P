#include "Player.h"
#include "TextureManager.h"
#include "Game.h"
#include <iostream>

using namespace std;

Player::Player(int x, int y, const char* texturePath) : startX(x), startY(y), x(x), y(y), speed(10) {
    playerTexture = TextureManager::LoadTexture(texturePath);
    if (!playerTexture) {
        cout << "Failed to load player texture!" << endl;
    }

    rect.x = x * TILE_SIZE; // Initialize rect.x based on constructor 'x' and tile size
    rect.y = y * TILE_SIZE; // Initialize rect.y based on constructor 'y' and tile size
    rect.w = TILE_SIZE;
    rect.h = TILE_SIZE;
}

Player::~Player() {
    SDL_DestroyTexture(playerTexture);
}

void Player::update(int mapWidth, int mapHeight, vector<vector<int>> map) {
    // You might have player updates here later
}

void Player::render() {
    TextureManager::Draw(playerTexture, { 0, 0, TILE_SIZE, TILE_SIZE }, rect); // Corrected: Call TextureManager::Draw to render the texture

}

//void Player::move(int dx, int dy, int mapWidth, int mapHeight, vector<vector<int>> map) {
//    int newX = x + dx;
//    int newY = y + dy; // Corrected typo: nexY to newY
//
//    // For now, basic movement without collision - collision will be added later
//    x = newX; // Update player's x position
//    y = newY; // Update player's y position
//
//    rect.x = x * 2; // Update rect.x to match the new player position
//    rect.y = y * 2; // Update rect.y to match the new player position // 10: toc do xe tang
//}

void Player::move(int dx, int dy, const int mapWidth, const int mapHeight, std::vector<std::vector<int>> map) {
    int newX = x + dx;
    int newY = y + dy;

    // --- Collision Detection (AABB) ---
    // 1. Calculate the player's potential new bounding box (rect)
    SDL_Rect newRect;
    newRect.x = newX * TILE_SIZE;
    newRect.y = newY * TILE_SIZE;
    newRect.w = TILE_SIZE; // Use the Player With instead of TILE_SIZE
    newRect.h = TILE_SIZE; // Use the Player Height instead of TILE_SIZE

    // 2. Check for collisions with walls (tile type '1')
    bool collision = false;

    // Iterate through all tiles within a reasonable range around the player
    for (int row = std::max(0, newY - 1); row <= std::min(mapHeight - 1, newY + 1); ++row) {
        for (int col = std::max(0, newX - 1); col <= std::min(mapWidth - 1, newX + 1); ++col) {
            if (map[row][col] == 1 || map[row][col] == 2) { // Check if it's a wall tile

                SDL_Rect wallRect;
                wallRect.x = col * TILE_SIZE;
                wallRect.y = row * TILE_SIZE;
                wallRect.w = TILE_SIZE;
                wallRect.h = TILE_SIZE;

                // AABB collision check
                if (newRect.x < wallRect.x + wallRect.w &&
                    newRect.x + newRect.w > wallRect.x &&
                    newRect.y < wallRect.y + wallRect.h &&
                    newRect.y + newRect.h > wallRect.y)
                {
                    collision = true;
                    break; // Exit inner loop if collision is found
                }
            }
        }
        if (collision) {
            break; // Exit outer loop if collision is found
        }
    }

    // 3. Only move the player if there was NO collision
    if (!collision) {
        x = newX;
        y = newY;
        rect.x = x * TILE_SIZE; // co the thay doi toc do o day
        rect.y = y * TILE_SIZE;
    }
}

void Player::respawn() {
    x = startX;
    y = startY;
    rect.x = x * TILE_SIZE;
    rect.y = y * TILE_SIZE;
}