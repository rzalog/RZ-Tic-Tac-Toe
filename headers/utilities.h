// utilities.h -- a collection of helpful functions for game design

#ifndef __UTILITIES_H__
#define __UTILITIES_H__

/*
 * Forward declarations
 */

struct SDL_Renderer;

/*
 * Function prototypes
 */

	// loads an image from a given path and returns an SDL_Texture* 
SDL_Texture* loadImage(std::string path, SDL_Renderer* renderer);

	// checks if a given point is within bounds of a given SDL_Rect
bool isPointInRect(int x, int y, const SDL_Rect& rect);

#endif