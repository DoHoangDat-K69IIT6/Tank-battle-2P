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
    if (!active) return;

    x += static_cast<int>(velocityX);
    y += static_cast<int>(velocityY);
    rect.x = x;
    rect.y = y;

    if (rect.x < 0 || rect.x > SCREEN_WIDTH || rect.y < 0 || rect.y > SCREEN_HEIGHT) {
        active = false;
        return;
    }

    int mapWidth = Game::getMapWidth();
    int mapHeight = Game::getMapHeight();
    const std::vector<std::vector<int>>& map = Game::getMap();

    if (mapWidth > 0 && mapHeight > 0 && !map.empty()) {
        // --- VERY SIMPLIFIED Collision Check: Check tile at bullet's CENTER ---
        int tileRow = (rect.y + rect.h / 2) / TILE_SIZE; // Get tile row of bullet's center
        int tileCol = (rect.x + rect.w / 2) / TILE_SIZE; // Get tile col of bullet's center

        // --- DEBUG PRINTS - START ---
        std::cout << "Bullet Update: x=" << rect.x << ", y=" << rect.y << std::endl;
        std::cout << "  Checking Tile at Row=" << tileRow << ", Col=" << tileCol << std::endl;
        // --- DEBUG PRINTS - END ---


        // Check if tile coordinates are within map bounds
        if (tileRow >= 0 && tileRow < mapHeight && tileCol >= 0 && tileCol < mapWidth) {
            if (map[tileRow][tileCol] == 1) {
                std::cout << "  SIMPLE COLLISION DETECTED! TileType=" << map[tileRow][tileCol] << ", Bullet deactivated." << std::endl; // Debug
                active = false; // Deactivate bullet if center is in a wall tile
                return;
            }

            /*if (map[tileRow][tileCol] == 2) {
                map[tileRow][tileCol] = 5;
            }*/
        }
    }
}

void Bullet::render() {
    if (active) {
        TextureManager::Draw(texture, { 0, 0, 16, 16 }, rect); // Render bullet if active
    }
}