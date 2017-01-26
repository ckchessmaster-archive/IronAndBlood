#pragma once

#include <glm/glm.hpp>
#include <Pixel/SpriteBatch.h>


class Bullet {

private:
	int _lifeTime;
	float _speed;
	glm::vec2 _direction;
	glm::vec2 _position;
public:
	Bullet(glm::vec2 pos, glm::vec2 dir, float speed, int lifeTime);
	~Bullet();

	void draw(Pixel::SpriteBatch& spriteBatch);

	// Returns true when we are out of life
	bool udpate();
};//end Bullet
