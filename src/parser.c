#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include "parser.h"
#include <string.h>

/* Devolve o número de tokens ou -1 em erro (ex.: aspas não fechadas).
 * tokens[i] deve apontar para strings alocadas (strdup) ou para um buffer
 * próprio -- documente sua escolha e libere a memória depois. */
int tokenizar_linha(const char *linha, char *tokens[], int max_tokens) {
    int qtdToken = 0;
    int tam_linha = strlen(linha);

    for (int i = 0; i < tam_linha; i++) {
        if (qtdToken >= max_tokens) {
            break;
        }

        if (isspace((unsigned char)linha[i])) {
            continue;
        }

        int inicio;
        int fim;

        if (linha[i] == '"') {
            i++;                    /* pula a aspa de abertura */
            inicio = i;
            while (linha[i] != '\0' && linha[i] != '"') {
                i++;
            }

            if (linha[i] == '\0') { /* chegou ao fim sem achar a aspa de fechamento */
                for (int k = 0; k < qtdToken; k++) {
                    free(tokens[k]);
                }
                return -1;
            }

            fim = i;                /* i está na aspa de fechamento */
        } else {
            inicio = i;
            while (linha[i] != '\0' && !isspace((unsigned char)linha[i])) {
                i++;
            }
            fim = i;                /* i está no espaço (ou no '\0') */
        }

        int tamanho = fim - inicio;

        tokens[qtdToken] = malloc(tamanho + 1);

        if (tokens[qtdToken] == NULL) {
            for (int k = 0; k < qtdToken; k++) {
                free(tokens[k]);
            }
            return -1;
        }

        for (int k = 0; k < tamanho; k++) {
            tokens[qtdToken][k] = linha[inicio + k];
        }
        tokens[qtdToken][tamanho] = '\0';
        qtdToken++;
    }

    return qtdToken;
}


// ==================================================================================================================================================================
//                                                                     Função de teste
// ==================================================================================================================================================================




// #define MAX_TOKENS 16

// /* ... coloque aqui a sua função tokenizar_linha ... */

// static void testar(const char *linha) {
//     char *tokens[MAX_TOKENS];
//     int n = tokenizar_linha(linha, tokens, MAX_TOKENS);

//     printf("Linha: [%s]\n", linha);
//     if (n < 0) {
//         printf("  erro de memoria\n\n");
//         return;
//     }
//     printf("  %d token(s)\n", n);
//     for (int k = 0; k < n; k++) {
//         printf("  tokens[%d] = [%s]\n", k, tokens[k]);
//         free(tokens[k]);
//     }
//     printf("\n");
// }

// int main(void) {
//     testar("echo ola");
//     testar("echo \"ola mundo\" teste");
//     testar("   muitos     espacos   aqui   ");
//     testar("\"so entre aspas\"");
//     testar("aspa nao fechada \"abc def");
//     testar("");
//     return 0;
// }