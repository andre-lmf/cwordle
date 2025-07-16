#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

/**
 * Função que lê uma palavra aleatória de um arquivo e a armazena na variável 'linha'
 * @param linha Ponteiro para a string onde a palavra será armazenada
 * @return void
 */
void word(char *linha){
    srand(time(NULL));
    FILE *arquivo = fopen("./word_list/wordle-list", "r");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        return;
    }
    int line_count = 0;
    char temp_line[7];
    while (fgets(temp_line, sizeof temp_line, arquivo) != NULL) {
        line_count++;
    }
    if (line_count == 0) {
        printf("Arquivo vazio.\n");
        fclose(arquivo);
        return;
    }
    // printf("%d\n", line_count);
    fseek(arquivo, 0, SEEK_SET);

    

    int random_line_number = rand() % line_count+1;

    char line[7];
    for (int i = 0; i < random_line_number; i++) {
        if (fgets(line, 7, arquivo) == NULL) {
            printf("Erro ao ler a linha %d do arquivo.\n", i + 1);
            break;
        }
    }
    fclose(arquivo);
    // printf("%s\n", line);
    strncpy(linha, line, sizeof line -1);
    linha[strcspn(linha, "\n")] = '\0';
    // strncpy(linha, line, sizeof line-1);
}

/**
 * Função que procura uma palavra no arquivo word_list/wordle-list
 * @param entrada A palavra a ser procurada
 * @return true se a palavra for encontrada, false caso contrário
 */
bool procurar_palavra(const char *entrada){
    FILE *arquivo = fopen("./word_list/wordle-list", "r");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        return false;
    }

    char linha[7];
    while (fgets(linha, sizeof linha, arquivo) != NULL) {
        linha[strcspn(linha, "\n")] = '\0';
        if (strcmp(entrada, linha) == 0) {
            fclose(arquivo);
            return true;
        }
    }
    fclose(arquivo);
    return false;
}


/**
 * Função que remove caracteres iguais de duas strings e armazena o resultado em uma terceira string
 * @param str1 Primeira string
 * @param str2 Segunda string
 * @param resultado String onde o resultado será armazenado
 * @return void
 */
void remover_caracteres_iguais(char *str1, char*str2, char*resultado) {
    int k = 0;
    for (int i = 0; str2[i]!='\0'; i++)
    {
        bool found = false;
        for (int j = 0; str1[j] != '\0'; j++)
        {
            if (str2[i]==str1[j]){
                found = true;
                break;
            }
        }
        
        if (!found)
        {
            resultado[k++] = str2[i];
        }
    }
    resultado[k] = '\0';
}
/**
 * Função de comparação para qsort, usada para ordenar caracteres
 * @param a Ponteiro para o primeiro elemento a ser comparado
 * @param b Ponteiro para o segundo elemento a ser comparado
 * @return Um valor negativo se a < b, zero se a == b, ou um valor positivo se a > b
 */
int comp(const void *a, const void *b) {
    return (*(char *)a - *(char *)b);  
}
