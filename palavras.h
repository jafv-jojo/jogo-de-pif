#ifndef PALAVRAS_H
#define PALAVRAS_H

typedef struct {
    char (*lista)[6];
    int quantidade;
    int capacidade;
} BancoPalavras;

int carregarPalavras(BancoPalavras *banco, char arquivo[]);
void liberarPalavras(BancoPalavras *banco);
void escolherPalavra(BancoPalavras *banco, char destino[]);

#endif