---
title: "BattleShip - Laboratório de Programação"
author:
- João Varelas
geometry: a4paper
geometry: margin=3cm
output: pdf_document
---


# Instruções

O código fonte encontra-se no diretório `src/` juntamente com o `Makefile`. 
Para facilitar a compilação, está incluido um script `.sh` com os comandos necessários.

Basta fazer `$ ./run_client.sh` para compilar e executar, equivalente a 
`$ cd src/ && make clean client && cd ../ && ./bin/client`.
Os binários executáveis do `client` e `server` encontram-se no diretório `bin/`.


# Testes

De modo a facilitar os testes, está incluido um `run_test.sh` para fornecer um input
pré-definido com o objetivo de efetuar ações automaticamente tais como selecionar
menus, enviar coordenadas, etc.

Por exemplo, o comando `$ ./run_test.sh tests/gen_rand_board.inp` executa o `client`
e fornece o input de `gen_rand_board.inp`.

Neste input em particular, são gerados 2 tabuleiros para cada jogador com barcos 
posicionados e direcionados aleatóriamente e inicia o jogo.
Todos os testes (inputs) estão dentro do diretório `tests/`.
A descrição de cada teste está disponível em `tests/INFO.txt`.


# Configuração do Jogo

É permitida a configuração prévia do jogo no menu `Settings`.
Pode ser alterado o tamanho do tabuleiro entre 20x20 e 40x40 e o número de barcos 
entre 5 e SIZE*SIZE/25, onde SIZE é o tamanho atual do lado do tabuleiro.

As configurações são guardadas de forma persistente no diretório `settings/`.
O formato do ficheiro é o seguinte:

- 1ª linha: Tamanho SIZE do lado do tabuleiro (SIZExSIZE)

- 2ª linha: Número N de barcos

- N linhas: Configuração do barco 5x5 em bitmap de forma contígua

Um exemplo do ficheiro `settings` é o seguinte:

- 20
- 5
- 0000000000001000000000000
- 0000000000011000000000000
- 0000000000011100000000000
- 0000000000111100000000000
- 0000001110001000010000000



Deve ser evitada a modificação direta do ficheiro. O próprio jogo permite alterar estas 
definições.

Existe um ficheiro `settings_default` que permite restaurar as definições originais.

Os barcos podem ser "desenhados" no menu `Settings`. Após definir o número N de barcos
são pedidas N configurações que consiste no posicionamento de pixeis numa matriz de
5x5.

É necessário que os pixeis colocados sejam adjacentes entre si sem contar com diagonais
(poliominó).



# Início do Jogo

O jogo dispõe, na maioria das vezes, menus numéricos em que é necessário introduzir um dígito
a partir de `1` para selecionar a opção pretendida.

No caso das coordenadas, elas devem ser enviadas na forma `(x y)` como por exemplo, `(4 2)`.
O `x` representa as linhas do tabuleiro e `y` as colunas.

Para iniciar um jogo deve ser selecionada a opção `1 - New game` e de seguida `1 - Play by turns`.

Depois é pedido um nickname do jogador e qual a estratégia pretendida: `1 - Random Strategy`
ou `2 - Custom Strategy`.

No caso da `1 - Random Strategy` os barcos são colocados no tabuleiro de forma aleatória e é 
apresentado o tabuleiro no seu estado final.

Na 2ª opção `2 - Custom Strategy`, o jogador tem a oportunidade de colocar os barcos 
um a um no tabuleiro, podendo movimentar de 4 formas (`Up`, `Down`, `Left`, `Right`) 
e ainda rodar a matriz do barco 90º no sentido dos ponteiros do relógio.

Quando o barco estiver na posição desejada, pode-se confimar com `6 - Done`.


Após os dois jogadores finalizarem a colocação de barcos nos seus tabuleiros, o jogo
inicia.

É escolhido um jogador aleatóriamente para iniciar o jogo (jogador 1 ou jogador 2)
e de imediato são pedidas coordenadas `(x y)` para fazer um disparo.

Cada célula do tabuleiro tem 3 estados possíveis: `UNKNOWN`, `HIT` e `MISSED`.

Quando um jogador acerta num pixel do barco do inimigo, repete a jogada, caso contrário,
passa a vez ao adversário.

Um jogador vence quando o seu adversário fica sem barcos disponíveis,
i.e., quando os pixeis de todos os barcos são atingidos.
