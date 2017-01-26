#include "MainGame.h"
#include <Pixel/Errors.h>
#include <Pixel/ResourceManager.h>

#include <iostream>
#include <string>

MainGame::MainGame() :
	_screenWidth(1024),
	_screenHeight(768),
	_time(0.0f),
	_gameState(GameState::PLAY),
	_maxFPS(60.0f)
{
	_camera.init(_screenWidth, _screenHeight);
}//end MainGame constructor


MainGame::~MainGame()
{
}//end MainGame destructor

//This runs the game
void MainGame::run()
{
	initSystems();

	//This only returns when the game ends
	gameLoop(); 
}//end run

 //Initialize SDL and Opengl and whatever else we need
void MainGame::initSystems()
{
	Pixel::init();

	_window.create("Game Engine", _screenWidth, _screenHeight, 0);

	initShaders();

	_spriteBatch.init();
	_fpsLimiter.init(_maxFPS);
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
		_fpsLimiter.begin();

		processInput();
		_time += 0.01f;

		_camera.update();

		for (int i = 0; i < _bullets.size();) {
			if (_bullets[i].udpate() == true) {
				_bullets[i] = _bullets.back();
				_bullets.pop_back();
			} else {
				i++;
			}//end if/else
		}//end for

		drawGame();

		_fps = _fpsLimiter.end();

		//print only once every 10 frames
		static int frameCounter = 0;
		frameCounter++;
		if (frameCounter == 10000)
		{
			std::cout << _fps << std::endl;
			frameCounter = 0;
		}// end if
	}//end while
}//end gameLoop

 //Processes input with SDL
void MainGame::processInput()
{
	SDL_Event evnt;

	const float CAMERA_SPEED = 5.0f;
	const float SCALE_SPEED = 0.1f;

	//Will keep looping until there are no more events to process
	while (SDL_PollEvent(&evnt)) {
		switch (evnt.type) {
			case SDL_QUIT:
				_gameState = GameState::EXIT;
				break;
			//end case SDL_QUIT
			case SDL_MOUSEMOTION:
				_inputManager.setMouseCoords(evnt.motion.x, evnt.motion.y);
				break;
			//end case SDL_MOUSEMOTION
			case SDL_KEYDOWN:
				_inputManager.pressKey(evnt.key.keysym.sym);
				break;
			//end case SDL_KEYDOWN
			case SDL_KEYUP:
				_inputManager.releaseKey(evnt.key.keysym.sym);
				break;
			//end case SDL_KEYUP
			case SDL_MOUSEBUTTONDOWN:
				_inputManager.pressKey(evnt.button.button);
				break;
			//end case SDL_MOUSEBUTTONDOWN
			case SDL_MOUSEBUTTONUP:
				_inputManager.releaseKey(evnt.button.button);
				break;
			//end SDL_MOUSEBUTTONUP
		}//end switch
	}//end while loop

	//Handle player input
	if (_inputManager.isKeyPressed(SDLK_w)) {
		_camera.setPosition(_camera.getPosition() + glm::vec2(0.0f, -CAMERA_SPEED));
	}//end if
	if (_inputManager.isKeyPressed(SDLK_s)) {
		_camera.setPosition(_camera.getPosition() + glm::vec2(0.0f, CAMERA_SPEED));
	}//end if
	if (_inputManager.isKeyPressed(SDLK_a)) {
		_camera.setPosition(_camera.getPosition() + glm::vec2(CAMERA_SPEED, 0.0f));
	}//end if
	if (_inputManager.isKeyPressed(SDLK_d)) {
		_camera.setPosition(_camera.getPosition() + glm::vec2(-CAMERA_SPEED, 0.0f));
	}//end if
	if (_inputManager.isKeyPressed(SDLK_q)) {
		_camera.setScale(_camera.getScale() + SCALE_SPEED);
	}//end if
	if (_inputManager.isKeyPressed(SDLK_e)) {
		_camera.setScale(_camera.getScale() - SCALE_SPEED);
	}//end if

	if (_inputManager.isKeyPressed(SDL_BUTTON_LEFT)) {
		glm::vec2 mouseCoords = _inputManager.getMouseCoords();
		mouseCoords = _camera.convertScreenToWorld(mouseCoords);
		
		glm::vec2 playerPosition(0.0f);
		glm::vec2 direction = mouseCoords - playerPosition;
		direction = glm::normalize(direction);

		_bullets.emplace_back(playerPosition, direction, 5.0f, 1000);
	}//end if
}//end processInput

 //Draws the game using the almighty OpenGL
void MainGame::drawGame()
{
	//Set the base depth to 1.0
	glClearDepth(1.0); 
	//Clear the color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

	_colorProgram.use();

	//We are using texture unit 0
	glActiveTexture(GL_TEXTURE0);
	//Get the uniform location
	GLint textureLocation = _colorProgram.getUniformLocation("mySampler");
	//Tell the shader that the texture is in texture unit 0
	glUniform1i(textureLocation, 0);

	//Set the camera matrix
	GLuint pLocation = _colorProgram.getUniformLocation("P");
	glm::mat4 cameramatrix = _camera.getcameraMatrix();
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameramatrix[0][0]));

	_spriteBatch.begin();

	//Create some sprites
	glm::vec4 pos(0.0f, 0.0f, 50.0f, 50.0f);
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	static Pixel::GLTexture texture = Pixel::ResourceManager::getTexture("Textures/jimmyJump_pack/PNG/CharacterRight_Standing.png");
	Pixel::Color color;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;

	//Draw some sprites
	_spriteBatch.draw(pos, uv, texture.id, 0.0f, color);	

	for (int i = 0; i < _bullets.size(); i++) {
		_bullets[i].draw(_spriteBatch);
	}//end for

	_spriteBatch.end();

	_spriteBatch.renderBatch();


	//unbind the texure
	glBindTexture(GL_TEXTURE_2D, 0); 

	//disable the shader
	_colorProgram.unuse();

	//Swap our buffer and draw everything to the screen
	_window.swapBuffer();
}//end drawGame