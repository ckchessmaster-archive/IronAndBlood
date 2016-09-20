#pragma once

#include <SDL/SDL.h>
#include <GL/glew.h>

#include "Sprite.h"
#include "GLSLProgram.h"
#include "GLTexture.h"

enum class GameState {PLAY, EXIT};

class MainGame
{
private:
	SDL_Window* _window;
	int _screenWidth;
	int _screenHeight;

	GameState _gameState;
	
	Sprite _sprite;


	GLSLProgram _colorProgram;
	GLTexture _playerTexture;

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

