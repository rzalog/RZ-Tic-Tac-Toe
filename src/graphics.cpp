// grahpics.cpp -- handles the graphics

/*
 * Includes
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include "graphics.h"

Graphics::Graphics(const char* title, const int init_xpos, const int init_ypos, const int width, const int height)
{
	// initialize pointers to nullptr
	m_window = nullptr;
	m_renderer = nullptr;

	// initialize success flag
	m_creationSuccess = true;

	// create window
	m_window = SDL_CreateWindow(title, init_xpos, init_ypos, width, height, 0);
	if (m_window == nullptr) {
		printf("Unable to create window. SDL Error: %s\n", SDL_GetError());
		m_creationSuccess = false;
	}
	else {
		// create renderer
		m_renderer = SDL_CreateRenderer(m_window, -1, 0);
		if (m_renderer == nullptr) {
			printf("Unable to create renderer. SDL Error: %s\n", SDL_GetError());
			m_creationSuccess = false;
		}
		else {
			SDL_SetRenderDrawColor(m_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		}
	}
}

Graphics::~Graphics()
{
	// destroy renderer and window
	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);
}

SDL_Renderer* Graphics::getRenderer() const
{
	return m_renderer;
}

bool Graphics::wasCreationSuccessful()
{
	return m_creationSuccess;
}