#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

#include "palavras.h"

#define TAMANHO_PALAVRA 5
#define TAMANHO_MAX_RESPOSTA 4096

#define URL_API "https://random-word-api.herokuapp.com/word?number=1&length=5&lang=pt-br"

typedef struct {
    char *dados;
    size_t tamanho;
} RespostaHTTP;

typedef struct {
    const char *utf8;
    char letra;
} Acento;

static size_t receberResposta(void *dados, size_t tamanho,
                              size_t quantidade, void *usuario) {
    size_t bytes = tamanho * quantidade;
    RespostaHTTP *resposta = (RespostaHTTP *)usuario;

    if (bytes > TAMANHO_MAX_RESPOSTA - resposta->tamanho) {
        return 0;
    }

    char *novo = realloc(resposta->dados,
                         resposta->tamanho + bytes + 1);

    if (novo == NULL) {
        return 0;
    }

    resposta->dados = novo;

    memcpy(resposta->dados + resposta->tamanho, dados, bytes);

    resposta->tamanho += bytes;
    resposta->dados[resposta->tamanho] = '\0';

    return bytes;
}

static int converterPalavra(const char *json, char destino[6]) {

    static const Acento acentos[] = {
        {"á", 'A'}, {"à", 'A'}, {"â", 'A'}, {"ã", 'A'},
        {"Á", 'A'}, {"À", 'A'}, {"Â", 'A'}, {"Ã", 'A'},

        {"é", 'E'}, {"è", 'E'}, {"ê", 'E'},
        {"É", 'E'}, {"È", 'E'}, {"Ê", 'E'},

        {"í", 'I'}, {"ì", 'I'}, {"î", 'I'},
        {"Í", 'I'}, {"Ì", 'I'}, {"Î", 'I'},

        {"ó", 'O'}, {"ò", 'O'}, {"ô", 'O'}, {"õ", 'O'},
        {"Ó", 'O'}, {"Ò", 'O'}, {"Ô", 'O'}, {"Õ", 'O'},

        {"ú", 'U'}, {"ù", 'U'}, {"û", 'U'},
        {"Ú", 'U'}, {"Ù", 'U'}, {"Û", 'U'},

        {"ç", 'C'}, {"Ç", 'C'}
    };

    const char *inicio = strchr(json, '"');

    if (inicio == NULL) {
        return 0;
    }

    inicio++;

    int posicao = 0;

    while (*inicio != '\0' && *inicio != '"') {

        unsigned char atual = (unsigned char)*inicio;
        char letra = '\0';
        int bytes = 1;

        if (atual >= 'a' && atual <= 'z') {
            letra = (char)(atual - 'a' + 'A');

        } else if (atual >= 'A' && atual <= 'Z') {
            letra = (char)atual;

        } else if (atual >= 128) {

            int encontrou = 0;

            int totalAcentos =
                (int)(sizeof(acentos) / sizeof(acentos[0]));

            for (int i = 0; i < totalAcentos; i++) {

                int tamanhoAcento = (int)strlen(acentos[i].utf8);

                if (strncmp(inicio, acentos[i].utf8,
                            tamanhoAcento) == 0) {

                    letra = acentos[i].letra;
                    bytes = tamanhoAcento;
                    encontrou = 1;
                    break;
                }
            }

            if (!encontrou) {
                return 0;
            }

        } else {
            return 0;
        }

        if (posicao >= TAMANHO_PALAVRA) {
            return 0;
        }

        destino[posicao] = letra;
        posicao++;

        inicio += bytes;
    }

    if (posicao != TAMANHO_PALAVRA) {
        return 0;
    }

    destino[posicao] = '\0';

    return 1;
}

int inicializarAPI(void) {
    return curl_global_init(CURL_GLOBAL_DEFAULT) == CURLE_OK;
}

int escolherPalavra(char destino[6]) {

    CURL *curl = curl_easy_init();

    if (curl == NULL) {
        return 0;
    }

    RespostaHTTP resposta = {NULL, 0};

    curl_easy_setopt(curl, CURLOPT_URL, URL_API);

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,
                     receberResposta);

    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &resposta);

    curl_easy_setopt(curl, CURLOPT_USERAGENT, "Termo-C/1.0");

    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 5L);

    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);

    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);

    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);

    CURLcode resultado = curl_easy_perform(curl);

    long statusHTTP = 0;

    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &statusHTTP);

    int sucesso = 0;

    if (resultado == CURLE_OK &&
        statusHTTP == 200 &&
        resposta.dados != NULL) {

        sucesso = converterPalavra(resposta.dados, destino);
    }

    if (!sucesso) {

        if (resultado != CURLE_OK) {
            fprintf(stderr, "Erro na API: %s\n",
                    curl_easy_strerror(resultado));

        } else {
            fprintf(stderr, "Nao foi possivel obter uma palavra valida.\n");
        }
    }

    free(resposta.dados);

    curl_easy_cleanup(curl);

    return sucesso;
}

void finalizarAPI(void) {
    curl_global_cleanup();
}