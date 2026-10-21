#ifndef PARSER_H
#define PARSER_H

#define MAX_ARGS 64
#define MAX_CMDS 16
#define MAX_LINE 1024

typedef struct {
    char *argv[MAX_ARGS + 1]; /* +1: execvp exige argv terminado em NULL */
    int argc;
    char *arq_entrada; /* NULL se não houver '<' */
    char *arq_saida; /* NULL se não houver '>' ou '>>' */
    int append; /* 1 para '>>', 0 para '>' */
} Comando;

typedef struct {
    Comando comandos[MAX_CMDS];
    int n_cmds;
    int background; /* 1 se terminar com '&' */
} LinhaComando;

/* Declaração das funções do parser (baseado no esqueleto) */
int tokenizar_linha(const char *linha, char *tokens[], int max_tokens);

#endif