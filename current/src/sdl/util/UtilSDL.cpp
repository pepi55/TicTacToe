#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <cstring>

#include "../../opengl/OpenGL.hpp"
#include "../SDL.hpp"

#include "UtilSDL.hpp"
#include "../../opengl/shader/ColorRGBA.hpp"

#include "../../board/Board.hpp"
#include "../../player/Player.hpp"

enum Menu {
	MAIN_MENU,
	PAUSE_MENU,
	TIED_MENU,
	VICTORY_MENU,
	NO_MENU
};

//Active menu
Menu menu = MAIN_MENU;

//Board
Board board;
int boardSizeX = board.getSizeX(),
		boardSizeY = board.getSizeY();

//Cells
int usedCellIDs = 0; //Keeping track of used cells
ColorRGBA cellColor = {1.0f, 1.0f, 1.0f, 1.0f}; //Cell's color modification
FRect cell = {0.0f, 0.0f, 900.0f, 822.0f}; //Cell's dimensions

//Shader program for the cells to use
TexturedPolygon2D cellProgram2D;

//Amount of players
Player player1;
Player player2;

//Screen text
std::string textInput = "";
std::string textOutput = "";
FRect stringArea;
FRect screenArea;
ColorRGBA fontColor = {0.0f, 0.0f, 0.0f, 1.0f}; //Text color modification

//Player turn
int activePlayer = 1; //In case more players join
bool pause = false;

UtilSDL::UtilSDL(void) : SCREEN_WIDTH(640), SCREEN_HEIGHT(480), SCREEN_POS_X(SDL_WINDOWPOS_UNDEFINED), SCREEN_POS_Y(SDL_WINDOWPOS_UNDEFINED), SCREEN_TITLE("Tic-Tac-Toe") {
	mQuit = false;

	mWindow = NULL;
	mContext = NULL;
}

UtilSDL::~UtilSDL(void) {
	close();
}

bool UtilSDL::initSDL(void) {
	bool success = true;
	int sdlFlags = SDL_INIT_VIDEO;

	if (SDL_Init(sdlFlags) != 0) {
		fprintf(stderr, "Could not initialize SDL!\nSDL_Error: %s\n", SDL_GetError());
		success = false;
	} else {
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		mWindow = SDL_CreateWindow(SCREEN_TITLE, SCREEN_POS_X, SCREEN_POS_Y, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
		if (mWindow == NULL) {
			fprintf(stderr, "Window could not be created!\nSDL_Error: %s\n", SDL_GetError());
			success = false;
		} else {
			mContext = SDL_GL_CreateContext(mWindow);

			if (mContext == NULL) {
				fprintf(stderr, "OpenGL context could not be created!\nSDL_Error: %s\n", SDL_GetError());
				success = false;
			} else {
				glewExperimental = GL_TRUE;

				GLenum glewError = glewInit();
				if (glewError != GLEW_OK) {
					fprintf(stderr, "Error initializing GLEW!\n%s\n", glewGetErrorString(glewError));
				}

				glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

				glEnable( GL_BLEND );
				glDisable( GL_DEPTH_TEST );
				glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

				if (SDL_GL_SetSwapInterval(1) < 0) {
					fprintf(stderr, "Waring: Unable to set VSync!\nError: %s\n", SDL_GetError());
				}

				ilInit();
				iluInit();
				ilClearColour(255, 255, 255, 000);

				ILenum ilError = ilGetError();
				if (ilError != IL_NO_ERROR) {
						fprintf(stderr, "Error initializing DevIL! %s\n", iluErrorString(ilError));
						return false;
				}

				if (!initGL()) {
					fprintf(stderr, "Unable to initialize OpenGL!\n");
					success = false;
				}
			}
		}
	}

	return success;
}

bool UtilSDL::initGL(void) {
	bool success = true;

	if (!cellProgram2D.loadProgram()) {
		fprintf(stderr, "Unable to load board shader!\n");
		success = false;
	} else {
		cellProgram2D.bind();
		cellProgram2D.setProjection(glm::ortho<GLfloat>(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 1.0f, -1.0f));
		cellProgram2D.updateProjection();

		cellProgram2D.setModelView(glm::mat4());
		cellProgram2D.updateModelView();

		cellProgram2D.setTextureUnit(0);

		Texture::setTexturedPolygon2D(&cellProgram2D);
		cellProgram2D.unbind();
	}

	if (!mFontProgram.loadProgram()) {
		fprintf(stderr, "Unable to load text shader!\n");
		success = false;
	} else {
		mFontProgram.bind();
		mFontProgram.setProjection(glm::ortho<GLfloat>(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 1.0f, -1.0f));
		mFontProgram.updateProjection();

		mFontProgram.setModelView(glm::mat4());
		mFontProgram.updateModelView();

		mFontProgram.setTextureUnit(0);

		Font::setFontProgram2D(&mFontProgram);
		mFontProgram.unbind();
	}

	return success;
}

bool UtilSDL::loadMedia(void) {
	bool success = true;

	if (!mFont.loadFreetype("img/font/lazy.ttf", 60)) {
		fprintf(stderr, "Unable to load font!\n");
		success = false;
	}

	SDL_Point boardPosition;
	boardPosition.x = SCREEN_WIDTH / 2 - 91;
	boardPosition.y = SCREEN_HEIGHT / 2 - 83;
	board.setPosition(boardPosition);

	if (!board.genBoard()) {
		fprintf(stderr, "Failed to generate board!\n");
		success = false;
	}

	return success;
}

void UtilSDL::loop(void) {
	while (!mQuit) {
		input();
		render();
	}

	close();
}

void UtilSDL::input(void) {
	SDL_Event e;
	SDL_StartTextInput();

	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT || e.key.keysym.sym == SDLK_ESCAPE) {
			mQuit = true;
		} else if (e.type == SDL_KEYDOWN) {
			if (e.key.keysym.sym == SDLK_RETURN) {
				if (menu == MAIN_MENU) {
					if (activePlayer == 1) {
						while (textInput.back() == ' ') {
							textInput.erase(textInput.end() - 1);
						}

						player1.setName(textInput);
						textInput.clear();

						activePlayer = 2;
					} else {
						while (textInput.back() == ' ') {
							textInput.erase(textInput.end() - 1);
						}

						player2.setName(textInput);
						textInput.clear();
						textOutput.clear();

						activePlayer = 1;
						menu = NO_MENU;
					}
				} else if (menu == TIED_MENU || menu == VICTORY_MENU) {
					reset();
				}
			} else if (e.key.keysym.sym == SDLK_BACKSPACE) {
				if (!textInput.empty()) {
					textInput.erase(textInput.end() - 1);
				}
			} else if (e.key.keysym.sym == SDLK_SPACE) {
				if (!pause && menu == NO_MENU) {
					menu = PAUSE_MENU;
					pause = true;
				} else if (menu == PAUSE_MENU) {
					menu = NO_MENU;
					pause = false;
				}
			}
		} else if (e.type == SDL_TEXTINPUT) {
			textInput += e.text.text;
		}

		if (menu == NO_MENU) {
			for (int x = 0; x < boardSizeX; ++x) {
				for (int y = 0; y < boardSizeY; ++y) {
					Cell *tempCell = board.getCell(x, y);

					tempCell->handleEvent(&e, activePlayer, (x + 73.0f) / (y + 9.0f), usedCellIDs);
				}
			}
		}

		if (board.checkRows()) {
			menu = VICTORY_MENU;
		}

		if (usedCellIDs == boardSizeX * boardSizeY) {
			menu = TIED_MENU;
		}
	}

	SDL_StopTextInput();
}

void UtilSDL::render(void) {
	screenArea.w = SCREEN_WIDTH;
	screenArea.h = SCREEN_HEIGHT;

	glClear(GL_COLOR_BUFFER_BIT);

	switch (menu) {
		case MAIN_MENU:
			stringArea = mFont.getStringArea("Please enter your name:");

			mFontProgram.bind();
			mFontProgram.setTextColor(fontColor);

			mFontProgram.setModelView(glm::mat4());
			mFont.renderText(0.0f, 0.0f, "Please enter your name:");

			if (activePlayer == 1) {
				textOutput = "Player 1: ";
			} else {
				textOutput = "Player 2: ";
			}

			mFontProgram.setModelView(glm::mat4());
			mFont.renderText(0.0f, stringArea.h, textOutput);
			mFont.renderText(0.0f, 0.0f, textInput);

			mFontProgram.setModelView(glm::mat4());
			mFont.renderText(0.0f, 0.0f, "Press \"Enter\" to continue...", &screenArea, FONT_TEXT_ALIGN_BOTTOM | FONT_TEXT_ALIGN_CENTERED_H);
			mFontProgram.unbind();
			break;

		case PAUSE_MENU:
			mFontProgram.bind();

			textOutput = "PAUSED";

			mFontProgram.setModelView(glm::mat4());
			mFont.renderText(0.0f, 0.0f, textOutput, &screenArea, FONT_TEXT_ALIGN_CENTERED_H | FONT_TEXT_ALIGN_CENTERED_V);

			mFontProgram.unbind();
			break;

		case NO_MENU:
			if (activePlayer == 1) {	//Player1 plays
				textOutput = "It's " + player1.getName() + "'s turn!";

				mFontProgram.bind();
				mFontProgram.setModelView(glm::mat4());
				mFont.renderText(0.0f, 0.0f, textOutput, &screenArea, FONT_TEXT_ALIGN_CENTERED_H | FONT_TEXT_ALIGN_TOP);
				mFontProgram.unbind();
			} else {									//Player2 plays
				textOutput = "It's " + player2.getName() + "'s turn!";

				mFontProgram.bind();
				mFontProgram.setModelView(glm::mat4());
				mFont.renderText(0.0f, 0.0f, textOutput, &screenArea, FONT_TEXT_ALIGN_CENTERED_H | FONT_TEXT_ALIGN_TOP);
				mFontProgram.unbind();
			}

			cellProgram2D.bind();
			cellProgram2D.setTextureColor(cellColor);

			for (int x = 0; x < boardSizeX; ++x) {
				for (int y = 0; y < boardSizeY; ++y) {
					Cell *tempCell = board.getCell(x, y);

					cellProgram2D.setModelView(glm::scale(glm::mat4(), glm::vec3(0.1f, 0.1f, 0.0f)));
					cellProgram2D.updateModelView();

					cellProgram2D.leftMultModelView(glm::translate<GLfloat>(glm::vec3(
									tempCell->getPosition().x, //boardPosition.x + ((cell.w * 0.1f + 1.0f) * x)
									tempCell->getPosition().y, //boardPosition.y + ((cell.h * 0.1f + 1.0f) * y)
									0.0f
									)));
					cellProgram2D.updateModelView();

					tempCell->renderCell();
				}
			}

			cellProgram2D.unbind();
			break;

		case TIED_MENU:
			textOutput = "It's a tied game!";

			mFontProgram.bind();
			mFontProgram.setModelView(glm::mat4());
			mFont.renderText(0.0f, 100.0f, textOutput, &screenArea, FONT_TEXT_ALIGN_CENTERED_H);

			mFontProgram.setModelView(glm::mat4());
			mFont.renderText(0.0f, 200.0f, "Want to try again?\nPress enter!", &screenArea, FONT_TEXT_ALIGN_CENTERED_H);

			mFontProgram.setModelView(glm::mat4());
			mFont.renderText(0.0f, 0.0f, "Or press escape to quit\nat any time.", &screenArea, FONT_TEXT_ALIGN_CENTERED_H | FONT_TEXT_ALIGN_BOTTOM);
			mFontProgram.unbind();
			break;

		case VICTORY_MENU:
			if (activePlayer == 2) {
				textOutput = "The winner is: " + player1.getName() + "!";
			} else {
				textOutput = "The winner is: " + player2.getName() + "!";
			}

			mFontProgram.bind();
			mFontProgram.setModelView(glm::mat4());
			mFont.renderText(0.0f, 100.0f, textOutput, &screenArea, FONT_TEXT_ALIGN_CENTERED_H);

			mFontProgram.setModelView(glm::mat4());
			mFont.renderText(0.0f, 200.0f, "Want to play again?\nPress enter!", &screenArea, FONT_TEXT_ALIGN_CENTERED_H);

			mFontProgram.setModelView(glm::mat4());
			mFont.renderText(0.0f, 0.0f, "Or press escape to quit\nat any time.", &screenArea, FONT_TEXT_ALIGN_CENTERED_H | FONT_TEXT_ALIGN_BOTTOM);
			mFontProgram.unbind();
			break;

		default:
			break;
	}

	SDL_GL_SwapWindow(mWindow);
}

void UtilSDL::reset(void) {
	menu = MAIN_MENU;
	usedCellIDs = 0;
	activePlayer = 1;

	for (int x = 0; x < boardSizeX; ++x) {
		for (int y = 0; y < boardSizeY; ++y) {
			Cell *tempCell = board.getCell(x, y);

			tempCell->setCellState();
		}
	}
}

void UtilSDL::close(void) {
	SDL_DestroyWindow(mWindow);

	mWindow = NULL;
	mContext = NULL;

	SDL_Quit();
}
