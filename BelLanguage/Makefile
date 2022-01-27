CC=g++

CFLAGS=-Werror -Wpedantic -Wextra -Wall

SRC_FRONT=Frontend/Tokens.cpp lib/Text/TextAnalyzer.cpp lib/Tree/Tree.cpp

Frontend=asm
Lib=cpu

all: $(Frontend) $(SRC_FRONT) $(Lib) $(SRC_LIB)

$(Frontend): $(SRC_FRONT)
	$(CC) $(SRC_FRONT)   $(CFLAGS) -o $(Frontend)


$(Lib): $(SRC_LIB)
	$(CC) $(SRC_LIB) $(CFLAGS) -o $(Lib)
