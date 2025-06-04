#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

const char* word(char *linha){
    srand(time(NULL));
    FILE *arquivo = fopen("./word_list/wordle-list", "r");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        return "1";
    }

    int line_count = 0;
    char temp_line[7];
    while (fgets(temp_line, sizeof temp_line, arquivo) != NULL) {
        line_count++;
    }
    if (line_count == 0) {
      printf("Arquivo vazio.\n");
      fclose(arquivo);
      return "0";
    }
    // printf("%d\n", line_count);
    fseek(arquivo, 0, SEEK_SET);

    

    int random_line_number = rand() % line_count;

    char line[7];
    for (int i = 0; i <= random_line_number; i++) {
        if (fgets(line, 7, arquivo) == NULL) {
            break;
        }
    }
    fclose(arquivo);
    // printf("%s\n", line);
    strncpy(linha, line, sizeof line -1);
    linha[strcspn(linha, "\n")] = '\0';
    // strncpy(linha, line, sizeof line-1);
    return linha;
}

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
