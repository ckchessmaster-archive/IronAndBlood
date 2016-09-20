#include "MainGame.h"
#include "Errors.h"
#include "ImageLoader.h"

#include <iostream>
#include <string>

MainGame::MainGame() :
	_screenWidth(1024), 
	_screenHeight(768), 
	_window(nullptr), 
	_time(0.0f), 
	_gameState(GameState::PLAY)
{
}//end MainGame constructor


MainGame::~MainGame()
{
}

//This runs the game
void MainGame::run()
{
	initSystems();

	_sprite.init(-1.0f, -1.0f, 2.0f, 2.0f); //Initialize our sprite. (temporary)
	
	_playerTexture = ImageLoader::loadPNG("Textures/jimmyJump_pack/PNG/CharacterRight_Standing.png");

	gameLoop(); //This only returns when the game ends
}//end run

 //Initialize SDL and Opengl and whatever else we need
void MainGame::initSystems()
{
	//Initialize SDL
	SDL_Init(SDL_INIT_EVERYTHING);

	_window = SDL_CreateWindow("Iron&Blood", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _screenWidth, _screenHeight, SDL_WINDOW_OPENGL);
	if (_window == nullptr)
		fatalError("SDL Window could not be created!");

	//Set up our OpenGL context
	SDL_GLContext glContext = SDL_GL_CreateContext(_window);
	if (glContext == nullptr)
		fatalError("SDL_GL context could not be created!");

	// glewExperimental = true; //uncommenct if getting many OpenGL related crashes
	GLenum error = glewInit();
	if (error != GLEW_OK)
		fatalError("Could not initialize glew!");

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); //Tell SDL that we want a double buffered window so we dont get any flickering
	glClearColor(0.0f, 0.0f, 1.0f, 1.0); //Set the background color to blue

	initShaders();
}//end initSystems

void MainGame::initShaders()
{
	_colorProgram.compileShaders("Shaders/colorShading.vert", "Shaders/colorShading.frag");
	_colorProgram.addAttribute("vertexPosition");
	_colorProgram.addAttribute("vertexColor");
	_colorProgram.addAttribute("vertexUV");
	_colorProgram.linkShaders();
}//initShaders

 //This is the main game loop for our program
void MainGame::gameLoop() 
{
	//Will loop until we set _gameState to EXIT
	while (_gameState != GameState::EXIT) 
	{
		processInput();
		_time += 0.08f;
		drawGame();
	}
}//end gameLoop

 //Processes input with SDL
void MainGame::processInput() 
{
	SDL_Event evnt;

	//Will keep looping until there are no more events to process
	while (SDL_PollEvent(&evnt)) 
	{
		switch (evnt.type) 
		{
			case SDL_QUIT:
				_gameState = GameState::EXIT;
				break;
			case SDL_MOUSEMOTION:
				std::cout << evnt.motion.x << " " << evnt.motion.y << std::endl;
				break;
		}//end switch
	}//end while loop
}//end processInput

 //Draws the game using the almighty OpenGL
void MainGame::drawGame() 
{

	glClearDepth(1.0); //Set the base depth to 1.0
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clear the color and depth buffer

	_colorProgram.use();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _playerTexture.id);//bind the texture
	GLint textureLocation = _colorProgram.getUniformLocation("mySampler");
	glUniform1i(textureLocation, 0);

	//Set uniforms
	GLuint timeLocation = _colorProgram.getUniformLocation("time");
	glUniform1f(timeLocation, _time);

	_sprite.draw(); //Draw our sprite!

	glBindTexture(GL_TEXTURE_2D, 0); //unbind the texure
	_colorProgram.unuse();

	SDL_GL_SwapWindow(_window); //Swap our buffer and draw everything to the screen!
}//end drawGame
