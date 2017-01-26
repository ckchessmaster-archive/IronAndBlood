#pragma once

#include <Pixel/Pixel.h>
#include <Pixel/GLSLProgram.h>
#include <Pixel/Window.h>
#include <Pixel/InputManager.h>
#include <Pixel/Camera2D.h>

class MainGame 
{	
private:
	// Initializes the core systems
	void initSystems();

	// Initializes the shaders
	void initShaders();

	// Main game loop for the program
	void gameLoop();

	// Handles input processing
	void processInput();

	// Renders the game
	void drawGame();

	// Member variables
	Pixel::Window _window; //The game window

	Pixel::GLSLProgram _textureProgram; //The sharder program

	Pixel::InputManager _inputManager; //Handles input

	Pixel::Camera2D _camera; //Main camera

public:
	MainGame();
	~MainGame();

	// Runs the game
	void run();
};//end MainGame