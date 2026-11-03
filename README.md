# Termo

Projeto final da disciplina de Programação Imperativa e Funcional, desenvolvido em linguagem C utilizando a biblioteca Raylib.

## Sobre o jogo

Termo é um jogo de adivinhação de palavras. O objetivo é descobrir uma palavra secreta de cinco letras em até seis tentativas.

A cada tentativa, o jogo indica quais letras estão nas posições corretas e quais aparecem na palavra, mas estão em posições diferentes.

- **Verde:** letra na posição correta.
- **Amarelo:** letra presente na palavra, mas em outra posição.
- **Cinza:** letra que não foi identificada como correta na tentativa.

O jogador vence quando acerta a palavra secreta. Caso não consiga acertar em seis tentativas, o jogo termina e revela a resposta.

## Tecnologias utilizadas

- **C:** desenvolvimento da lógica e das estruturas do jogo.
- **Raylib:** criação da interface gráfica e leitura das teclas.
- **libcurl:** realização de requisições HTTP para obtenção de palavras aleatórias.
- **Random Word API:** fornecimento de palavras em português brasileiro.
- **GCC:** compilação do código.
- **GNU Make:** automatização da compilação.
- **pkg-config:** identificação das bibliotecas e suas dependências.

## Funcionalidades

- Obtenção de uma palavra secreta aleatória por meio de uma API externa.
- Seleção de palavras em português brasileiro com exatamente cinco letras.
- Conversão de caracteres acentuados para letras sem acento.
- Entrada de palavras pelo teclado.
- Avaliação das letras de cada tentativa.
- Exibição das tentativas em uma grade.
- Indicação visual das letras por cores.
- Reinício da partida após o término, com uma nova consulta à API.
- Registro dos resultados em arquivo.

## Integração com a API

O projeto utiliza a [Random Word API](https://random-word-api.herokuapp.com/home) para obter palavras aleatórias em português brasileiro.

A requisição utiliza os seguintes parâmetros:

- `number=1`: solicita uma palavra.
- `length=5`: solicita palavras com cinco letras.
- `lang=pt-br`: define o idioma como português brasileiro.

Endpoint utilizado:

```text
https://random-word-api.herokuapp.com/word?number=1&length=5&lang=pt-br
```

A comunicação com a API é realizada por meio da biblioteca libcurl, responsável pelas requisições HTTP e pelo recebimento das respostas.

As palavras recebidas passam por um processo de conversão para letras maiúsculas e remoção de acentos, mantendo a compatibilidade com a lógica de comparação do jogo.

A consulta acontece no início de cada partida e sempre que uma nova partida é iniciada.

## Organização dos arquivos

| Arquivo | Descrição |
|---|---|
| `main.c` | Inicialização da janela, leitura do teclado, controle da partida e desenho da interface. |
| `jogo.h` | Definição da estrutura da partida e declaração das funções. |
| `jogo.c` | Regras do jogo e verificação das tentativas. |
| `palavras.h` | Declaração das funções de integração com a API. |
| `palavras.c` | Comunicação com a API, recebimento da resposta, conversão e preparação da palavra secreta. |
| `ranking.h` | Definição das estruturas dos resultados. |
| `ranking.c` | Gerenciamento da lista encadeada e gravação dos resultados. |
| `ranking.txt` | Arquivo que armazena os resultados das partidas. |
| `Makefile` | Comandos para compilar, executar e limpar o projeto. |

## Conceitos de programação utilizados

O projeto utiliza os seguintes requisitos exigidos pela disciplina:

- **Structs:** organização dos dados de uma partida, dos resultados e da resposta HTTP.
- **Ponteiros:** acesso e alteração das estruturas, além do gerenciamento da lista encadeada e do buffer de resposta.
- **Alocação dinâmica de memória:** utilização de `malloc()` para criação dos nós do ranking e `realloc()` para armazenamento da resposta recebida pela API.
- **Listas encadeadas:** organização dos resultados das partidas.
- **Matrizes:** armazenamento das tentativas e das cores correspondentes a cada letra.
- **Leitura e escrita em arquivos:** persistência dos resultados das partidas no arquivo `ranking.txt`.

## Requisitos para executar

Para compilar e executar o jogo, é necessário ter:

- GCC compatível com C11.
- Biblioteca Raylib instalada.
- Biblioteca libcurl instalada.
- GNU Make.
- pkg-config.
- Conexão com a internet para consultar a API.

No Windows, recomenda-se utilizar o MSYS2 UCRT64 para disponibilizar as dependências de compilação.

### Instalação do libcurl no MSYS2

Caso ainda não tenha a biblioteca instalada, execute no terminal MSYS2 UCRT64:

```bash
pacman -S mingw-w64-ucrt-x86_64-curl
```

## Como executar no Windows

O projeto pode ser compilado pelo terminal PowerShell do VS Code utilizando as ferramentas instaladas pelo MSYS2.

1. Abra a pasta do projeto no VS Code.
2. Abra o terminal PowerShell.
3. Configure temporariamente as variáveis de ambiente do MSYS2:

```powershell
$env:MSYSTEM = "UCRT64"
$env:CHERE_INVOKING = "1"
$env:PATH = "C:\msys64\ucrt64\bin;C:\msys64\usr\bin;$env:PATH"
```

4. Confira se as bibliotecas estão disponíveis:

```powershell
pkg-config --modversion raylib libcurl
```

5. Compile o projeto:

```powershell
make
```

6. Execute o jogo:

```powershell
./termo.exe
```

Também é possível compilar e executar diretamente com:

```powershell
make run
```

Para remover o executável gerado:

```powershell
make clean
```

## Controles

- **Letras:** digitar a palavra.
- **Backspace:** apagar a última letra.
- **Enter:** enviar a tentativa.
- **R:** iniciar uma nova partida após o término.

## Observações

O projeto utiliza uma API externa para obter as palavras secretas, não dependendo de um arquivo local para manter uma lista de palavras.

É necessária uma conexão com a internet para iniciar uma partida e obter novas palavras. Caso a API esteja indisponível, não será possível iniciar uma nova partida até que a comunicação seja restabelecida.

## Autores

Projeto desenvolvido por Felipe Assis (fafs@cesar.school), Gabriel Cassemiro (gcrfl@cesar.school) e José Artur Viana (jafv@cesar.school) para a disciplina de Programação Imperativa e Funcional.
