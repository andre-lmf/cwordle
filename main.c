#include <stdio.h> // printf, fgets, FILE, fopen, fclose, perror
#include <stdbool.h> // bool
#include <string.h> // strncpy, strcspn, strlen, strcmp
#include <ctype.h> // tolower
#include <stdlib.h> // srand, rand, qsort
#include "alg.h"

#define RED   "\033[31m"
#define GRN   "\033[32m"
#define YEL   "\033[33m"
#define BOLD  "\033[1m"
#define RESET "\033[0m"

/**
 * Função principal do jogo CWordle
 * 
 * O jogador deve adivinhar uma palavra de 5 letras em até 6 tentativas
 * 
 * A cada tentativa, o jogador recebe feedback sobre as letras corretas e suas posições
 * 
 * Se a letra estiver na posição correta, ela é marcada como verde
 * Se a letra estiver na palavra, mas na posição errada, ela é marcada como amarela
 * Se a letra não estiver na palavra, ela não recebe marcação (*)
 * 
 * O jogo termina quando o jogador adivinha a palavra ou esgota as tentativas
 * @param void
 * @return 0 se o jogo for concluído com sucesso. 1 se o jogo não rodar
 */
int main(void)
{
    char palavra[8] = {0}, palavra_var[8], cores[8], letras_g[8] = {0}, letras_y[8] = {0}, letras_e[25] = {0}, letras_y_filter[8], letras[27] = {0},letras_temp[27] = {0}, letras_temp2[27] = {0}, letras_d[27] = {0};
    /**
     * palavra = palavra a ser adivinhada
     * palavra_var = cópia de palavra usada para verificar letras
     * cores = cores das letras do chute
     * letras = alfabeto
     * letras_d = letras desconhecidas
     * letras_e = letras erradas
     * letras_g = letras corretas
     * letras_y = letras parc. corretas
     * letras_y_filter = letras parc. corretas filtradas (sem letras corretas)
     * letras_temp = letras temporárias usadas para filtrar
     * letras_temp2 = letras temporárias usadas para filtrar
     */
    preencher_alfabeto(letras);
    int tentativas = 1, index_i, index_j, index_e;
    bool found = false;
    
    gerar_palavra(palavra);
    if(strlen(palavra) < 5) {
        printf("Erro ao ler a palavra.\n");
        return 1;
    }
    strncpy(palavra_var, palavra, sizeof palavra - 1);
    char entrada[160];
    strncpy(cores, "*****", 7);
    cores[strcspn(cores, "\n")] = '\0';
    // puts(palavra); // debug
    for (int i = 1; i <= 6; i++)
    {
        index_i = strlen(letras_g);
        index_e = strlen(letras_e);
        index_j = strlen(letras_y);
        printf("Chute %d: ", i);

        while (fgets(entrada, sizeof entrada - 1, stdin))
        {
            entrada[strcspn(entrada, "\n")] = '\0';
            for (int j = 0; j < strlen(entrada); j++)
            {
                entrada[j] = tolower(entrada[j]);
            }
            if (strlen(entrada) == 5)
                if(procurar_palavra(entrada)) {
                    break;
                } else {
                    printf("Palavra inválida. Tente novamente.\n");
            }
            else
            {
                printf("Insira uma palavra com 5 letras.\n");
            }
            printf("Chute %d: ", i);
        }
        
        guardar_letras_corretas(entrada, palavra, cores, palavra_var, &index_i, letras_g);
        guardar_letras_meio_corretas(cores, palavra_var, entrada, letras_g, letras_y, &index_j);
        guardar_letras_erradas(letras_e, entrada, letras_y, letras_g, &index_e);
       
        remover_caracteres_iguais(letras_g, letras_y, letras_y_filter);
        remover_caracteres_iguais(letras_g, letras, letras_temp);
        remover_caracteres_iguais(letras_y_filter, letras_temp, letras_temp2);
        remover_caracteres_iguais(letras_e, letras_temp2, letras_d);

        qsort(letras_g, strlen(letras_g), sizeof(char), comparar);
        qsort(letras_y_filter, strlen(letras_y_filter), sizeof(char), comparar);
        qsort(letras_e, strlen(letras_e), sizeof(char), comparar);

        printf("Letras desconhecidas: ");
        exibir_letras(letras_d);

        printf("\n");
        
        printf(GRN);

        printf("Letras corretas: ");
        exibir_letras(letras_g);

        printf(RESET);

        printf("\n");

        printf(YEL);

        printf("Letras parcialmente corretas: ");
        exibir_letras(letras_y_filter);

        printf(RESET);

        printf("\n");

        printf(RED);

        printf("Letras erradas: ");
        exibir_letras(letras_e);

        printf(RESET);
        
        printf("\n");

        imprimir_chute(cores, entrada);
        
        printf(RESET);

        printf("\n");

        if (!strcmp(entrada, palavra))
        {
            found = true;
            break;
        }
        strncpy(palavra_var, palavra, sizeof palavra - 1);
        palavra_var[strcspn(palavra_var, "\n")] = '\0';
        strncpy(cores, "*****", 7);
        cores[strcspn(cores, "\n")] = '\0';
        tentativas++;
    }
    if (found)
    {
        printf("Parabéns, você achou a palavra em %d tentativa(s)!\n", tentativas);
    }
    else
    {
        printf("Você perdeu! A palavra era \"%s\".\n", palavra);
    }
    return 0;
}