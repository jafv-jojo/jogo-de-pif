# Cermo

Projeto final da disciplina de Programação Imperativa e Funcional, desenvolvido em linguagem C utilizando a biblioteca Raylib. O jogo é inspirado no Termo.

## Sobre o jogo

Cermo é um jogo de adivinhação de palavras. O objetivo é descobrir uma palavra secreta de cinco letras em até seis tentativas.

A cada tentativa, o jogo indica quais letras estão nas posições corretas e quais aparecem na palavra, mas estão em posições diferentes.

- **Verde:** letra na posição correta.
- **Amarelo:** letra presente na palavra, mas em outra posição.
- **Cinza:** letra que não foi identificada como correta na tentativa.

O jogador vence quando acerta a palavra secreta. Caso não consiga acertar em seis tentativas, o jogo termina e revela a resposta.

Além do modo normal, o jogo possui os modos **Dueto** (duas palavras ao mesmo tempo, em até sete tentativas) e **Quarteto** (quatro palavras ao mesmo tempo, em até nove tentativas).

## Tecnologias utilizadas

- **C:** desenvolvimento da lógica e das estruturas do jogo.
- **Raylib:** criação da interface gráfica, animações, sons e leitura do teclado e do mouse.
- **GCC:** compilação do código.
- **GNU Make:** automatização da compilação.
- **pkg-config:** identificação das bibliotecas e suas dependências.

## Funcionalidades

- Sorteio da palavra secreta a partir de uma lista local de palavras comuns em português brasileiro.
- Validação das tentativas com a mesma lista de palavras.
- Conversão de caracteres acentuados para letras sem acento.
- Entrada de palavras pelo teclado físico ou pelo teclado na tela, clicando com o mouse.
- Teclado na tela com as cores das letras já testadas.
- Avaliação das letras de cada tentativa.
- Exibição das tentativas em uma grade.
- Indicação visual das letras por cores.
- Modos Normal, Dueto e Quarteto.
- Animação das letras sendo reveladas uma por uma após cada tentativa.
- Animação de tremor e som quando a palavra é inválida.
- Caixa de resultado que aparece deslizando ao fim da partida.
- Confetes caindo ao vencer, que colidem com o chão e se acumulam.
- Botões do menu com cantos arredondados, sombra e efeito ao passar o mouse.
- Título do menu com animação de flutuar.
- Opção de tela cheia, pelo botão do menu ou pela tecla F11, mantendo a proporção do jogo.
- Ícone da janela desenhado pelo próprio jogo.
- Reinício da partida após o término.
- Registro dos resultados em arquivo.
- Tela de estatísticas com partidas jogadas, vitórias, sequências, distribuição de tentativas e últimas partidas.

## Palavras do jogo

O jogo utiliza o arquivo `palavras_5_letras.txt`, com uma palavra comum de cinco letras por linha, sem acentos. A palavra secreta é sorteada desse arquivo, e uma tentativa só é aceita se estiver nele.

As palavras são carregadas na memória ao iniciar o jogo, utilizando alocação dinâmica, e convertidas para letras maiúsculas para manter a compatibilidade com a lógica de comparação do jogo.

## Organização dos arquivos

| Arquivo | Descrição |
|---|---|
| `main.c` | Inicialização da janela, leitura do teclado e do mouse, carregamento das palavras, controle das partidas, animações, tela cheia e desenho da interface. |
| `jogo.h` | Definição da estrutura da partida e declaração das funções. |
| `jogo.c` | Regras do jogo e verificação das tentativas. |
| `dueto.h` / `dueto.c` | Estrutura e regras do modo Dueto. |
| `quarteto.h` / `quarteto.c` | Estrutura e regras do modo Quarteto. |
| `ranking.h` | Definição das estruturas dos resultados. |
| `ranking.c` | Gerenciamento da lista encadeada e gravação dos resultados. |
| `palavras_5_letras.txt` | Lista de palavras do jogo. |
| `ranking.txt` | Arquivo que armazena os resultados das partidas. |
| `sons/sfx/` | Efeitos sonoros do jogo. |
| `Makefile` | Comandos para compilar, executar e limpar o projeto. |

## Conceitos de programação utilizados

O projeto utiliza os seguintes requisitos exigidos pela disciplina:

- **Structs:** organização dos dados de uma partida, dos resultados, das listas de palavras e dos confetes.
- **Ponteiros:** acesso e alteração das estruturas, além do gerenciamento da lista encadeada e das listas de palavras.
- **Alocação dinâmica de memória:** utilização de `malloc()` para criação dos nós do ranking e de cada palavra carregada, e `realloc()` para aumentar as listas de palavras durante a leitura dos arquivos.
- **Listas encadeadas:** organização dos resultados das partidas.
- **Matrizes:** armazenamento das tentativas e das cores correspondentes a cada letra.
- **Leitura e escrita em arquivos:** leitura da lista de palavras e persistência dos resultados das partidas no arquivo `ranking.txt`.
- **Animação, movimento e colisão:** revelação das letras, tremor da linha, caixa de resultado deslizando, confetes que caem e colidem com o chão, e cliques do mouse nos botões e no teclado da tela usando colisão entre ponto e retângulo.

## Requisitos para executar

Para compilar e executar o jogo, é necessário ter:

- GCC compatível com C11.
- Biblioteca Raylib instalada.
- GNU Make.
- pkg-config.

Os arquivos `palavras_5_letras.txt`, `ranking.txt` e a pasta `sons` devem estar na mesma pasta de onde o jogo é executado.

No Windows, recomenda-se utilizar o MSYS2 UCRT64 para disponibilizar as dependências de compilação.

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

4. Confira se a Raylib está disponível:

```powershell
pkg-config --modversion raylib
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

## Como executar no Linux

Os passos abaixo foram feitos para Ubuntu e Debian. Também funcionam no WSL (Ubuntu dentro do Windows); no Windows 11 a janela do jogo abre normalmente pelo WSLg.

1. Instale as ferramentas de compilação e as dependências da Raylib:

```bash
sudo apt update
sudo apt install build-essential git cmake pkg-config \
    libasound2-dev libx11-dev libxrandr-dev libxi-dev libxcursor-dev \
    libxinerama-dev libgl1-mesa-dev libwayland-dev libxkbcommon-dev
```

2. Baixe, compile e instale a Raylib:

```bash
git clone --depth 1 --branch 5.5 https://github.com/raysan5/raylib.git
cd raylib
cmake -B build -DBUILD_SHARED_LIBS=ON -DBUILD_EXAMPLES=OFF
cmake --build build
sudo cmake --install build
sudo ldconfig
cd ..
```

3. Confira se a Raylib está disponível:

```bash
pkg-config --modversion raylib
```

4. Entre na pasta do projeto e compile:

```bash
make
```

5. Execute o jogo:

```bash
./termo.exe
```

Também é possível compilar e executar diretamente com `make run`, e remover o executável com `make clean`.

## Controles

- **Letras:** digitar a palavra, pelo teclado ou clicando no teclado da tela.
- **Backspace / APAGAR:** apagar a última letra.
- **Enter / ENTER:** enviar a tentativa.
- **R:** iniciar uma nova partida após o término.
- **ESC:** voltar ao menu.
- **F11:** alternar tela cheia.
- **Mouse:** escolher o modo de jogo, abrir as estatísticas e usar os botões.

## Observações

O jogo não precisa de conexão com a internet. As palavras vêm do arquivo `palavras_5_letras.txt`, que pode ser editado para adicionar ou remover palavras.

## Autores

Projeto desenvolvido por Felipe Assis (fafs@cesar.school), Gabriel Cassemiro (gcrfl@cesar.school) e José Artur Viana (jafv@cesar.school) para a disciplina de Programação Imperativa e Funcional.