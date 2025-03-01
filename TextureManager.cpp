#include "TextureManager.h"
#include <iostream>
#include "Game.h"
#include <SDL_image.h> // Include SDL_image header

SDL_Texture* TextureManager::LoadTexture(const char* texture) {
    SDL_Surface* tempSurface = IMG_Load(texture);  // Use IMG_Load from SDL_image
    if (tempSurface == nullptr) {
        std::cerr << "Failed to load image: " << texture << " Error: " << IMG_GetError() << std::endl;
        return nullptr;
    }

    SDL_Texture* tex = SDL_CreateTextureFromSurface(Game::renderer, tempSurface);
    SDL_FreeSurface(tempSurface);  // Don't forget to free the surface!
    return tex;
}

void TextureManager::Draw(SDL_Texture* texture, SDL_Rect sourceRect, SDL_Rect destRect, SDL_RendererFlip flip) {
    SDL_RenderCopyEx(Game::renderer, texture, &sourceRect, &destRect, 0.0, nullptr, flip);
}