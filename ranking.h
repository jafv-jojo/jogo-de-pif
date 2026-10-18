#ifndef RANKING_H
#define RANKING_H

typedef struct Resultado {
    char nome[30];
    int tentativas;
    int venceu;
    struct Resultado *proximo;
} Resultado;

int carregarRanking(Resultado **inicio, char arquivo[]);
int adicionarResultado(Resultado **inicio, char nome[],
                       int tentativas, int venceu);
int salvarResultado(char arquivo[], char nome[],
                    int tentativas, int venceu);
void liberarRanking(Resultado *inicio);

#endif