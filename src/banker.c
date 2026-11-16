#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int numeroProcessos = 0;
    int numeroRecursos;

    int *recursosTotais;
    numeroRecursos = argc - 1;
    recursosTotais = malloc(numeroRecursos * sizeof(int));

    int *available;
    available = malloc(numeroRecursos * sizeof(int));
    
    int **allocation;

    FILE *arquivo;
    arquivo = fopen("customer.txt", "r");
    if(arquivo == NULL){
        fprintf(stderr, "ERRO: nao foi possivel abrir o arquivo customer");
        return 1;

    }
    char linha[256];
    while(fgets(linha, sizeof(linha), arquivo) != NULL){
        numeroProcessos++;

    }

    rewind(arquivo);

    allocation = malloc(numeroProcessos * sizeof(int *));

    for(int i = 0; i < numeroProcessos; i++){
        allocation[i] = malloc(numeroRecursos * sizeof(int));

        for(int j = 0; j < numeroRecursos; j++){
            allocation[i][j] = 0;

        }
    }

    int **max;
    int **need;
    max = malloc(numeroProcessos * sizeof(int *));
    need = malloc(numeroProcessos * sizeof(int *));

    for (int i = 0; i < numeroProcessos; i++) {
        max[i] = malloc(numeroRecursos * sizeof(int));
        need[i] = malloc(numeroRecursos * sizeof(int));

    }

    bool *finish;

    for(int i = 0; i < numeroRecursos; i++){
        recursosTotais[i] = strtol(argv[i + 1], NULL, 10);
        available[i] = strtol(argv[i + 1], NULL, 10);
        
    }

    

    return 0;
}