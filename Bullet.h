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

    bool isBuffBullet; // Thêm bi?n ?? ki?m tra xem ?ây có ph?i ??n buff không

private:

    int x;
    int y;
    float velocityX; // Use float for velocity to allow finer movement control
    float velocityY;
    SDL_Texture* texture;
    SDL_Rect rect;
    SDL_Rect srcRect;
    bool active; // Flag to indicate if the bullet is active (on screen)

    // --- NEW CONSTANTS for Bullet Sprite Sheet ---
    static const int SPRITE_WIDTH;   // Width of each bullet sprite
    static const int SPRITE_HEIGHT;  // Height of each bullet sprite
    static const int SPRITES_PER_ROW; // Number of bullet sprites per row (if applicable)
};