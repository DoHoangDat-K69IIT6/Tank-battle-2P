//#include "Bullet.h"
//#include "TextureManager.h"
//#include "Game.h" // Include Game.h for renderer access
//#include <iostream>
//
//using namespace std;
//
//Bullet::Bullet(int startX, int startY, int directionX, int directionY, const char* texturePath)
//    : x(startX), y(startY), velocityX(directionX * 10.0f), velocityY(directionY * 10.0f), active(true) {
//    texture = TextureManager::LoadTexture(texturePath);
//    if (!texture) {
//        std::cerr << "Failed to load bullet texture!" << std::endl;
//    }
//    rect.x = x;
//    rect.y = y;
//    rect.w = 16; // Adjust bullet width as needed
//    rect.h = 16; // Adjust bullet height as needed
//}
//
//Bullet::~Bullet() {
//    SDL_DestroyTexture(texture);
//}
//
//void Bullet::update() {
//    if (!active) return; // If not active, no update needed
//
//    x += static_cast<int>(velocityX); // Update x position based on velocityX
//    y += static_cast<int>(velocityY); // Update y position based on velocityY
//
//    rect.x = x; // Update rect x and y to match the new position
//    rect.y = y;
//
//    // Basic deactivation logic: Bullet goes off-screen (adjust as needed)
//    if (rect.x < 0 || rect.x > SCREEN_WIDTH || rect.y < 0 || rect.y > SCREEN_HEIGHT) {
//        active = false;
//    }
//}
//
//void Bullet::render() {
//    if (active) {
//        TextureManager::Draw(texture, { 0, 0, 16, 16 }, rect); // Render bullet if active
//    }
//}

// Bullet.cpp - Modify the update function

#include "Bullet.h"
#include "TextureManager.h"
#include "Game.h" // Include Game.h for map and renderer access
#include <iostream>

using namespace std;

Bullet::Bullet(int startX, int startY, int directionX, int directionY, const char* texturePath)
    : x(startX), y(startY), velocityX(directionX * 10.0f), velocityY(directionY * 10.0f), active(true) {
    texture = TextureManager::LoadTexture(texturePath);
    if (!texture) {
        std::cerr << "Failed to load bullet texture!" << std::endl;
    }
    rect.x = x;
    rect.y = y;
    rect.w = 16; // Adjust bullet width as needed
    rect.h = 16; // Adjust bullet height as needed
}

Bullet::~Bullet() {
    SDL_DestroyTexture(texture);
}

void Bullet::update() {
    if (!active) return; // If not active, no update needed

    x += static_cast<int>(velocityX); // Update x position based on velocityX
    y += static_cast<int>(velocityY); // Update y position based on velocityY

    rect.x = x; // Update rect x and y to match the new position
    rect.y = y;

    // Basic deactivation logic: Bullet goes off-screen (adjust as needed)
    if (rect.x < 0 || rect.x > SCREEN_WIDTH || rect.y < 0 || rect.y > SCREEN_HEIGHT) {
        active = false;
        return; // Early return if deactivated off-screen
    }

    // --- Bullet-Wall Collision Detection ---
    int mapWidth = Game::getMapWidth();   // Get map dimensions from Game class
    int mapHeight = Game::getMapHeight();
    const std::vector<std::vector<int>>& map = Game::getMap(); // Get map data

    if (mapWidth > 0 && mapHeight > 0 && !map.empty()) { // Check if map is loaded

        int startTileRow = std::max(0, rect.y / TILE_SIZE - 1);
        int endTileRow = std::min(mapHeight / TILE_SIZE - 1, rect.y / TILE_SIZE + 1);
        int startTileCol = std::max(0, rect.x / TILE_SIZE - 1);
        int endTileCol = std::min(mapWidth / TILE_SIZE - 1, rect.x / TILE_SIZE + 1);

        for (int row = startTileRow; row <= endTileRow; ++row) {
            for (int col = startTileCol; col <= endTileCol; ++col) {
                if (map[row][col] == 1 || map[row][col] == 2) { // Wall tiles
                    SDL_Rect wallRect;
                    wallRect.x = col * TILE_SIZE;
                    wallRect.y = row * TILE_SIZE;
                    wallRect.w = TILE_SIZE;
                    wallRect.h = TILE_SIZE;

                    if (rect.x < wallRect.x + wallRect.w &&
                        rect.x + rect.w > wallRect.x &&
                        rect.y < wallRect.y + wallRect.h &&
                        rect.y + rect.h > wallRect.y)
                    {
                        active = false; // Deactivate bullet on wall collision
                        return;       // Exit update after collision
                    }
                }
            }
        }
    }
    // --- End Bullet-Wall Collision Detection ---
}

void Bullet::render() {
    if (active) {
        TextureManager::Draw(texture, { 0, 0, 16, 16 }, rect); // Render bullet if active
    }
}