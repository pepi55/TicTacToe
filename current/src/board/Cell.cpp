#include "Cell.hpp"

Cell::Cell(void) {
	mPosition.x = 0;
	mPosition.y = 0;
	mCellID = 0;

	mCurrentState = DEFAULT;
}

Cell::~Cell(void) {
	mPosition.x = 0;
	mPosition.y = 0;
	mCellID = 0;
}

void Cell::handleEvent(SDL_Event *e, int &activePlayer, float cellID, int &usedIDs) {
	if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP) {
		static float usedCells[9];
		int x, y;
		SDL_GetMouseState(&x, &y);

		bool pressed = false;

		for (int i = 0; i < usedIDs; i++) {
			if (cellID == usedCells[i]) {
				pressed = true;
			}
		}

		if (!pressed) {
			bool inside = true;

			if (x < mPosition.x - 45) {
				inside = false;
			} else if (x > mPosition.x + 45) {
				inside = false;
			} else if (y < mPosition.y - 41) {
				inside = false;
			} else if (y > mPosition.y + 41) {
				inside = false;
			}

			if (inside) {
				switch (e->type) {
					case SDL_MOUSEBUTTONDOWN:
							if (activePlayer == 1) {
								mCurrentState = PLAYER1;

								activePlayer = 2;
							} else if (activePlayer == 2) {
								mCurrentState = PLAYER2;

								activePlayer = 1;
							}

							usedCells[usedIDs++] = cellID;
						break;

					default:
						break;
				}
			}
		}
	}
}

bool Cell::clipCell(void) {
	bool success = true;

	if (loadTextureFromFile32("img/final/cell.png")) {
		FRect cellClip = {0, 0, 900, 822};

		for (int i = 0; i < 3; i++) {
			cellClip.y = 822 * i;
			addClipSprite(cellClip);
		}

		if (!generateDataBuffer()) {
			fprintf(stderr, "Cell.cpp:66: Unable to clip cells!\n");
			success = false;
		}
	} else {
		fprintf(stderr, "Cell.cpp:70: Unable to load cell sprite!\n");
		success = false;
	}

	return success;
}

void Cell::renderCell(void) {
	renderSprite(mCurrentState);
}

SDL_Point Cell::getPosition(void) {
	return mPosition;
}

CellState Cell::getCellState(void) {
	return mCurrentState;
}

void Cell::setCellID(float id) {
	mCellID = id;
}

void Cell::setPosition(int x, int y) {
	mPosition.x = x;
	mPosition.y = y;
}

void Cell::setCellState(CellState state) {
	mCurrentState = state;
}
