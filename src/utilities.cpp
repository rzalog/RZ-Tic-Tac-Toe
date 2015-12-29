// utilities.cpp -- helpful functions to have
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include "utilities.h"

	// NOTE, haveColorKey, R, G, B all have default arguments
SDL_Texture* loadImage(SDL_Renderer* renderer, std::string path, bool haveColorKey, Uint8 R, Uint8 G, Uint8 B)
{
	// final texture
	SDL_Texture* newTexture = nullptr;

	// surface on which image is to be loaded on
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == nullptr) {
		printf("Unable to load image %s. SDL_image error: %s\n", path.c_str(), IMG_GetError());
	}
	else {
			// color key image
		if (haveColorKey) {
			SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, R, G, B));
		}
		newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
		if (newTexture == nullptr) {
			printf("Unable to create texture from %s. SDL Error: %s\n", path.c_str(), SDL_GetError());
		}
		else {
			SDL_FreeSurface(loadedSurface);
		}
	}

	return newTexture;
}

bool isPointInRect(int x, int y, const SDL_Rect& rect)
{
	// x, y coordinates are within the bounds of the rectangle
	return ( (x > rect.x && x < rect.x + rect.w) && (y > rect.y && y < rect.y + rect.h) );
}