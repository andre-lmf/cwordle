#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define RED   "\033[31m"
#define GRN   "\033[32m"
#define YEL   "\033[33m"
#define BOLD  "\033[1m"
#define RESET "\033[0m"



/**
 * Função que lê uma palavra aleatória de um arquivo e a armazena na variável 'linha'
 * @param linha Ponteiro para a string onde a palavra será armazenada
 * @return void
 */
void gerar_palavra(char *linha){
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
bool procurar_palavra(char entrada[]){
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
void remover_caracteres_iguais(char str1[], char str2[], char resultado[]) {
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
int comparar(const void *a, const void *b) {
    return (*(char *)a - *(char *)b);  
}

/**
 * Função que guarda as letras corretas em um array
 * @param entrada A palavra de entrada do usuário
 * @param palavra A palavra que o usuário está tentando adivinhar
 * @param cores Array de cores correspondentes às letras
 * @param palavra_var Cópia da palavra que o usuário está tentando adivinhar. _var significa que pode ser modificada sem afetar a original
 * @param index_i Ponteiro para o índice atual do array de letras corretas
 * @param letras_g Array onde as letras corretas serão armazenadas
 * @return void
 */
void guardar_letras_corretas(char entrada[],char palavra[], char cores[], char palavra_var[], int* index_i, char letras_g[]){
    int index = *index_i;
    for (int i = 0; i < 5; i++)
    {
        if (entrada[i] == palavra[i])
        {
            cores[i] = 'G';
            char *pos = strchr(palavra_var, entrada[i]);
            if (pos)
            {
                *pos = ' ';
                if (!strchr(letras_g, entrada[i]))
                {
                    letras_g[index++] = entrada[i];
                    *index_i = index;
                }
            }
        }
    }
    letras_g[strcspn(letras_g, "\n")] = '\0';
}


/**
 * Função que guarda as letras parcialmente corretas em um array
 * @param cores Array de cores correspondentes às letras
 * @param palavra_var Cópia da palavra que o usuário está tentando adivinhar. _var significa que pode ser modificada sem afetar a original
 * @param entrada A palavra de entrada do usuário
 * @param letras_g Array de letras corretas
 * @param letras_y Array de letras parcialmente corretas
 * @param index_j Ponteiro para o índice atual do array de letras parcialmente corretas
 * @return void
 */
void guardar_letras_meio_corretas(char cores[], char palavra_var[], char entrada[], char letras_g[], char letras_y[], int *index_j){
    int index = *index_j;
     for (int i = 0; i < 5; i++)
        {
            if (cores[i] != 'G' && strchr(palavra_var, entrada[i]))
            {
                cores[i] = 'Y';
                char *pos = strchr(palavra_var, entrada[i]);
                if (pos)
                {
                    *pos = ' ';
                    if (!strchr(letras_y, entrada[i]) && !strchr(letras_g, entrada[i]))
                    {
                        letras_y[index++] = entrada[i];
                        *index_j = index;
                    }
                }
            }
        }
}

/**
 * Função que guarda as letras erradas em um array
 * @param letras_e Array onde as letras erradas serão armazenadas
 * @param entrada A palavra de entrada do usuário
 * @param letras_y Array de letras parcialmente corretas    
 * @param letras_g Array de letras corretas
 * @param index_e Ponteiro para o índice atual do array de letras erradas
 * @return void
 */
void guardar_letras_erradas(char letras_e[], char entrada[],char letras_y[], char letras_g[], int*index_e){
    int index = *index_e;
    for (int i = 0; i < 5; i++)
        {   
            if (!strchr(letras_e, entrada[i]) && !strchr(letras_y, entrada[i]) && !strchr(letras_g, entrada[i]))
            {
                letras_e[index++] = entrada[i];
                *index_e = index;
            }
        }
}

/**
 * Função que preenche um array com todas as letras do alfabeto
 * @param letras Array onde as letras serão armazenadas
 * @return void  
 */
void preencher_alfabeto(char letras[]){
    int i = 0;
    for (char c = 'a'; c <='z'; c++)
    {
        letras[i] = c;
        i++;
    }
}

/**
 * Função que exibe as letras de um array
 * @param letras Array de letras a ser exibido
 * @return void
 */
void exibir_letras(char letras[]){
    for (int i = 0; i < strlen(letras); i++)
        {
            if (i != strlen(letras) - 1)
            {
                printf("%c, ", letras[i]);
            }
            else
            {
                printf("%c", letras[i]);
            }
    }
}

/**
 * Função que imprime o chute do usuário com as cores correspondentes
 * @param cores Array de cores correspondentes às letras
 * @param entrada A palavra de entrada do usuário
 * @return void
*/
void imprimir_chute(char cores[], char entrada[]){
    for (int i = 0; i < 5; i++)
        {
            if (cores[i]!='*')
            {
                if (cores[i]=='G')
                {
                    printf(BOLD);
                    printf(GRN);
                }
                if(cores[i]=='Y') {
                    printf(BOLD);
                    printf(YEL);
                }
                printf("%c", entrada[i]);
                printf(RESET);
            } else {
                printf(RED);
                printf("*");
                printf(RESET);
            }   
        }
}

