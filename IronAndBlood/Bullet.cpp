#include "Bullet.h"

#include <Pixel/ResourceManager.h>


Bullet::Bullet(glm::vec2 pos, glm::vec2 dir, float speed, int lifeTime) {
	_lifeTime = lifeTime;
	_position = pos;
	_direction = dir;
	_speed = speed;
}//end Constructor

Bullet::~Bullet() {}

void Bullet::draw(Pixel::SpriteBatch& spriteBatch) {
		
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	static Pixel::GLTexture texture = Pixel::ResourceManager::getTexture("Textures/jimmyJump_pack/PNG/CharacterRight_Standing.png");

	Pixel::Color color;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;

	glm::vec4 posAndSize = glm::vec4(_position.x, _position.y, 30, 30);

	spriteBatch.draw(posAndSize, uv, texture.id, 0.0f, color);
}//end draw

bool Bullet::udpate() {
	_position += _direction * _speed;
	_lifeTime--;
	if (_lifeTime == 0) {
		return true;
	}//end if
	return false;
}//end update
