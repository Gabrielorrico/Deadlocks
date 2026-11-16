#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


bool algoritmoProtecao(int numeroProcessos, int numeroRecursos, int **allocation, int **need, int *available){
    int *work = malloc(numeroRecursos * sizeof(int));
    
    for(int i = 0; i < numeroRecursos; i++){
        work[i] = available[i];
    }
    
    bool *finish = malloc(numeroProcessos * sizeof(bool));
    
    for(int i = 0; i < numeroProcessos; i++){
        finish[i] = false;
    }
    
    // algoritmo de protecao;
    bool progresso = true;
    while(progresso == true){
        progresso = false;
        
        bool liberado = false;
        for(int i = 0; i < numeroProcessos; i++){
            liberado = false;
            
            if(finish[i] == false){
                for(int j = 0; j < numeroRecursos; j++){
                    if(need[i][j] <= work[j]){
                        liberado = true;
                    }else{
                        liberado = false;
                        break;
                    }
                }
                
                if(liberado == true){
                    
                    for(int k = 0; k < numeroRecursos; k++){
                        work[k] += allocation[i][k];
                    }
                    
                    finish[i] = true;
                    progresso = true;
                }
                
            }else{
                continue;
            }
        }
    }
    
    bool estadoSeguro = true;
    
    for(int i = 0; i < numeroProcessos; i++){
        if(finish[i] == false){
            estadoSeguro = false;
            break;
        }
    }
    
    free(work);
    free(finish);
    
    return estadoSeguro;
}

void lerComandos(int numeroProcessos, int numeroRecursos, int **allocation, int **need, int *available){
    
    FILE *arquivo = fopen("commands.txt","r");
    if(arquivo == NULL){
        fprintf(stderr,"ERRO: erro ao abrir o commands.txt");
        exit(1);
    }

    char linha[256];
    char *tok;

    while(fgets(linha, sizeof(linha), arquivo) != NULL){
        tok = strtok(linha, " \n");

        if(strcmp(tok, "RQ") == 0){
            int processo; 

            tok = strtok(NULL, " \n");
            processo = strtol(tok, NULL, 10);

            int *request = malloc(numeroRecursos * sizeof(int));

            for(int i = 0; i < numeroRecursos; i++){
                tok = strtok(NULL, " \n");
                request[i] = strtol(tok, NULL, 10);
            }

            bool requestValido = false;
            for(int i = 0; i < numeroRecursos; i++){
                if(request[i] <= need[processo][i]){
                    requestValido = true;
                }else{
                    requestValido = false;
                    break;
                }
            }

            if(requestValido == true){
                for(int i = 0; i < numeroRecursos; i++){
                    if(request[i] <= available[i]){
                        requestValido = true;
                    }else{
                        requestValido = false;
                        break;
                    }
                }
            }

            if(requestValido == true){
                for(int i = 0; i < numeroRecursos; i++){
                    available[i] -= request[i];
                    allocation[processo][i] += request[i];
                    need[processo][i] -= request[i];
                }

                bool estadoSeguro = algoritmoProtecao(numeroProcessos, numeroRecursos, allocation, need, available);
                
                if(estadoSeguro == false){
                    for(int i = 0; i < numeroRecursos; i++){
                        available[i] += request[i];
                        allocation[processo][i] -= request[i];
                        need[processo][i] += request[i];
                    }
                }
            }

            free(request);

        }else if(strcmp(tok, "RL") == 0){
            int processo; 

            tok = strtok(NULL, " \n");
            processo = strtol(tok, NULL, 10);

            int *release = malloc(numeroRecursos * sizeof(int));

            for(int i = 0; i < numeroRecursos; i++){
                tok = strtok(NULL, " \n");
                release[i] = strtol(tok, NULL, 10);
            }

            bool releaseValido = true;
            for(int i = 0; i < numeroRecursos; i++){
                if(release[i] <= allocation[processo][i]){
                    releaseValido = true;
                }else{
                    releaseValido = false;
                    break;
                }
            }

            if(releaseValido == true){
                for(int i = 0; i < numeroRecursos; i++){
                    available[i] += release[i];
                    allocation[processo][i] -= release[i];
                    need[processo][i] += release[i];
                }
            }

            free(release);
        }else if(strcmp(tok, "*") == 0){
            break;

        }
    }


}

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

    for(int i = 0; i < numeroRecursos; i++){
        recursosTotais[i] = strtol(argv[i + 1], NULL, 10);
        available[i] = strtol(argv[i + 1], NULL, 10);

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

    //algoritmo de protecao;
    bool estadoSeguro = algoritmoProtecao(numeroProcessos, numeroRecursos, allocation, need, available);
    //fim do algoritmo do protecao;

    return 0;
}