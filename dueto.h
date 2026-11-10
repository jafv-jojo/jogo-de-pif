#ifndef DUETO_H
#define DUETO_H

#define DUETO_TAMANHO 5
#define DUETO_MAX_TENTATIVAS 7
#define DUETO_QUANTIDADE_PALAVRAS 2

typedef struct {
    char palavras[DUETO_QUANTIDADE_PALAVRAS][DUETO_TAMANHO + 1];
    char tentativas[DUETO_MAX_TENTATIVAS][DUETO_TAMANHO + 1];
    int cores[DUETO_QUANTIDADE_PALAVRAS][DUETO_MAX_TENTATIVAS][DUETO_TAMANHO];
    int quantidadeTentativas;
    int acertou[DUETO_QUANTIDADE_PALAVRAS];
    int venceu;
    int terminou;
} Dueto;

void iniciarDueto(Dueto *dueto, const char palavra1[], const char palavra2[]);
int enviarTentativaDueto(Dueto *dueto, const char tentativa[]);

#endif