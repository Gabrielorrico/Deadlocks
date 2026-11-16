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

    int *work;
    work = malloc(numeroRecursos * sizeof(int));
    
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

    fclose(arquivo);

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
    finish = malloc(numeroProcessos * sizeof(bool));
    for(int i = 0; i < numeroProcessos; i++){
        finish[i] = false;
    }

    for(int i = 0; i < numeroRecursos; i++){
        recursosTotais[i] = strtol(argv[i + 1], NULL, 10);
        available[i] = strtol(argv[i + 1], NULL, 10);
        work[i] = strtol(argv[i + 1], NULL, 10);

    }

    FILE *arq = fopen("customer.txt", "r");
    if(arq == NULL){
        fprintf(stderr, "ERRO: nao foi possivel abrir o arquivo customer");
        return 1;

    }

    int i = 0;
    char *tok;
    
    while (fgets(linha, sizeof(linha), arq) != NULL){
        tok = strtok(linha,",");

        for(int j = 0; j < numeroRecursos; j++){
            max[i][j] = strtol(tok,NULL,10);
            tok = strtok(NULL, ",");
        }
        i++;

    }

    fclose(arq);

    for(int i = 0; i < numeroProcessos; i++){
        for(int j = 0; j < numeroRecursos; j++){
            need[i][j] = max[i][j] - allocation[i][j];
        }
    }
    

    return 0;
}