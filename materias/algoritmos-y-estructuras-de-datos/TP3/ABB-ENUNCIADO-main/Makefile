CC       := gcc
CFLAGS   := -std=c99 -Wall -Wconversion -Werror -g
LDFLAGS  := -lm

SRCS     := $(wildcard src/*.c)
TEST_SRC := pruebas_alumno.c
EXE     := test
FORMAT_SRCS := src/abb.c pruebas_alumno.c

all: format $(EXE) run

$(EXE): $(TEST_SRC) $(SRCS)
	@$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

format:
	@clang-format -i -style=file $(FORMAT_SRCS)

run: $(EXE)
	./$(EXE)
	make clean

valgrind: $(EXE)
	valgrind --leak-check=full -s ./$(EXE)
	make clean

clean:
	@rm -f $(EXE)

.PHONY: all $(EXE) format run valgrind clean