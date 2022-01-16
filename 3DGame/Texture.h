#pragma once


#include "RendererOGL.h"

#include <string>

using std::string;

class Texture {

public:
	Texture();
	~Texture();

	void unload();

	
	bool loadOGL(RendererOGL& rendererP, const string& filenameP);

	inline SDL_Texture* toSDLTexture() const { return SDLTexture; }

	void updateInfo(int& widthOut, int& heightOut);

	inline int getWidth() const { return width; }
	inline int getHeight() const { return height; }

	void setActive() const;

private:
	string filename;
	SDL_Texture* SDLTexture = nullptr;

	int width;
	int height;

	unsigned int textureID;

};
