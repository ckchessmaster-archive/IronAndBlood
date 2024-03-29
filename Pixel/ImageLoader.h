#pragma once

#include "GLTexture.h"

#include <string>

namespace Pixel
{
	class ImageLoader
	{
	public:
		static GLTexture loadPNG(std::string filePath);
	};
}
