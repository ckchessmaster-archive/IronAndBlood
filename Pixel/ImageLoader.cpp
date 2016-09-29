#include "ImageLoader.h"
#include "picoPNG.h"
#include "IOManager.h"
#include "Errors.h"

namespace Pixel
{
	GLTexture ImageLoader::loadPNG(std::string filePath)
	{
		GLTexture texture = {}; //Create a GLTexture and initialize all its fields to 0

		std::vector<unsigned char> in; //This is the input data to decodePNG, which we load from a file
		std::vector<unsigned char> out; //This is the output data from decodePNG, which is the pixel data for our texture

		unsigned long width, height;

		//Read in the image file contents into a buffer
		if (IOManager::readFileToBuffer(filePath, in) == false)
			fatalError("Failed to load PNG file to buffer!");
		//end if

		//Decode the .png format into an array of pixels
		int errorCode = decodePNG(out, width, height, &(in[0]), in.size());
		if (errorCode != 0)
			fatalError("decodePNG failed with error: " + std::to_string(errorCode));
		//end if

		glGenTextures(1, &(texture.id)); //Generate the openGL texture object

		glBindTexture(GL_TEXTURE_2D, texture.id); //Bind the texture object
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(out[0])); //Upload the pixels to the texture

		//Set some texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		glGenerateMipmap(GL_TEXTURE_2D); //generate the mipmaps

		glBindTexture(GL_TEXTURE_2D, 0); //unbind the texture

		texture.width = width;
		texture.height = height;

		return texture; //return a copy of the texture data
	}//end ImageLoader
}