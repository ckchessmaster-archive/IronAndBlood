#pragma once

#include <SDL/SDL.h>
#include <GL/glew.h>

#include <Pixel/Pixel.h>
#include <Pixel/GLSLProgram.h>
#include <Pixel/GLTexture.h>
#include <Pixel/Sprite.h>
#include <Pixel/Window.h>
#include <Pixel/Camera2D.h>

#include <vector>

enum class GameState {PLAY, EXIT};

class MainGame
{
private:
	Pixel::Window _window;
	int _screenWidth;
	int _screenHeight;

	GameState _gameState;
	
	std::vector<Pixel::Sprite*> _sprites;

	Pixel::GLSLProgram _colorProgram;
	Pixel::Camera2D _camera;

	float _fps;
	float _maxFPS;
	float _frameTime;

	float _time;

	void initSystems();
	void initShaders();
	void gameLoop();
	void processInput();
	void drawGame();
	void calculateFPS();

public:
	MainGame();
	~MainGame();

	void run();

	
};

