#pragma once
#include <SDL.h>
#include <vector>
#include "Bullet.h"
using namespace std;

enum FacingDirection {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

class Player {
public:
    Player(int x, int y, const char* texturePath); // Constructor
    ~Player();                                    // Destructor

    void update(int mapWidth, int mapHeight, vector<vector<int>> map); // Update logic (currently empty)
    void render();                                    // Render the player
    void move(int dx, int dy, const int mapWidth, const int mapHeight, vector<vector<int>> map, const Player* otherPlayer);     // Move the player
    void respawn();

    void shoot(vector<Bullet*>& bullets, SDL_Texture* bulletTexture);

    SDL_Rect getRect() const { return rect; }          // Get the player's rectangle (for collision later)

    FacingDirection getFacingDirection() const { return facingDirection; }
private:
    FacingDirection facingDirection;

    int startX;
    int startY;
    int x;             // X coordinate in tile units
    int y;             // Y coordinate in tile units
    int speed;         // Movement speed (pixels per step)
    SDL_Texture* playerTexture; // Texture for the player's image
    SDL_Rect rect;      // Rectangle to define player's position and size
};
