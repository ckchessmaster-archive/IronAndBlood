#pragma once

#include <GL/glew.h>
#include "GLTexture.h"

#include <string>

namespace Pixel
{
	//A 2D quad that can be rendered to the screen
	class Sprite
	{
	private:
		int _x;
		int _y;
		int _width;
		int _height;
		GLuint _vboID;
		GLTexture _texture;

	public:
		Sprite();
		~Sprite();

		void init(float x, float y, float width, float height, std::string texturePath);

		void draw();

	};
}
