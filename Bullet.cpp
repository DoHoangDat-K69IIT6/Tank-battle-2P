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

// Bullet.cpp (modified update function)
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
    vector<vector<int>>& map = Game::getMap(); // Get a reference to the map (important for modification)

    if (mapWidth > 0 && mapHeight > 0 && !map.empty()) {
        bool collisionDetected = false; // Flag to track if any corner collided

        // --- 4-CORNER COLLISION CHECK ---
        SDL_Point corners[4];
        corners[0] = { rect.x, rect.y };                      // Top-left
        corners[1] = { rect.x + rect.w - 1, rect.y };          // Top-right
        corners[2] = { rect.x, rect.y + rect.h - 1 };          // Bottom-left
        corners[3] = { rect.x + rect.w - 1, rect.y + rect.h - 1 }; // Bottom-right

        for (const auto& corner : corners) {
            int tileRow = corner.y / TILE_SIZE;
            int tileCol = corner.x / TILE_SIZE;

            if (tileRow >= 0 && tileRow < mapHeight && tileCol >= 0 && tileCol < mapWidth) {
                int tileType = map[tileRow][tileCol];

                if (tileType == 1) { // Indestructible wall
                    collisionDetected = true;
                    std::cout << "  Bullet Corner Collision with Indestructible Wall! Deactivating bullet." << std::endl;
                    break; // No need to check other corners, collision found
                }
                else if (tileType == 2) { // Destructible Wall (3 hits)
                    map[tileRow][tileCol] = 3; // Change to 1-hit remaining wall (type 3)
                    collisionDetected = true;
                    std::cout << "  Bullet Corner Collision with Destructible Wall (3 hits -> 2 hit)! Deactivating bullet." << std::endl;
                    break;
                }
                else if (tileType == 3) { // Destructible Wall (2 hit remaining)
                    map[tileRow][tileCol] = 4; // Destroy the wall (set to empty)
                    collisionDetected = true;
                    std::cout << "  Bullet Corner Collision with Destructible Wall (2 hit -> 1 hit)! Deactivating bullet." << std::endl;
                    break;
                }
                else if (tileType == 4) { // Destructible Wall (1 hit remaining)
                    map[tileRow][tileCol] = 0; // Destroy the wall (set to empty)
                    collisionDetected = true;
                    std::cout << "  Bullet Corner Collision with Destructible Wall (1 hit -> Destroyed)! Deactivating bullet." << std::endl;
                    break;
                }
            }
        }

        if (collisionDetected) {
            active = false; // Deactivate bullet if any corner collided
        }
    }
}

void Bullet::render() {
    if (active) {
        //TextureManager::Draw(texture, NULL, rect); // Render bullet if active
        SDL_RenderCopy(Game::renderer, texture, NULL, &rect);
    }
}