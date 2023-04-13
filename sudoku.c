#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <math.h>

int **matrizD;
int matriz_tam;

int validador_linha = 1;
int validador_coluna = 1;
int validador_sub = 1;


typedef struct {
    int linha_incio;
    int linha_fim;
    int sub_colunas;
    int sub_linhas;
} thread_arg_t;

void *validar_linhas(void *arg) {
    thread_arg_t *thread_arg = (thread_arg_t *) arg;
    int *seen = (int *) calloc(matriz_tam, sizeof(int));
    for (int i = thread_arg->linha_incio; i < thread_arg->linha_fim; i++) {
        for (int j = 0; j < matriz_tam; j++) {
            int numero = matrizD[i][j];
            if (numero > 0 && numero <= matriz_tam) {
                if (seen[numero - 1]) {
                    //printf("O sudoku é inválido: numero repetido %d na linha %d\n", numero, i + 1);
                    free(seen);
                    validador_linha = 0;
                    return NULL;
                } else {
                    seen[numero - 1] = 1;
                }
            } else {
                //printf("O sudoku é inválido: numero invalido %d na linha %d\n", numero, i + 1);
                printf("File out of format");
                free(seen);
                return NULL;
            }
        }
        for (int j = 0; j < matriz_tam; j++) {
            seen[j] = 0;
        }
    }
    free(seen);
     // confirma q está tudo bem e pode printar SUCESS
    return NULL;
}

void *validar_colunas(void *arg) {
    thread_arg_t *thread_arg = (thread_arg_t *) arg;
    int *seen = (int *) calloc(matriz_tam, sizeof(int));
    for (int j = thread_arg->linha_incio; j < thread_arg->linha_fim; j++) {
        for (int i = 0; i < matriz_tam; i++) {
            int numero = matrizD[i][j];
            if (numero > 0 && numero <= matriz_tam) {
                if (seen[numero - 1]) {
                    //printf("O sudoku é inválido: numero repetido %d in column %d\n", numero, j + 1);
                    free(seen);
                    validador_coluna = 0;
                    return NULL;
                } else {
                    seen[numero - 1] = 1;
                }
            } else {
                //printf("O sudoku é inválido: numero invalido %d in column %d\n", numero, j + 1);
                printf("File out of format");
                free(seen);
                return NULL;
            }
        }
        for (int i = 0; i < matriz_tam; i++) {
            seen[i] = 0;
        }
    }
    free(seen);
 // mais uma validação para poder printar SUCESS
    return NULL;
}

thread_arg_t subLinha;

int checar_submatriz(int row, int col, int sub_linhas, int sub_colunas) {
    int seen[matriz_tam];
    memset(seen, 0, sizeof(seen));

    for (int i = row; i < row + sub_linhas; i++) {
        for (int j = col; j < col + sub_colunas; j++) {
            int numero = matrizD[i][j];
            if (numero > 0 && numero <= matriz_tam) {
                if (seen[numero - 1]) {
                    return 0;
                } else {
                    seen[numero - 1] = 1;
                }
            } else {
                printf("File out of format");
                return 0;
            }
        }
    }

    return 1;
}

void *validar_submatriz(void *arg) {
    thread_arg_t *thread_arg = (thread_arg_t *)arg;
    int sub_linhas = subLinha.sub_linhas;
    int sub_colunas = subLinha.sub_colunas;

    for (int row = thread_arg->linha_incio; row < thread_arg->linha_fim; row += sub_linhas) {
        for (int col = 0; col < matriz_tam; col += sub_colunas) {
            int seen[matriz_tam];
            memset(seen, 0, sizeof(seen));

            for (int i = row; i < row + sub_linhas; i++) {
                for (int j = col; j < col + sub_colunas; j++) {
                    int numero = matrizD[i][j];
                    if (numero > 0 && numero <= matriz_tam) {
                        if (seen[numero - 1]) {
                            validador_sub = 0;
                            return NULL;
                        } else {
                            seen[numero - 1] = 1;
                        }
                    } else {
                        printf("File out of format");
                        validador_sub = 0;
                        return NULL;
                    }
                }
            }
        }
    }

    return NULL;
}


int file_len(char *filename);

int check_last_char(const char *filename);

int main(int argc, char *argv[]){

    FILE *pont_arq;

    const char *input_file = argv[1];

    int last_char = check_last_char(input_file);
    if (last_char == ' ' || last_char == '\n') {
        printf("File out of format\n");
        return 1;
    }

    if (argc != 2){
        printf("Invalid number of parameters\n");
        return 1;
    }

    int val,val2,valsub1,valsub2;
    
    char linha[20];
    char linha2[20];
    char aqv_matriz[2000];

    pont_arq = fopen(argv[1], "r");

    if(pont_arq == NULL){
        printf("Error to read file\n");
        return 1 ;
    }
    
    fgets(linha, 20, pont_arq);
    //printf("%s", linha);

    fgets(linha2, 20, pont_arq);
    //printf("%s", linha2);
    char *aux;
    aux = strchr(linha, 'x');
    if (aux == NULL) {
        printf("File out of format");
        return 1;
    }
    *aux ='\0'; //transformando o x em null pra dps converter a string antes e dps do 'x' pra inteiro
    val = atoi(linha);
    val2 = atoi(aux+1);
    if(val != val2){
        printf("File out of format");
        return 1;
    }

    aux = strchr(linha2, 'x');
    if(aux == NULL){
        printf("File out of format");
        return 1;
    }
    *aux ='\0'; // a msm coisa aqui só q para a submatriz
    valsub1 = atoi(linha2);
    valsub2 = atoi(aux+1);
    if(valsub1*valsub2 != val){
       printf("File out of format");
        return 1;
    }

        if(valsub1*valsub2 != val2){
            printf("File out of format");
            return 1;
        }
    
    if(val < 9 || val2 < 9){
        printf("File out of format");
        return 1;
    }


// sem esse seek set os valores da minha matriz estavam virando lixo de memoria entao ele meio q resetou o ponteiro e leu denovo o arquivo do começo da matriz
    fseek(pont_arq, 0, SEEK_SET);
    fgets(linha, 20, pont_arq); 
    fgets(linha2, 20, pont_arq); 

   //convertendo a matriz para inteiro COM STRTOK
   int matriz[val][val2];
   int i = 0, j = 0, contador = 0;
   int qnt_linhas_arq = 0;
   char *valores = NULL;

    qnt_linhas_arq = file_len(argv[1]);

    //CONFERE TAMANHO DAS LINHAS
    if(qnt_linhas_arq - 2 != val2){
        printf("File out of format");
        return 1;
    }


 // Alocando a matriz
matriz_tam = val2;

 matrizD = (int **)malloc(matriz_tam * sizeof(int *));

  for (int i = 0; i < matriz_tam; i++) {
    matrizD[i] = (int *)malloc(matriz_tam * sizeof(int));
    for (int j = 0; j < matriz_tam; j++) {
      fscanf(pont_arq, "%d", &matrizD[i][j]);
    }
  }


//printf("matriz Dinamica convertida:\n");
    for(int i = 0; i < val; i++){
        for(int j = 0; j < val2; j++){
            if (matrizD[i][j] <= 0 || matrizD[i][j] > val) {
      
                printf("File out of format");//numero menor ou igual a 0 ou numero maior q val ou val2 na matriz
                return 1;
            }
        }
    }


   // Validar o sudoku utilizando pelo menos 12 threads

subLinha.sub_linhas = valsub1; // atribuindo o valor das sublinhas na struct
subLinha.sub_colunas = valsub2;
int thread_index = 0;
int total_threads = valsub1 + valsub2 + (valsub1 * valsub2);
pthread_t threads[total_threads];

// Criar threads para validar linhas
for (int i = 0; i < valsub1; i++) {
    thread_arg_t *thread_arg = (thread_arg_t *) malloc(sizeof(thread_arg_t));
    thread_arg->linha_incio = i * valsub2;
    thread_arg->linha_fim = (i + 1) * valsub2;
    pthread_create(&threads[thread_index], NULL, validar_linhas, thread_arg);
    thread_index++;
}

// Criar threads para validar colunas
for (int i = 0; i < valsub2; i++) {
    thread_arg_t *thread_arg = (thread_arg_t *) malloc(sizeof(thread_arg_t));
    thread_arg->linha_incio = i * valsub1;
    thread_arg->linha_fim = (i + 1) * valsub1;
    pthread_create(&threads[thread_index], NULL, validar_colunas, thread_arg);
    thread_index++;
}

// Criar threads para validar submatrizes
for (int i = 0; i < valsub1; i++) {
    for (int j = 0; j < valsub2; j++) {
        thread_arg_t *thread_arg = (thread_arg_t *) malloc(sizeof(thread_arg_t));
            thread_arg->linha_incio = i * valsub1;
            thread_arg->linha_fim = (i + 1) * valsub1;
            thread_arg->sub_colunas = j * valsub2;
            thread_arg->sub_linhas = (j + 1) * valsub2;
            pthread_create(&threads[thread_index], NULL, validar_submatriz, thread_arg);
            thread_index++;
    }

}
// Wait for the threads to finish
for (int i = 0; i < total_threads; i++) {
    pthread_join(threads[i], NULL);
} 


// Free 
for (int i = 0; i < matriz_tam; i++) {
    free(matrizD[i]);
}
free(matrizD);

   


  fclose(pont_arq);

FILE * resultado;
    resultado = fopen("sudoku_albgm.out","w");
        if(resultado == NULL){
            printf("Error to read file\n");
            return 1;
        }

if (validador_linha && validador_coluna  && validador_sub ){
    fprintf(resultado,"SUCCESS");
    //printf("sucesso");
} else {
    fprintf(resultado,"FAIL");
    //printf("falha\n");
    
}

fclose(resultado);
  return 0;
  } 


int file_len(char *filename)
{
    char* readline = NULL;
    int cont = 0;
    size_t len = 0;

    FILE *fp = fopen (filename, "r");

    while ((getline(&readline, &len, fp)) != -1) {
        
        char *separator = strtok(readline, "\n");
        
        cont++;
    }
    
    fclose(fp);

    return cont;
} 

int check_last_char(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        //printf("Error to read file3");
        return 1;
    }

    fseek(file, -1, SEEK_END); // Vá para o penúltimo caractere do arquivo
    int last_char = fgetc(file); // Leia o último caractere
    fclose(file);

    return last_char;
}