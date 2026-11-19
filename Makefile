CC = gcc
CFLAGS = -Wall -Wextra -std=c11
LIBS = $(shell pkg-config --cflags --libs raylib)

ARQUIVOS = main.c jogo.c ranking.c dueto.c quarteto.c
all: termo.exe

termo.exe: $(ARQUIVOS)
	$(CC) $(CFLAGS) $(ARQUIVOS) -o termo.exe $(LIBS)

run: all
	./termo.exe

clean:
	rm -f termo.exe