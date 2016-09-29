#include "MainGame.h"
#include <Pixel/Errors.h>

#include <iostream>
#include <string>

MainGame::MainGame() :
	_screenWidth(1024), 
	_screenHeight(768), 
	_time(0.0f), 
	_gameState(GameState::PLAY),
	_maxFPS(60.0f)
{
}//end MainGame constructor


MainGame::~MainGame()
{
}//end MainGame destructor

//This runs the game
void MainGame::run()
{
	initSystems();

	_sprites.push_back(new Pixel::Sprite());
	_sprites.back()->init(-1.0f, -1.0f, 1.0f, 1.0f, "Textures/jimmyJump_pack/PNG/CharacterRight_Standing.png"); //Initialize our sprite
	
	_sprites.push_back(new Pixel::Sprite());
	_sprites.back()->init(0.0f, -1.0f, 1.0f, 1.0f, "Textures/jimmyJump_pack/PNG/CharacterRight_Standing.png"); //Initialize our sprite

	gameLoop(); //This only returns when the game ends
}//end run

 //Initialize SDL and Opengl and whatever else we need
void MainGame::initSystems()
{
	Pixel::init();

	_window.create("Game Engine", _screenWidth, _screenHeight, 0);

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
		//used for frame time measuring
		float startTicks = SDL_GetTicks(); 

		processInput();
		_time += 0.08f;
		drawGame();
		calculateFPS();

		//print only once every 10 frames
		static int frameCounter = 0;
		frameCounter++;
		if (frameCounter == 10)
		{
			std::cout << _fps << std::endl;
			frameCounter = 0;
		}// end if
		
		
		float frameTicks = SDL_GetTicks() - startTicks;
		//limit the fps to the max fps
		if (1000.0f / _maxFPS > frameTicks)
			SDL_Delay(1000.0f / _maxFPS - frameTicks);
		//end if
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
				//std::cout << evnt.motion.x << " " << evnt.motion.y << std::endl;
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
	GLint textureLocation = _colorProgram.getUniformLocation("mySampler");
	glUniform1i(textureLocation, 0);

	//Set uniforms
	GLuint timeLocation = _colorProgram.getUniformLocation("time");
	glUniform1f(timeLocation, _time);

	//Draw our sprites!
	for (int i = 0; i < _sprites.size(); i++)
	{
		_sprites[i]->draw();
	}

	glBindTexture(GL_TEXTURE_2D, 0); //unbind the texure
	_colorProgram.unuse();

	_window.swapBuffer();
}//end drawGame

void MainGame::calculateFPS()
{
	static const int NUM_SAMPLES = 10;
	static float frameTimes[NUM_SAMPLES];
	static int currentFrame = 0;

	static float prevTicks = SDL_GetTicks();
	
	float currentTicks;
	currentTicks = SDL_GetTicks();

	_frameTime = currentTicks - prevTicks;
	frameTimes[currentFrame % NUM_SAMPLES] = _frameTime;

	prevTicks = currentTicks;

	int count;

	currentFrame++;
	if (currentFrame < NUM_SAMPLES)
		count = currentFrame;
	else
		count = NUM_SAMPLES;
	//end if/else
	
	float frameTimeAverage = 0;
	for (int i = 0; i < count; i++)
		frameTimeAverage += frameTimes[i];
	//end for

	frameTimeAverage /= count;

	//make sure we don't divide by 0
	if (frameTimeAverage > 0)
		_fps = 1000.0f / frameTimeAverage;
	else
		_fps = 60.0f;
	//end if/else
}
