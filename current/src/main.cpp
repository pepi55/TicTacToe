#include "sdl/util/UtilSDL.hpp"

int main(void) {
	UtilSDL game;

	if (!game.initSDL()) {
		fprintf(stderr, "Error initializing SDL!\n");
	} else {
		if (!game.loadMedia()) {
			fprintf(stderr, "Error loading media!\n");
		} else {
			game.loop();
		}
	}

	return 0;
}
