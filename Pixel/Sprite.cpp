#include "Sprite.h"
#include "Vertex.h"
#include "ResourceManager.h"

#include <cstddef>

namespace Pixel
{
	Sprite::Sprite()
	{
		_vboID = 0;
	}


	Sprite::~Sprite()
	{
		if (_vboID != 0) {
			glDeleteBuffers(1, &_vboID);
		}
	}

	//Initializes the sprite VBO. x, y, width, and height are
	//in the normalized device coordinate space. so, [-1, 1]
	void Sprite::init(float x, float y, float width, float height, std::string texturePath)
	{
		//Set up our private vars
		_x = x;
		_y = y;
		_width = width;
		_height = height;

		_texture = ResourceManager::getTexture(texturePath);

		//Generate the buffer if it hasn't already been generated
		if (_vboID == 0)
			glGenBuffers(1, &_vboID);

		Vertex vertexData[6]; //This array will hold our vertex data.

		//First Triangle
		vertexData[0].setPosition(x + width, y + height);
		vertexData[0].setUV(1.0f, 1.0f);

		vertexData[1].setPosition(x, y + height);
		vertexData[1].setUV(0.0f, 1.0f);

		vertexData[2].setPosition(x, y);
		vertexData[2].setUV(0.0f, 0.0f);

		//Second Triangle
		vertexData[3].setPosition(x, y);
		vertexData[3].setUV(0.0f, 0.0f);

		vertexData[4].setPosition(x + width, y);
		vertexData[4].setUV(1.0f, 0.0f);

		vertexData[5].setPosition(x + width, y + height);
		vertexData[5].setUV(1.0f, 1.0f);

		//Color the vertices
		for (int i = 0; i < 6; i++)
			vertexData[i].setColor(255, 0, 255, 255);
		//end for

		vertexData[1].setColor(0, 0, 255, 255);

		vertexData[4].setColor(0, 255, 0, 255);


		glBindBuffer(GL_ARRAY_BUFFER, _vboID); //Tell opengl to bind our vertex buffer object
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW); //Upload the data to the GPU

		glBindBuffer(GL_ARRAY_BUFFER, 0); //Unbind the buffer (optional)
	}

	//Draws the sprite to the screen
	void Sprite::draw()
	{
		glBindTexture(GL_TEXTURE_2D, _texture.id); //bind the texture

		glBindBuffer(GL_ARRAY_BUFFER, _vboID); //Bind the buffer object

		glEnableVertexAttribArray(0); //Tell opengl that we want to use the first attribute array.

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position)); //This is the position attribute pointer
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color)); //This is the color attribute pointer
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv)); //This is the UV attribute pointer

		glDrawArrays(GL_TRIANGLES, 0, 6); //Draw the 6 vertices to the screen

		glDisableVertexAttribArray(0); //Disable the vertex attrib array. This  is NOT optional

		glBindBuffer(GL_ARRAY_BUFFER, 0); //Unbind the VBO
	}
}