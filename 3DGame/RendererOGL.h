#pragma once

#include "IRenderer.h"
#include "VertexArray.h"
#include "Vector2.h"
#include "Shader.h"

#include <vector>

class RendererOGL : public IRenderer {

public:
	RendererOGL();
	virtual ~RendererOGL();
	RendererOGL(const RendererOGL&) = delete;
	RendererOGL& operator=(const RendererOGL&) = delete;

	bool initialize(Window& window);
	void beginDraw();
	void draw();
	void endDraw();

	void addSprite(class SpriteComponent* sprite);
	void removeSprite(class SpriteComponent* sprite);
	void drawSprite(const Actor& actor, const class Texture& tex, Rectangle srcRect, Vector2 origin, Flip flip) const;

	void addMesh(class MeshComponent* mesh);
	void removeMesh(class MeshComponent* mesh);
	void setViewMatrix(const Matrix4& viewP);

	void close();
	IRenderer::Type type() { return Type::OGL; }

private:
	void drawSprites();
	void drawMeshes();

	Window* window;
	VertexArray* spriteVertexArray;
	SDL_GLContext context;

	std::vector<class SpriteComponent*> sprites;
	std::vector<class MeshComponent*> meshes;

	Shader* shader;

	Matrix4 spriteViewProj;
	Matrix4 view;
	Matrix4 projection;


};
