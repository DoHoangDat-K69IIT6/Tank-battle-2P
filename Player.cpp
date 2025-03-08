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

void Player::move(int dx, int dy, int mapWidth, int mapHeight, vector<vector<int>> map) {
    int newX = x + dx;
    int newY = y + dy; // Corrected typo: nexY to newY

    // For now, basic movement without collision - collision will be added later
    x = newX; // Update player's x position
    y = newY; // Update player's y position

    rect.x = x * TILE_SIZE; // Update rect.x to match the new player position
    rect.y = y * TILE_SIZE; // Update rect.y to match the new player position // 10: toc do xe tang
}

void Player::respawn() {
    x = startX;
    y = startY;
    rect.x = x * TILE_SIZE;
    rect.y = y * TILE_SIZE;
}