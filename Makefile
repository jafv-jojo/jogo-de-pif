CC = gcc
CFLAGS = -Wall -Wextra -std=c11
LIBS = $(shell pkg-config --cflags --libs raylib)

ARQUIVOS = main.c jogo.c palavras.c ranking.c

all:
	$(CC) $(CFLAGS) $(ARQUIVOS) -o termo.exe $(LIBS)

run: all
	./termo.exe

clean:
	rm -f termo.exe
