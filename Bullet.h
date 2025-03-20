#pragma once
#include <SDL.h>

class Bullet {
public:
    Bullet(int startX, int startY, int directionX, int directionY, const char* texturePath);
    ~Bullet();

    void update();
    void render();

    SDL_Rect getRect() const { return rect; }
    bool isActive() const { return active; }

    void deactivate() { active = false; } // Function to deactivate bullet explicitly
    // bool isActive() const { return active; }

private:

    int x;
    int y;
    float velocityX; // Use float for velocity to allow finer movement control
    float velocityY;
    SDL_Texture* texture;
    SDL_Rect rect;
    bool active; // Flag to indicate if the bullet is active (on screen)
};