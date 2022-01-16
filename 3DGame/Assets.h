#pragma once

#include "Texture.h"
#include "Shader.h"
#include "Mesh.h"

#include <map>
#include <string>

using std::map;
using std::string;

//A static singleton Assets class that hosts several functions to load resources. 
//Each loaded resource is also stored for future reference by string handles.
//All functions and resources are static and no public constructor is defined.

class Assets {
	
public:
	static map<string, Texture> textures;
	static map<string, Shader> shaders;
	static map<string, Mesh> meshes;

	//Loads a texture from file
	static Texture loadTexture(IRenderer& renderer, const string& filename, const string& name);

	//Retrieves a stored texture
	static Texture& getTexture(const string& name);

	//Loads Shader
	static Shader loadShader(const string& vShaderFile, const string& fShaderFile, const string& tcShaderFile, const string& teShaderFile,
		const string& gShaderFile, const string& name);

	//Load Mesh
	static Mesh loadMesh(const string& filename, const string& name);

	//Retrieves a stored shader
	static Shader& getShader(const string& name);

	//Retrieves mesh
	static Mesh& getMesh(const string& name);
	//Properly de-allocates all loaded resources
	static void clear();

private:
	Assets(){}

	//Loads a single texture from file
	static Texture loadTextureFromFile(IRenderer& renderer, const string& filename);

	//Loads and generates a shader from file
	static Shader loadShaderFromFile(const string& vShaderFile, const string& fShaderFile, const string& tcShaderFile = "", 
		const string& teShaderFile = "",const string& gShaderFile = "");

	static Mesh loadMeshFromFile(const string& filename);
};

