# cwordle

Clone de Wordle feito em C

# Explicação do Código
## word()
O procedimento word() recebe uma "string" e atribui uma palavra pseudoaleatória a essa string.

Usa `srand(time(NULL)` para gerar uma semente

Manipulação de arquivo básica
```c
    FILE *arquivo = fopen("./word_list/wordle-list", "r");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        return;
    }
```
Contagem de linhas

*Importante notar que o arquivo a ser utilizado será o mesmo para todos os casos (pelo menos na implementação atual). Poderia até tirar essa seção se não for implementar outro banco de palavras, uma vez que a quantidade de linhas sempre será mesma.*
```c
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
```
Isto coloca o ponteiro do arquivo no começo
```c
    fseek(arquivo, 0, SEEK_SET);
```
    
Por fim, a função rand() gera um número aleatório com line_count sendo o máximo.
**line** recebe a palavra que está na (i+1)° linha.

O if serve para sair do loop para casos não definidos.
```c
    int random_line_number = rand() % line_count;

    char line[7];
    for (int i = 0; i < random_line_number; i++) {
        if (fgets(line, 7, arquivo) == NULL) {
            break;
        }
    }
    fclose(arquivo);
    strncpy(linha, line, sizeof line -1);
    linha[strcspn(linha, "\n")] = '\0';
}
```
## remover_caracteres_iguais()
O nome do procedimento já indica sua funcionalidade. Não há muito o que explicar, uma vez que é um algoritmo que roubei.

## comp()
Função de comparação para a função qsort()

## main() - 1
Agora a parte principal!
Com
```#include "alg.h"``` consegue-se importar os algoritmos de alg.h.

```c
    char chute[8], palavra[8], palavra_var[8], cores[8], letras_g[8] = {0}, letras_y[8] = {0}, letras_y_filter[8];
    bool found = false;
    int tentativas = 1, index_i, index_j;
    word(palavra);
    strncpy(palavra_var, palavra, sizeof palavra - 1);
    char entrada[160];
    strncpy(cores, "#####", 7); 
    cores[strcspn(cores, "\n")] = '\0';
    // puts(palavra); linha de código utilizada apenas para debug
```
A seguir, tem-se a explicação das variáveis do código acima.

**chute** é um elemento de qualidade de vida para o jogador, para mostrar como que seu chute compara com a reposta.

Exemplo:
```bash
# A palavra é waker
Chute 1: salet
Letras verdes: a, e
Letras amarelas:
Cores: #G#G#
*a*e* #esse aqui é o array chute!
Chute 2: waker
Letras verdes: a, e, w, k, r
Letras amarelas:
Cores: GGGGG
Parabéns, você achou a palavra em 2 tentativa(s)!
```
**palavra** é onde será armazenado nossa palavra pseudoaleatória:
```word(palavra)```

**palavra_var** é interessante, uma vez que a lógica das letras verdes e amarelas depende dela. Basicamente é cópia de *palavra*, que pode-se modificar pois não afetará o resultado.  Ela será abordada mais a frente.

**cores** é definida como "#####" no começo de cada tentativa. Quando o jogador inserir o chute dele, ela será modificada de acordo com o sucesso da tentativa. 

Seja *a* a entrada informada do jogador e um array de caracteres: 
* cores[i] será 'G' (green) se a[i] for igual a palavra[i]. 
* cores[i] será 'Y' (yellow) se a[i] está na palavra, porém não está na posição correta. 
* cores[i] permanecerá '#' se a[i] não estiver na palavra.

*Importante notar que a lógica de letras amarelas tem uma característica chata, como demonstrado a seguir:*
```bash
# A palavra é "flood"
Chute 1: ooxzo
Letras verdes: 
Letras amarelas: o
Cores: YY###
```
*Perceba que o terceiro o não foi contado, apesar da palavra "flood" conter 'o'.*

**letras_g**, **letras_y**, e **letras_y_filter** são usadas para mostrar as letras verdes e amarelas, vistas nos exemplos. **letras_y_filter** é, na verdade, o que será exibido para letras amarelas. **letras_y** contém as letras de **letras_g**, o que não é útil para exibir. Por isso, usa-se remover_caracteres_iguais.

**found** é um *flag* básico para determinar se a palavra foi encontrada ou não.

**tentativas** é usada para contar tentativas. Começa com 1 pois o jogador sempre terá gasto uma tentativa.

**index_i** e **index_j** são usadas, respectivamente, para colocar letras em **letras_g** e **letras_y** na  primeira posição não ocupada.

**entrada** é onde será armazenado o input do jogador.

Pode ter percebido que inicializei a maioria dos arrays com tamanho 8. Por quê? Porque funciona, e provavelmente será alterado na versão final do projeto.

## main() - 2
Chegou a hora da lógica do jogo:

O jogador terá 6 tentativas, por isso ```i < 7```.

Como descrito em *main() - 1*, **index_i** será usado como índice para inserir uma letra na primeira posição não ocupada.
```c
for (int i = 1; i < 7; i++)
    {
        index_i = strlen(letras_g);
        if (i != 1)
        {
            puts(chute);
        }
        printf("Chute %d: ", i);
        strncpy(chute, "*****", 6);
        chute[strcspn(chute, "\n")] = '\0';
```
O while é usado abaixo para tratamento de erro (casos em que a qtd de caracteres de **entrada** != 5) 
```c
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
```
A seguir tem-se dois loops similares, um para letras verdes e outro para letras amarelas.

```c
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
```
        
## License

[GNU-GPL3.0](https://choosealicense.com/licenses/gpl-3.0/)
