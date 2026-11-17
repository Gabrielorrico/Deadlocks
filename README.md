# Algoritmo do Banqueiro (Banker's Algorithm)

Implementação em **C** do Algoritmo do Banqueiro, utilizado para
controlar a alocação de recursos entre processos e evitar que o sistema
entre em um estado inseguro.

O programa recebe a quantidade total de instâncias de cada tipo de
recurso pela linha de comando, lê as demandas máximas dos processos em
`customer.txt` e executa as solicitações e liberações descritas em
`commands.txt`.

## Estrutura do projeto

``` text
Deadlocks/
├── src/
│   └── banker.c
├── banker              # Executável gerado pelo make
├── commands.txt        # Comandos de solicitação e liberação
├── customer.txt        # Demanda máxima de cada processo
├── evidencias.log      # Registro de evidências/testes
├── Makefile
├── README.md
└── result.txt          # Resultado da execução
```

## Requisitos

-   GCC
-   Make
-   Ambiente Linux, Unix ou macOS (ou um ambiente compatível, como WSL)

## Compilação

Na pasta raiz do projeto, execute:

``` bash
make
```

O `Makefile` compila `src/banker.c` e gera o executável `banker` na
pasta raiz.

Para remover o executável gerado:

``` bash
make clean
```

## Como executar

Informe, como argumentos, a quantidade total de instâncias de cada tipo
de recurso, na mesma ordem utilizada nos arquivos de entrada.

Exemplo:

``` bash
./banker 10 5 7
```

Nesse exemplo, o sistema possui três tipos de recursos, com 10, 5 e 7
instâncias, respectivamente.

## Arquivos de entrada

### `customer.txt`

Cada linha representa um processo (cliente), e cada valor representa a
demanda máxima daquele processo para um tipo de recurso.

Conteúdo utilizado neste projeto:

``` text
7,5,3
3,2,2
9,0,2
2,2,2
4,3,3
```

Assim, o arquivo define cinco processos e três tipos de recursos. Por
exemplo, a primeira linha indica que o processo 0 pode solicitar, no
máximo, 7 instâncias do recurso 0, 5 do recurso 1 e 3 do recurso 2.

### `commands.txt`

O arquivo contém os comandos que serão executados pelo programa, um por
linha.

Conteúdo utilizado neste projeto:

``` text
RQ 0 0 2 1
RQ 2 3 0 2
RQ 1 2 0 0
RQ 4 0 0 2
RL 0 0 1 1
RQ 3 2 1 1
*
RQ 4 4 0 0
RQ 1 1 0 2
RQ 0 0 2 0
RQ 2 0 1 0
```

Comandos disponíveis:

  -----------------------------------------------------------------------
  Comando                             Descrição
  ----------------------------------- -----------------------------------
  `RQ`                                Solicita recursos para um processo.

  `RL`                                Libera recursos que estão alocados
                                      a um processo.

  `*`                                 Exibe as tabelas de máximo,
                                      alocação e necessidade, além dos
                                      recursos disponíveis.
  -----------------------------------------------------------------------

Formato dos comandos:

``` text
RQ <processo> <recurso 0> <recurso 1> ... <recurso N>
RL <processo> <recurso 0> <recurso 1> ... <recurso N>
```

O número de valores de recursos deve corresponder à quantidade de tipos
de recursos informada na execução do programa.

## Resultado

As mensagens produzidas durante a execução são gravadas em `result.txt`.
O comando `*` registra o estado das tabelas:

-   **MAXIMUM:** demanda máxima de cada processo;
-   **ALLOCATION:** recursos atualmente alocados a cada processo;
-   **NEED:** recursos que ainda podem ser necessários para cada
    processo;
-   **AVAILABLE:** quantidade de recursos que permanece disponível no
    sistema.

Uma solicitação só é concedida quando respeita os recursos disponíveis,
não ultrapassa a demanda máxima do processo e mantém o sistema em estado
seguro. Caso contrário, a solicitação é negada.

## Observações

-   Execute o programa a partir da pasta raiz do projeto, onde estão
    `customer.txt` e `commands.txt`.
-   Se alterar apenas `customer.txt` ou `commands.txt`, não é necessário
    executar `make novamente`; basta executar o programa outra vez.
-   Se alterar `src/banker.c`, execute `make` para recompilar.
