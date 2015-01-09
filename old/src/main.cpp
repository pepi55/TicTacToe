#include "sdl/LSDL.hpp"
#include "util/LUtilSDL.hpp"

//add "int argc, char *args[]" for cross-compat
int main(void) {
	if (!initSDL()) {
		fprintf(stderr, "Failed to initialize SDL!\n");
	} else {
		if (!loadMedia()) {
			fprintf(stderr, "Failed to load media!\n");
		} else {
			mainLoop();
		}
	}

	closeSDL();

	return 0;
}
