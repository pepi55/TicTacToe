#pragma once

#include "Cell.hpp"

class Board {
	public:
		Board(void);
		~Board(void);

		bool genBoard(void);
		bool checkRows(void);

		int getSizeX(void);
		int getSizeY(void);
		SDL_Point getPosition(void);
		Cell *getCell(int x, int y);

		void setPosition(SDL_Point position);

	private:
		static const int mSizeX = 3;
		static const int mSizeY = 3;

		SDL_Point mPosition;
		Cell mBoard[mSizeX][mSizeY];
};
