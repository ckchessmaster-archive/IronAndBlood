#include "TextureCache.h"
#include "ImageLoader.h"

#include <iostream>

namespace Pixel
{
	TextureCache::TextureCache()
	{
	}


	TextureCache::~TextureCache()
	{
	}

	GLTexture TextureCache::getTexture(std::string texturePath)
	{
		//lookup the texture and see if its in the map
		auto mit = _textureMap.find(texturePath); //auto in this case is equivalanet to this: std::map<std::string, GLTexture>::iterator

		//check if its not in the map
		if (mit == _textureMap.end())
		{
			GLTexture newTexture = ImageLoader::loadPNG(texturePath); //load the texture

			_textureMap.insert(make_pair(texturePath, newTexture)); //insert the texture into the map

			std::cout << "Loaded Texture!\n";
			return newTexture;
		}

		std::cout << "Used Cached Texture!\n";
		return mit->second;
	}
}