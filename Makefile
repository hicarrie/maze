# Specify compiler
CC = gcc

# Source files
SRC = src/*.c

# Executable name
NAME = maze

# RM + flags
RM = rm -f

# Compiler flags
CFLAGS = -g -w

# Linker flags
LFLAGS = -lSDL2 -lm

# SDL Flags
SDLFLAGS = `sdl2-config --cflags --libs`


# Compiles executable
all: $(SRC)
	$(CC) $(SRC) $(CFLAGS) $(LFLAGS) -o $(NAME) $(SDLFLAGS)

# Remove temporary files and executable
clean:
	$(RM) *~ $(NAME)
