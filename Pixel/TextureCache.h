#pragma once

#include <map>

#include "GLTexture.h"

namespace Pixel
{
	class TextureCache
	{
	private:
		std::map<std::string, GLTexture> _textureMap;

	public:
		TextureCache();
		~TextureCache();

		GLTexture getTexture(std::string texturePath);
	};
}
