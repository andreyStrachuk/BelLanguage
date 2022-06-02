CC=g++

CFLAGS=-Werror -Wpedantic -Wextra

SRC_ASM=Assembler/Assembler.cpp Assembler/general.cpp lib/stack.cpp lib/dump.cpp lib/verificator.cpp lib/cmdslib.cpp
SRC_CPU=CPU/CPU.cpp CPU/service.cpp lib/stack.cpp lib/dump.cpp lib/verificator.cpp lib/cmdslib.cpp

Assembler=asm
Processor=cpu

all: $(Assembler) $(SRC_ASM) $(Processor) $(SRC_CPU)

$(Assembler): $(SRC_ASM)
	$(CC) $(SRC_ASM)   $(CFLAGS) -o $(Assembler)


$(Processor): $(SRC_CPU)
	$(CC) $(SRC_CPU) $(CFLAGS) -o $(Processor)
