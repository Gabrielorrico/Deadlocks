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

void lerComandos(int numeroProcessos, int numeroRecursos, int **allocation, int **max, int **need, int *available){

    FILE *arquivo = fopen("commands.txt","r");
    if(arquivo == NULL){
        fprintf(stderr,"ERRO: erro ao abrir o commands.txt");
        exit(1);
    }

    FILE *resultado = fopen("result.txt", "w");
    if(resultado == NULL){
        fprintf(stderr,"ERRO: nao foi possivel abrir o arquivo result.txt");
        fclose(arquivo);
        exit(1);
    }

    char linha[256];
    char *tok;

    while(fgets(linha, sizeof(linha), arquivo) != NULL){
        tok = strtok(linha, " \r\n");

        if(tok == NULL){
            continue;
        }

        if(strcmp(tok, "RQ") == 0){
            int processo;

            tok = strtok(NULL, " \r\n");
            if(tok == NULL){
                fprintf(stderr, "ERRO: comando RQ invalido\n");
                exit(1);
            }

            processo = strtol(tok, NULL, 10);

            if(processo < 0 || processo >= numeroProcessos){
                fprintf(stderr, "ERRO: processo invalido\n");
                exit(1);
            }

            int *request = malloc(numeroRecursos * sizeof(int));

            for(int i = 0; i < numeroRecursos; i++){
                tok = strtok(NULL, " \r\n");

                if(tok == NULL){
                    fprintf(stderr, "ERRO: quantidade de recursos invalida no RQ\n");
                    exit(1);
                }

                request[i] = strtol(tok, NULL, 10);

                if(request[i] < 0){
                    fprintf(stderr, "ERRO: quantidade de recursos invalida no RQ\n");
                    exit(1);
                }
            }

            bool requestValido = true;
            bool excedeNeed = false;
            bool faltaAvailable = false;

            for(int i = 0; i < numeroRecursos; i++){
                if(request[i] > need[processo][i]){
                    requestValido = false;
                    excedeNeed = true;
                    break;
                }
            }

            if(requestValido == true){
                for(int i = 0; i < numeroRecursos; i++){
                    if(request[i] > available[i]){
                        requestValido = false;
                        faltaAvailable = true;
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

                    fprintf(resultado, "The customer %d request", processo);

                    for(int i = 0; i < numeroRecursos; i++){
                        fprintf(resultado, " %d", request[i]);
                    }

                    fprintf(resultado, " was denied because result in an unsafe state\n");

                }else{
                    fprintf(resultado, "Allocate to customer %d the resources", processo);

                    for(int i = 0; i < numeroRecursos; i++){
                        fprintf(resultado, " %d", request[i]);
                    }

                    fprintf(resultado, "\n");
                }

            }else if(excedeNeed == true){
                fprintf(resultado, "The customer %d request", processo);

                for(int i = 0; i < numeroRecursos; i++){
                    fprintf(resultado, " %d", request[i]);
                }

                fprintf(resultado, " was denied because exceed its maximum allocation\n");

            }else if(faltaAvailable == true){
                fprintf(resultado, "The resources");

                for(int i = 0; i < numeroRecursos; i++){
                    fprintf(resultado, " %d", available[i]);
                }

                fprintf(resultado, " are not enough to customer %d request", processo);

                for(int i = 0; i < numeroRecursos; i++){
                    fprintf(resultado, " %d", request[i]);
                }

                fprintf(resultado, "\n");
            }

            free(request);

        }else if(strcmp(tok, "RL") == 0){
            int processo;

            tok = strtok(NULL, " \r\n");
            if(tok == NULL){
                fprintf(stderr, "ERRO: comando RL invalido\n");
                exit(1);
            }

            processo = strtol(tok, NULL, 10);

            if(processo < 0 || processo >= numeroProcessos){
                fprintf(stderr, "ERRO: processo invalido\n");
                exit(1);
            }

            int *release = malloc(numeroRecursos * sizeof(int));

            for(int i = 0; i < numeroRecursos; i++){
                tok = strtok(NULL, " \r\n");

                if(tok == NULL){
                    fprintf(stderr, "ERRO: quantidade de recursos invalida no RL\n");
                    exit(1);
                }

                release[i] = strtol(tok, NULL, 10);

                if(release[i] < 0){
                    fprintf(stderr, "ERRO: quantidade de recursos invalida no RL\n");
                    exit(1);
                }
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

                fprintf(resultado, "Release from customer %d the resources", processo);

                for(int i = 0; i < numeroRecursos; i++){
                    fprintf(resultado, " %d", release[i]);
                }

                fprintf(resultado, "\n");
            }

            free(release);

        }else if(strcmp(tok, "*") == 0){
            fprintf(resultado, "MAXIMUM | ALLOCATION | NEED\n");

            for(int i = 0; i < numeroProcessos; i++){
                for(int j = 0; j < numeroRecursos; j++){
                    fprintf(resultado, "%d", max[i][j]);

                    if(j < numeroRecursos - 1){
                        fprintf(resultado, " ");
                    }
                }

                fprintf(resultado, " | ");

                for(int j = 0; j < numeroRecursos; j++){
                    fprintf(resultado, "%d", allocation[i][j]);

                    if(j < numeroRecursos - 1){
                        fprintf(resultado, " ");
                    }
                }

                fprintf(resultado, " | ");

                for(int j = 0; j < numeroRecursos; j++){
                    fprintf(resultado, "%d", need[i][j]);

                    if(j < numeroRecursos - 1){
                        fprintf(resultado, " ");
                    }
                }

                fprintf(resultado, "\n");
            }

            fprintf(resultado, "AVAILABLE");

            for(int i = 0; i < numeroRecursos; i++){
                fprintf(resultado, " %d", available[i]);
            }

            fprintf(resultado, "\n");

        }else{
            fprintf(stderr, "ERRO: comando invalido\n");
            exit(1);
        }
    }

    fclose(arquivo);
    fclose(resultado);

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


    lerComandos(numeroProcessos, numeroRecursos, allocation, max, need, available);

    return 0;
}