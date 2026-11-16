
#include <string.h>
#include "quarteto.h"

void iniciarQuarteto(Quarteto *quarteto,
                    const char palavra1[],
                    const char palavra2[],
                    const char palavra3[],
                    const char palavra4[]) {
    memset(quarteto, 0, sizeof(Quarteto));

    strcpy(quarteto->palavras[0], palavra1);
    strcpy(quarteto->palavras[1], palavra2);
    strcpy(quarteto->palavras[2], palavra3);
    strcpy(quarteto->palavras[3], palavra4);
}

int enviarTentativaQuarteto(Quarteto *quarteto,
                           const char tentativa[]) {
    if (quarteto->terminou ||
        strlen(tentativa) != QUARTETO_TAMANHO ||
        quarteto->quantidadeTentativas >= QUARTETO_MAX_TENTATIVAS) {
        return 0;
    }

    int linha = quarteto->quantidadeTentativas;
    strcpy(quarteto->tentativas[linha], tentativa);

    for (int tab = 0; tab < QUARTETO_QUANTIDADE_PALAVRAS; tab++) {
        if (quarteto->acertou[tab]) {
            continue;
        }

        int usadas[QUARTETO_TAMANHO] = {0};
        const char *palavra = quarteto->palavras[tab];

        for (int i = 0; i < QUARTETO_TAMANHO; i++) {
            if (tentativa[i] == palavra[i]) {
                quarteto->cores[tab][linha][i] = 2;
                usadas[i] = 1;
            }
        }

        for (int i = 0; i < QUARTETO_TAMANHO; i++) {
            if (quarteto->cores[tab][linha][i] == 2) {
                continue;
            }

            for (int j = 0; j < QUARTETO_TAMANHO; j++) {
                if (!usadas[j] && tentativa[i] == palavra[j]) {
                    quarteto->cores[tab][linha][i] = 1;
                    usadas[j] = 1;
                    break;
                }
            }
        }

        if (strcmp(tentativa, palavra) == 0) {
            quarteto->acertou[tab] = 1;
            quarteto->tentativaAcerto[tab] = linha + 1;
        }
    }

    quarteto->quantidadeTentativas++;

    int todasAcertadas = 1;

    for (int tab = 0; tab < QUARTETO_QUANTIDADE_PALAVRAS; tab++) {
        if (!quarteto->acertou[tab]) {
            todasAcertadas = 0;
            break;
        }
    }

    if (todasAcertadas) {
        quarteto->venceu = 1;
        quarteto->terminou = 1;
    } else if (quarteto->quantidadeTentativas ==
               QUARTETO_MAX_TENTATIVAS) {
        quarteto->terminou = 1;
    }

    return 1;
}
