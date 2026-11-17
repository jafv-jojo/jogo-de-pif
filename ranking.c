
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ranking.h"

int adicionarResultado(Resultado **inicio, char nome[],
                       int tentativas, int venceu) {

    Resultado *novo = malloc(sizeof(Resultado));

    if (novo == NULL) {
        return 0;
    }

    snprintf(novo->nome, sizeof(novo->nome), "%s", nome);

    novo->tentativas = tentativas;
    novo->venceu = venceu;

    novo->proximo = *inicio;
    *inicio = novo;

    return 1;
}

int carregarRanking(Resultado **inicio, char arquivo[]) {

    FILE *f = fopen(arquivo, "r");

    if (f == NULL) {
        return 0;
    }

    char nome[30];
    int tentativas;
    int venceu;
    int quantidade = 0;

    while (fscanf(f, "%29s %d %d",
                  nome, &tentativas, &venceu) == 3) {

        if (adicionarResultado(inicio, nome,
                               tentativas, venceu)) {
            quantidade++;
        }
    }

    fclose(f);

    return quantidade;
}

int salvarResultado(char arquivo[], char nome[],
                    int tentativas, int venceu) {

    FILE *f = fopen(arquivo, "a");

    if (f == NULL) {
        return 0;
    }

    fprintf(f, "%s %d %d\n", nome, tentativas, venceu);

    fclose(f);

    return 1;
}

void calcularEstatisticas(Resultado *inicio,
                          Estatisticas *estatisticas) {

    if (estatisticas == NULL) {
        return;
    }

    estatisticas->totalPartidas = 0;
    estatisticas->totalVitorias = 0;
    estatisticas->sequenciaAtual = 0;
    estatisticas->melhorSequencia = 0;

    int sequencia = 0;
    int contandoSequenciaAtual = 1;

    Resultado *atual = inicio;

    while (atual != NULL) {

        estatisticas->totalPartidas++;

        if (atual->venceu) {

            estatisticas->totalVitorias++;

            sequencia++;

            if (sequencia > estatisticas->melhorSequencia) {
                estatisticas->melhorSequencia = sequencia;
            }

            if (contandoSequenciaAtual) {
                estatisticas->sequenciaAtual++;
            }

        } else {

            sequencia = 0;
            contandoSequenciaAtual = 0;
        }

        atual = atual->proximo;
    }
}

void liberarRanking(Resultado *inicio) {

    while (inicio != NULL) {

        Resultado *atual = inicio;

        inicio = inicio->proximo;

        free(atual);
    }
}
