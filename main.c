#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "alg.h"

/**
 * Função principal do jogo CWordle
 * 
 * O jogador deve adivinhar uma palavra de 5 letras em até 6 tentativas
 * 
 * A cada tentativa, o jogador recebe feedback sobre as letras corretas e suas posições
 * 
 * Se a letra estiver na posição correta, ela é marcada como verde (G)
 * Se a letra estiver na palavra, mas na posição errada, ela é marcada como amarela (Y)
 * Se a letra não estiver na palavra, ela não recebe marcação
 * 
 * O jogo termina quando o jogador adivinha a palavra ou esgota as tentativas
 * @param void
 * @return 0 se o jogo for concluído com sucesso. 1 se o jogo não rodar
 */
int main(void)
{
    char chute[8], palavra[8] = {0}, palavra_var[8], cores[8], letras_g[8] = {0}, letras_y[8] = {0}, letras_y_filter[8];
    bool found = false;
    int tentativas = 1, index_i, index_j;
    word(palavra);
    if(strlen(palavra) < 5) {
        printf("Erro ao ler a palavra.\n");
        return 1;
    }
    // palavra[strcspn(palavra, "\n")] = '\0';
    strncpy(palavra_var, palavra, sizeof palavra - 1);
    char entrada[160];
    strncpy(cores, "*****", 7);
    cores[strcspn(cores, "\n")] = '\0';
    puts(palavra); // debug
    for (int i = 1; i <= 6; i++)
    {
        index_i = strlen(letras_g);
        if (i != 1)
        {
            puts(chute);
        }
        printf("Chute %d: ", i);
        strncpy(chute, "#####", 6);
        chute[strcspn(chute, "\n")] = '\0';

        while (fgets(entrada, sizeof entrada - 1, stdin))
        {
            entrada[strcspn(entrada, "\n")] = '\0';
            for (int i = 0; i < strlen(entrada); i++)
            {
                entrada[i] = tolower(entrada[i]);
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


        for (int i = 0; i < 5; i++)
        {
            if (entrada[i] == palavra[i])
            {
                chute[i] = entrada[i];
                // cores[i] = 'G';
                char *pos = strchr(palavra_var, entrada[i]);
                if (pos)
                {
                    *pos = ' ';
                    if (!strchr(letras_g, entrada[i]))
                    {
                        letras_g[index_i] = entrada[i];
                        index_i++;
                    }
                }
            }
        }

        letras_g[strcspn(letras_g, "\n")] = '\0';
        index_j = strlen(letras_y);

        for (int i = 0; i < 5; i++)
        {
            if 
            (
                cores[i] != 'G' && 
                strchr(palavra_var, entrada[i]))
            {
                cores[i] = 'Y';
                char *pos = strchr(palavra_var, entrada[i]);
                if (pos)
                {
                    *pos = ' ';
                    if (!strchr(letras_y, entrada[i]) && !strchr(letras_g, entrada[i]))
                    {
                        letras_y[index_j] = entrada[i];
                        index_j++;
                    }
                }
            }
        }
        remover_caracteres_iguais(letras_g, letras_y, letras_y_filter);


        qsort(letras_g, strlen(letras_g), sizeof(char), comp);
        qsort(letras_y_filter, strlen(letras_y_filter), sizeof(char), comp);

        printf("Letras verdes: ");
        for (int i = 0; i < strlen(letras_g); i++)
        {
            if (i != strlen(letras_g) - 1)
            {
                printf("%c, ", letras_g[i]);
            }
            else
            {
                printf("%c", letras_g[i]);
            }
        }
        printf("\n");

        printf("Letras amarelas: ");
        for (int i = 0; i < strlen(letras_y_filter); i++)
        {
            if (i != strlen(letras_y_filter) - 1)
            {
                printf("%c, ", letras_y_filter[i]);
            }
            else
            {
                printf("%c", letras_y_filter[i]);
            }
        }
        printf("\n");
        // printf("Cores: %s\n", cores);

        if (!strcmp(entrada, palavra))
        {
            found = true;
            break;
        }
        strncpy(palavra_var, palavra, sizeof palavra - 1);
        palavra_var[strcspn(palavra_var, "\n")] = '\0';
        strncpy(cores, "*****", 7);
        // cores[strcspn(cores, "\n")] = '\0';
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