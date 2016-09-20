#pragma once

#include <GL/glew.h>

//A 2D quad that can be rendered to the screen
class Sprite
{
private:
	int _x;
	int _y;
	int _width;
	int _height;
	GLuint _vboID;

public:
	Sprite();
	~Sprite();

	void init(float x, float y, float width, float height);

	void draw();

};

