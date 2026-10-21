CC = gcc
CFLAGS = -Wall -Wextra -g -std=c11 -D_POSIX_C_SOURCE=200809L
OBJS = src/main.o src/parser.o src/executor.o src/builtins.o

mysh: $(OBJS)
# TODO: regra de ligação
clean:
# TODO: remover objetos e executável