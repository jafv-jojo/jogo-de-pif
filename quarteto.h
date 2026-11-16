
#ifndef QUARTETO_H
#define QUARTETO_H

#define QUARTETO_TAMANHO 5
#define QUARTETO_MAX_TENTATIVAS 9
#define QUARTETO_QUANTIDADE_PALAVRAS 4

typedef struct {
    char palavras[QUARTETO_QUANTIDADE_PALAVRAS][QUARTETO_TAMANHO + 1];
    char tentativas[QUARTETO_MAX_TENTATIVAS][QUARTETO_TAMANHO + 1];

    int cores[QUARTETO_QUANTIDADE_PALAVRAS]
             [QUARTETO_MAX_TENTATIVAS]
             [QUARTETO_TAMANHO];

    int quantidadeTentativas;
    int acertou[QUARTETO_QUANTIDADE_PALAVRAS];
    int tentativaAcerto[QUARTETO_QUANTIDADE_PALAVRAS];
    int venceu;
    int terminou;
} Quarteto;

void iniciarQuarteto(Quarteto *quarteto,
                    const char palavra1[],
                    const char palavra2[],
                    const char palavra3[],
                    const char palavra4[]);

int enviarTentativaQuarteto(Quarteto *quarteto,
                           const char tentativa[]);

#endif
