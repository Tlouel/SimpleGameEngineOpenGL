#pragma once

#include "Rectangle.h"
#include "Window.h"
#include "Vector2.h"
#include "Actor.h"
#include "IRenderer.h"

#include <SDL.h>

class RendererSDL : public IRenderer {

public:

	RendererSDL();
	virtual ~RendererSDL();
	RendererSDL(const RendererSDL&) = delete;
	RendererSDL& operator = (const RendererSDL&) = delete;

	bool initialize(Window& window);

	void beginDraw();
	void draw();
	void endDraw();

	void drawRect(const Rectangle& rect) const;
	void addSprite(class SpriteComponent* sprite);
	void removeSprite(class SpriteComponent* sprite);

	
	void drawSprite(const Actor& actor, const class Texture& tex, Rectangle srcRect, Vector2 origin, Flip flip) const;

	SDL_Renderer* toSDLRenderer() const { return SDLRenderer; }
	IRenderer::Type type() { return Type::SDL; }

	void close();


private:

	void drawSprites();

	SDL_Renderer* SDLRenderer = nullptr;
	std::vector<SpriteComponent*>sprites;
};

