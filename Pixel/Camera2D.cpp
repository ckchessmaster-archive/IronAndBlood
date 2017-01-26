#include "Camera2D.h"

namespace Pixel
{
	Camera2D::Camera2D() : _screenWidth(500),
		_screenHeight(500),
		_needsMatrixUpdate(true),
		_scale(1.0f),
		_position(0.0f, 0.0f),
		_cameraMatrix(1.0f),
		_orthoMatrix(1.0f)
	{
	}


	Camera2D::~Camera2D()
	{
	}

	void Camera2D::init(int screenWidth, int screenHeight)
	{
		_screenWidth = screenWidth;
		_screenHeight = screenHeight;
		_orthoMatrix = glm::ortho(0.0f, (float)_screenWidth, 0.0f, (float)_screenHeight);
	}//end init

	void Camera2D::update()
	{
		//check to see if the matrix needs updating
		if (_needsMatrixUpdate)
		{
			//transform the matrix
			glm::vec3 translate(-_position.x + _screenWidth / 2, -_position.y + _screenHeight / 2, 0.0f);
			_cameraMatrix = glm::translate(_orthoMatrix, translate);

			//scale the matrix
			glm::vec3 scale(_scale, _scale, 0.0f);
			_cameraMatrix = glm::scale(glm::mat4(1.0f), scale) * _cameraMatrix;

			_needsMatrixUpdate = false;
		}//end if
	}//end update

	glm::vec2 Camera2D::convertScreenToWorld(glm::vec2 screenCoords) {
		// Invert Y direction
		screenCoords.y = _screenHeight - screenCoords.y;
		// Set 0,0 as the center
		screenCoords -= glm::vec2(_screenWidth / 2, _screenHeight / 2);
		// Scale the coordinates
		screenCoords /= _scale;
		// Translate with the camera position
		screenCoords += _position;

		return screenCoords;
	}//end convertScreenToWorld
}