#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include "parser.h"
#include <string.h>

/* Devolve o número de tokens ou -1 em erro (ex.: aspas não fechadas).
 * tokens[i] deve apontar para strings alocadas (strdup) ou para um buffer
 * próprio -- documente sua escolha e libere a memória depois. */

int tokenizar_linha(const char *linha, char *tokens[], int entre_aspas[], int max_tokens) {
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
        int estava_entre_aspas = 0;

        if (linha[i] == '"') {
            estava_entre_aspas = 1;
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

        entre_aspas[qtdToken] = estava_entre_aspas;

        qtdToken++;
    }

    return qtdToken;
}


/*se retornar 1 é com aspas, se for 0, sem aspas.*/
static int operador(char *tokens[], const int entre_aspas[], int i, const char *s) {
    return !entre_aspas[i] && strcmp(tokens[i], s) == 0;
}

static int eh_qualquer_op(char *tokens[], const int entre_aspas[], int i) {
    /*verifica cada tipo de operador*/ 
    return operador(tokens, entre_aspas, i, "|")  ||
           operador(tokens, entre_aspas, i, "<")  ||
           operador(tokens, entre_aspas, i, ">")  ||
           operador(tokens, entre_aspas, i, ">>") ||
           operador(tokens, entre_aspas, i, "&");
}

int interpretar_tokens(char *tokens[], const int entre_aspas[], int n, LinhaComando *linha, char *erro, size_t erro_tam) {
    
    memset(linha, 0, sizeof *linha);/*zera toda struct linha de comando*/
    Comando *atual = &linha->comandos[0];

    for (int i = 0; i < n; i++) {
         /*verifica se é passado o pipe antes de qualquer comando*/
        if (operador(tokens, entre_aspas, i, "|")) {
            if (atual->argc == 0) {
                snprintf(erro, erro_tam, "erro de sintaxe próximo a '|'");
                return -1;
            }
            /*pipeline ultrapassa o qtd máx de comandos permitidos*/
            if (linha->n_cmds + 1 >= MAX_CMDS) {
                snprintf(erro, erro_tam, "pipeline longo demais");
                return -1;
            }
            
            atual->argv[atual->argc] = NULL;
            linha->n_cmds++;
            atual = &linha->comandos[linha->n_cmds];

        } else if (operador(tokens, entre_aspas, i, "<")  ||
                   operador(tokens, entre_aspas, i, ">")  ||
                   operador(tokens, entre_aspas, i, ">>")) {
                    
            /*verfica se o token comando é o ultimo da linha.*/
            if (i + 1 >= n) {
                snprintf(erro, erro_tam, "erro de sintaxe, operador é o ultimo da linha");
                return -1;
            }
            /*Verifica se um operador nao foi passado logo depois de outro*/
            if (eh_qualquer_op(tokens, entre_aspas, i + 1)) {
                snprintf(erro, erro_tam, "erro de sintaxe próximo a '%s'", tokens[i + 1]);
                return -1;
            }

            //Guarda o nome do arquivo no comando atual
            if (operador(tokens, entre_aspas, i, "<")) {
                atual->arq_entrada = tokens[i + 1];
            } else {
                atual->arq_saida = tokens[i + 1];
                atual->append = operador(tokens, entre_aspas, i, ">");
            }
            i++;   /* ignora o nome do arquivo para não repetir */

        } else if (operador(tokens, entre_aspas, i, "&")) {
            if (i != n - 1) {
                /*verifica se o & não está no final da linha de comando*/
                snprintf(erro, erro_tam, "'&' só é válido no fim da linha");
                return -1;
            }
            linha->background = 1;

        } else {
            if (atual->argc >= MAX_ARGS) {
                snprintf(erro, erro_tam, "ultrapassou o limite máx de argumentos permitodos");
                return -1;
            }
            atual->argv[atual->argc++] = tokens[i];
        }
    }


    // Verifica se após o operador, há algum comando
    if (atual->argc == 0) {
        snprintf(erro, erro_tam, "erro de sintaxe: comando vazio");
        return -1;
    }

    //Atualiza a quantidade de comandos na linha, pois não pega o ultimo.
    linha->n_cmds++;
    return 0;
}