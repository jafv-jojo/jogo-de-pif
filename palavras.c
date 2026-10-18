#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "palavras.h"

int carregarPalavras(BancoPalavras *banco, char arquivo[]) {
    FILE *f = fopen(arquivo, "r");

    if (f == NULL) {
        return 0;
    }

    banco->lista = NULL;
    banco->quantidade = 0;
    banco->capacidade = 0;

    char palavra[100];

    while (fscanf(f, "%99s", palavra) == 1) {
        if (strlen(palavra) != 5) {
            continue;
        }

        if (banco->quantidade == banco->capacidade) {
            int novaCapacidade;

            if (banco->capacidade == 0) {
                novaCapacidade = 10;
            } else {
                novaCapacidade = banco->capacidade * 2;
            }

            char (*novaLista)[6] =
                realloc(banco->lista, novaCapacidade * sizeof(*banco->lista));

            if (novaLista == NULL) {
                free(banco->lista);
                banco->lista = NULL;
                banco->quantidade = 0;
                banco->capacidade = 0;
                fclose(f);
                return 0;
            }

            banco->lista = novaLista;
            banco->capacidade = novaCapacidade;
        }

        strcpy(banco->lista[banco->quantidade], palavra);
        banco->quantidade++;
    }

    fclose(f);

    return banco->quantidade > 0;
}

void liberarPalavras(BancoPalavras *banco) {
    free(banco->lista);

    banco->lista = NULL;
    banco->quantidade = 0;
    banco->capacidade = 0;
}

void escolherPalavra(BancoPalavras *banco, char destino[]) {
    int indice = rand() % banco->quantidade;
    strcpy(destino, banco->lista[indice]);
}