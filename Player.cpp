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


void Player::move(int dx, int dy, const int mapWidth, const int mapHeight, std::vector<std::vector<int>> map, const Player* otherPlayer) {
    std::cout << "Player::move - Before: x=" << x << ", y=" << y << ", rect.x=" << rect.x << ", rect.y=" << rect.y << std::endl;

    int newX = x + dx;
    int newY = y + dy;

    // --- Collision Detection (AABB) ---
    SDL_Rect newRect;
    newRect.x = newX * TILE_SIZE;
    newRect.y = newY * TILE_SIZE;
    newRect.w = rect.w;
    newRect.h = rect.h;

    // 1. Check for collisions with walls (tile type '1' or '2')
    bool collision = false;

    for (int row = std::max(0, newY - 1); row <= std::min(mapHeight - 1, newY + 1); ++row) {
        for (int col = std::max(0, newX - 1); col <= std::min(mapWidth - 1, newX + 1); ++col) {
            if (map[row][col] == 1 || map[row][col] == 2) {

                SDL_Rect wallRect;
                wallRect.x = col * TILE_SIZE;
                wallRect.y = row * TILE_SIZE;
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
        rect.x = x * TILE_SIZE;
        rect.y = y * TILE_SIZE;
    }

    std::cout << "Player::move - After:  x=" << x << ", y=" << y << ", rect.x=" << rect.x << ", rect.y=" << rect.y << std::endl; // Debug print
}

void Player::respawn() {
    x = startX;
    y = startY;
    rect.x = x * TILE_SIZE;
    rect.y = y * TILE_SIZE;
}