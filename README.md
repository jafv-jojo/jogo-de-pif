# Termo

Projeto final da disciplina de Programação Imperativa e Funcional, desenvolvido em linguagem C com a biblioteca Raylib.

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
- **GCC:** compilação do código.
- **Makefile:** automatização da compilação.

## Funcionalidades

- Sorteio de uma palavra secreta a partir de um arquivo.
- Entrada de palavras pelo teclado.
- Avaliação das letras de cada tentativa.
- Exibição das tentativas em uma grade.
- Indicação visual das letras por cores.
- Reinício da partida após o término.
- Registro dos resultados em arquivo.

## Organização dos arquivos

| Arquivo | Descrição |
|---|---|
| `main.c` | Inicialização da janela, leitura do teclado e desenho da interface. |
| `jogo.h` | Definição da estrutura da partida e declaração das funções. |
| `jogo.c` | Regras do jogo e verificação das tentativas. |
| `palavras.h` | Definição da estrutura do banco de palavras. |
| `palavras.c` | Leitura das palavras, alocação dinâmica e sorteio da palavra secreta. |
| `ranking.h` | Definição da estrutura dos resultados. |
| `ranking.c` | Gerenciamento da lista encadeada e gravação dos resultados. |
| `palavras.txt` | Lista de palavras utilizadas nas partidas. |
| `ranking.txt` | Arquivo que armazena os resultados das partidas. |
| `Makefile` | Comandos para compilar o projeto. |

## Conceitos de programação utilizados

O projeto utiliza os seguintes conceitos exigidos pela disciplina:

- **Structs:** organização dos dados de uma partida e dos resultados.
- **Ponteiros:** acesso e alteração das estruturas, além do gerenciamento da lista encadeada.
- **Alocação dinâmica de memória:** armazenamento do banco de palavras e criação dos nós do ranking.
- **Listas encadeadas:** organização dos resultados das partidas.
- **Matrizes:** armazenamento das tentativas e das cores correspondentes a cada letra.
- **Leitura e escrita em arquivos:** carregamento do banco de palavras e persistência dos resultados.

## Requisitos para executar

Para compilar e executar o jogo, é necessário ter:

- GCC compatível com C11.
- Biblioteca Raylib instalada.
- GNU Make.
- MSYS2 UCRT64 configurado no Windows, ou outro ambiente compatível com a instalação da Raylib.

## Como executar no Windows

1. Instale o MSYS2 e configure o ambiente UCRT64.
2. Instale o GCC, o Make, a Raylib e o `pkg-config`.
3. Abra o terminal MSYS2 UCRT64.
4. Acesse a pasta do projeto

5. Compile o projeto:

   ```bash
   make
   ```

6. Execute o jogo:

   ```bash
   ./termo.exe
   ```

## Controles

- **Letras:** digitar a palavra.
- **Backspace:** apagar a última letra.
- **Enter:** enviar a tentativa.
- **R:** iniciar uma nova partida após o término.

## Observações

O banco de palavras por enquanto utiliza palavras de cinco letras sem acentos nesta versão inicial. O arquivo `palavras.txt` pode ser editado para adicionar novas palavras, mantendo o formato de uma palavra por linha.

O arquivo `ranking.txt` armazena os resultados das partidas, permitindo que sejam recuperados em execuções posteriores.

# Autores

Projeto desenvolvido por **Felipe Assis (fafs@cesar.school), Gabriel Cassemiro (gcrfl@cesar.school) e José Artur Viana (jafv@cesar.school)** para a disciplina de Programação Imperativa e Funcional.
