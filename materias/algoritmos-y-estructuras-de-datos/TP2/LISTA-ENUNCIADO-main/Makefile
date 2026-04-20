CC       := gcc
CFLAGS   := -std=c99 -Wall -Wconversion -Werror -g
LDFLAGS  := -lm

SRCS     := $(wildcard src/*.c)
TEST_SRC := pruebas_alumno.c
EXE1     := test
EXE2     := ejemplo

all: $(EXE2) $(EXE1)

$(EXE1): $(TEST_SRC) $(SRCS)
	@$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(EXE2): ejemplo.c $(SRCS)
	@$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

format:
	@clang-format -i -style=file src/cola.c src/pila.c src/lista.c pruebas_alumno.c ejemplo.c

run: $(EXE)
	valgrind --leak-check=full -s ./test

clean:
	@rm -f $(EXE1) $(EXE2)

.PHONY: all $(EXE1) $(EXE2) format clean run