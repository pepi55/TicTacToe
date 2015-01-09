CC = g++ -std=c++11
DEBUG = -ggdb
FLAGS = -march=native -pipe -W -Wall -pedantic -I/usr/include/freetype2
LFLAGS = -lGL -lGLU -lglut -lGLEW -lIL -lILU -lfreetype -lSDL2
VPATH = ./src:./src/gameManager:./src/board:./src/input:./src/player:./src/opengl:./src/opengl/shader:./src/opengl/tex:./src/opengl/tex/data:./src/opengl/font:./src/sdl/util:./src/sdl

SOURCES = ./src/sdl/util/UtilSDL.cpp \
					./src/opengl/shader/ShaderProgram.cpp \
					./src/opengl/tex/Texture.cpp \
					./src/opengl/tex/SpriteSheet.cpp \
					./src/opengl/tex/TexturedPolygon2D.cpp \
					./src/opengl/font/Font.cpp \
					./src/opengl/font/FontProgram2D.cpp \
					./src/player/Player.cpp \
					./src/board/Board.cpp \
					./src/board/Cell.cpp \
					./src/main.cpp

OBJECTS = $(SOURCES:.cpp=.o)
OBJECTS_DEBUG = $(SOURCES:.cpp=$(DEBUG).o)

EXECUTABLE = runWithPrimus

.DEFAULT_GOAL:= debug

release: $(OBJECTS)
	@echo "================="
	@echo "Linking target $@"
	@$(CC) -O2 $(FLAGS) $(LFLAGS) $^ -o $(EXECUTABLE)
	@echo "================="
	@echo "Linking finished"
	@echo "================="

%.o: %.cpp
	@echo "================="
	@echo "Compiling $<"
	@$(CC) -O2 $(FLAGS) -c $< -o $@

debug: $(OBJECTS_DEBUG)
	@echo "================="
	@echo "Linking target $@"
	$(CC) $(DEBUG) $(FLAGS) $(LFLAGS) $^ -o $(EXECUTABLE)-debug
	@echo "================="
	@echo "Linking finished"
	@echo "================="

%$(DEBUG).o: %.cpp
	@echo "================="
	@echo "Compiling $<"
	$(CC) $(DEBUG) $(FLAGS) -c $< -o $@

.PHONY: clean

clean:
ifneq ("$(wildcard $(EXECUTABLE)-debug)", "")
	@echo "================="
	@echo "Removing $(OBJECTS_DEBUG)"
	@rm $(OBJECTS_DEBUG)
	@echo "================="
	@echo "Removing $(EXECUTABLE)-debug"
	@rm $(EXECUTABLE)-debug
	@echo "================="
else ifneq ("$(wildcard $(EXECUTABLE))", "")
	@echo "================="
	@echo "Removing $(OBJECTS)"
	@rm $(OBJECTS)
	@echo "================="
	@echo "Removing $(EXECUTABLE)"
	@rm $(EXECUTABLE)
	@echo "================="
endif
