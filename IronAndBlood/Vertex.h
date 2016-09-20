#pragma once

#include <GL/glew.h>

struct Position
{
	float x;
	float y;
};

struct Color
{
	GLubyte r;
	GLubyte g;
	GLubyte b;
	GLubyte a;
};

struct UV
{
	float u;
	float v;
};

/* 
 * We want our vertex to be a multiple of 12 bytes
 * In this case we have 2 floats and 4 GLubytes
 */
struct Vertex
{
	Position position;
	Color color; //4 bytes for fgba
	UV uv; //UV texture coordinates

	void setPosition(float x, float y)
	{
		position.x = x;
		position.y = y;
	}//end setPosition

	void setColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a)
	{
		color.r = r;
		color.g = g;
		color.b = b;
		color.a = a;
	}//end setColor

	void setUV(float u, float v)
	{
		uv.u = u;
		uv.v = v;
	}//end setUV
};//end Vertex