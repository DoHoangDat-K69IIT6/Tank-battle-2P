#include "Bullet.h"
#include "TextureManager.h"
#include "Game.h" // Include Game.h for renderer access
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
    }
}

void Bullet::render() {
    if (active) {
        TextureManager::Draw(texture, { 0, 0, 16, 16 }, rect); // Render bullet if active
    }
}