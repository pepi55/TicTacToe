#include "Board.hpp"

Board::Board(void) {
}

Board::~Board(void) {
}

bool Board::genBoard(void) {
	bool success = true;

	for (int x = 0; x < mSizeX; x++) {
		for (int y = 0; y < mSizeY; y++) {
			mBoard[x][y].setPosition(mPosition.x + (x * 91), mPosition.y + (y * 83));
			if (mBoard[x][y].clipCell()) {
				mBoard[x][y].setCellID((x + 73.0f) / (y + 9.0f));
			} else {
				fprintf(stderr, "Board:16: Unable to clip cell!\n");
				success = false;
			}
		}
	}

	return success;
}

bool Board::checkRows(void) {
	bool victory = false;

	for (int x = 0; x < mSizeX; x++) {
		if (mBoard[x][0].getCellState() != DEFAULT && mBoard[x][1].getCellState() != DEFAULT && mBoard[x][2].getCellState() != DEFAULT) {
			if (mBoard[x][0].getCellState() == mBoard[x][1].getCellState() && mBoard[x][1].getCellState() == mBoard[x][2].getCellState()) {
				victory = true;

				break;
			}
		}
	}

	for (int y = 0; y < mSizeY; y++) {
		if (mBoard[0][y].getCellState() != DEFAULT && mBoard[1][y].getCellState() != DEFAULT && mBoard[2][y].getCellState() != DEFAULT) {
			if (mBoard[0][y].getCellState() == mBoard[1][y].getCellState() && mBoard[1][y].getCellState() == mBoard[2][y].getCellState()) {
				victory = true;

				break;
			}
		}
	}

	if (mBoard[0][0].getCellState() != DEFAULT && mBoard[1][1].getCellState() != DEFAULT && mBoard[2][2].getCellState() != DEFAULT) {
		if (mBoard[0][0].getCellState() == mBoard[1][1].getCellState() && mBoard[1][1].getCellState() == mBoard[2][2].getCellState()) {
			victory = true;
		}
	}

	if (mBoard[2][0].getCellState() != DEFAULT && mBoard[1][1].getCellState() != DEFAULT && mBoard[0][2].getCellState() != DEFAULT) {
		if (mBoard[2][0].getCellState() == mBoard[1][1].getCellState() && mBoard[1][1].getCellState() == mBoard[0][2].getCellState()) {
			victory = true;
		}
	}

	return victory;
}

int Board::getSizeX(void) {
	return mSizeX;
}

int Board::getSizeY(void) {
	return mSizeY;
}

SDL_Point Board::getPosition(void) {
	return mPosition;
}

Cell *Board::getCell(int x, int y) {
	return &mBoard[x][y];
}

void Board::setPosition(SDL_Point position) {
	mPosition = position;
}
