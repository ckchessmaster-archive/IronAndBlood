#pragma once

#include <SDL/SDL.h>
#include <GL/glew.h>

#include <Pixel/Pixel.h>
#include <Pixel/GLSLProgram.h>
#include <Pixel/GLTexture.h>
#include <Pixel/Sprite.h>
#include <Pixel/Window.h>
#include <Pixel/Camera2D.h>
#include <Pixel/SpriteBatch.h>
#include <Pixel/InputManager.h>
#include <Pixel/Timing.h>

#include <vector>

#include "Bullet.h"

enum class GameState {PLAY, EXIT};

class MainGame
{
private:
	Pixel::Window _window;
	int _screenWidth;
	int _screenHeight;

	GameState _gameState;

	Pixel::GLSLProgram _colorProgram;
	Pixel::Camera2D _camera;

	Pixel::SpriteBatch _spriteBatch;

	Pixel::InputManager _inputManager;
	Pixel::FpsLimiter _fpsLimiter;

	std::vector<Bullet> _bullets;

	float _maxFPS;
	float _fps;
	float _time;

	void initSystems();
	void initShaders();
	void gameLoop();
	void processInput();
	void drawGame();

public:
	MainGame();
	~MainGame();

	void run();

	
};

