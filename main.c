#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "alg.c" // Algoritmos para gerar uma palavra aleatória
int main(void)
{
    char linha[7], chute[8], palavra[8], palavra_var[8], cores[7], letras_g[7], letras_y[7], letras_y_filter[7]; // VALORES DE ARRAY MTO ALEATÓRIOS ISSO DEU CERTO ENT NN VOU MUDAR
    bool found = false;
    int tentativas = 1, index_i, index_j;
    strncpy(palavra, word(linha), sizeof word(linha) - 1);
    strncpy(palavra_var, palavra, sizeof palavra - 1);
    // puts(palavra);
    char entrada[160]; // Evitar casos em que qtd de caracteres>5 (Veja ponto 1)
    strncpy(cores, "#####", 7);
    cores[strcspn(cores, "\n")] = '\0';
    for (int i = 1; i < 7; i++)
    {
        index_i = strlen(letras_g);
        puts(palavra);
        if (i != 1)
        {
            puts(chute);
        }
        printf("Chute %d: ", i);
        strncpy(chute, "*****", 5);
        while (fgets(entrada, sizeof entrada - 1, stdin))
        {
            entrada[strcspn(entrada, "\n")] = '\0';
            if (strlen(entrada) == 5)
            { // 5 letras (1)
                break;
            }
            else
            {
                printf("Insira uma palavra com 5 letras.\n");
            }
            printf("Chute %d: ", i);
        }
        for (int i = 0; i < 5; i++)
        {
            entrada[i] = tolower(entrada[i]);
            if (entrada[i] == palavra[i])
            {
                chute[i] = entrada[i];
                cores[i] = 'G';
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
            entrada[i] = tolower(entrada[i]);
            if (cores[i] != 'G' && strchr(palavra_var, entrada[i]))
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
        // int n = sizeof(letras_g)/sizeof(letras_g[0]);
        // qsort(letras_g, n, sizeof(char), comparar);
        // teste2(letras_g, strlen(letras_g));
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
        // n = sizeof(letras_y_filter)/sizeof(letras_y_filter[0]);
        // qsort(letras_y_filter, n, sizeof(char), comparar);
        // teste2(letras_y_filter, strlen(letras_y_filter));
        printf("Letras amarelas: ");
        for (int i = 0; i < strlen(letras_y_filter); i++)
        {
            if (i != strlen(letras_y_filter) - 1)
            {
                printf("%c,", letras_y_filter[i]);
            }
            else
            {
                printf("%c", letras_y_filter[i]);
            }
        }
        printf("\n");
        puts(cores);

        if (!strcmp(entrada, palavra))
        {
            found = true;
            break;
        }
        strncpy(palavra_var, palavra, sizeof palavra - 1);
        palavra_var[strcspn(palavra_var, "\n")] = '\0';
        strncpy(cores, "#####", 7);
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
}