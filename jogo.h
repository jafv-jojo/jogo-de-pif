#ifndef JOGO_H
#define JOGO_H

#define TAMANHO 5
#define MAX_TENTATIVAS 6

#define CINZA 0
#define AMARELO 1
#define VERDE 2

typedef struct {
    char palavra[TAMANHO + 1];
    char tentativas[MAX_TENTATIVAS][TAMANHO + 1];
    int cores[MAX_TENTATIVAS][TAMANHO];
    int quantidadeTentativas;
    int venceu;
    int terminou;
} Jogo;

void iniciarJogo(Jogo *jogo, char palavra[]);
int enviarTentativa(Jogo *jogo, char tentativa[]);

#endif