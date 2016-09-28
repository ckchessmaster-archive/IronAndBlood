#include "MainGame.h"
#include "Errors.h"

#include <iostream>
#include <string>

MainGame::MainGame() :
	_screenWidth(1024), 
	_screenHeight(768), 
	_window(nullptr), 
	_time(0.0f), 
	_gameState(GameState::PLAY),
	_maxFPS(60.0f)
{
}//end MainGame constructor


MainGame::~MainGame()
{
}

//This runs the game
void MainGame::run()
{
	initSystems();

	_sprites.push_back(new Sprite());
	_sprites.back()->init(-1.0f, -1.0f, 1.0f, 1.0f, "Textures/jimmyJump_pack/PNG/CharacterRight_Standing.png"); //Initialize our sprite
	
	_sprites.push_back(new Sprite());
	_sprites.back()->init(0.0f, -1.0f, 1.0f, 1.0f, "Textures/jimmyJump_pack/PNG/CharacterRight_Standing.png"); //Initialize our sprite

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
		float startTicks = SDL_GetTicks(); //used for frame time measuring

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

	SDL_GL_SwapWindow(_window); //Swap our buffer and draw everything to the screen!
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
