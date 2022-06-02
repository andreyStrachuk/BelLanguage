CC=g++

CFLAGS= -Wpedantic -Wextra -Wall

SRC_FRONT=Frontend/Tokens.cpp Frontend/RecursiveDescent.cpp lib/Text/TextAnalyzer.cpp lib/Tree/Tree.cpp

SRC_BACK=Backend/Backend.cpp Backend/BackendFunctions.cpp lib/Text/TextAnalyzer.cpp lib/Tree/Tree.cpp

SRC_DRIVER=Belcc/main.cpp

Frontend=front
Backend=back
Driver=belcc

all: $(Frontend) $(SRC_FRONT) $(Backend) $(SRC_BACK) $(Driver) $(SRC_DRIVER)

$(Frontend): $(SRC_FRONT)
	$(CC) $(CFLAGS) $(SRC_FRONT) -o $(Frontend)
	
$(Backend): $(SRC_BACK)
	$(CC) $(CFLAGS) $(SRC_BACK) -o $(Backend)

$(Driver):  $(SRC_DRIVER)
	$(CC) $(CFLAGS) $(SRC_DRIVER) -o $(Driver)
