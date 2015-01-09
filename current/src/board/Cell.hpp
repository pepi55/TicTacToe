#pragma once

#include "../opengl/tex/SpriteSheet.hpp"
#include "../sdl/SDL.hpp"

enum CellState {
	DEFAULT = 0,
	PLAYER1 = 1,
	PLAYER2 = 2
};

class Cell : public SpriteSheet {
	public:
		Cell(void);
		~Cell(void);

		void handleEvent(SDL_Event *e, int &activePlayer, float cellID, int &usedIDs);
		void renderCell(void);
		bool clipCell(void);

		SDL_Point getPosition(void);
		CellState getCellState(void);

		void setCellID(float id);
		void setPosition(int x, int y);
		void setCellState(CellState state = DEFAULT);

	private:
		float mCellID;
		SDL_Point mPosition;
		CellState mCurrentState;
};
