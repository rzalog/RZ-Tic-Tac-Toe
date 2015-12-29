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
 	// contains optional color arguments (for color keying that are by default set to color key to white)
SDL_Texture* loadImage(SDL_Renderer* renderer, std::string path, bool haveColorKey = true, Uint8 R = 0xFF, Uint8 G = 0xFF, Uint8 B = 0xFF);

	// checks if a given point is within bounds of a given SDL_Rect
bool isPointInRect(int x, int y, const SDL_Rect& rect);

#endif