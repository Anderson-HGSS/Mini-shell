#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include "parser.h"

int tokenizar_linha(const char *linha, char *tokens[], int max_tokens) {
    enum { FORA, PALAVRA, ASPAS } estado = FORA;
    char aspa = 0;
    char buf[MAX_LINE];
    int n = 0, len = 0;

    for (const char *p = linha; *p != '\0'; p++) {
        switch (estado) {
        case FORA:
            if (isspace(*p)) {
                continue;
            } else if (*p == '"' || *p == '\'') {
                estado = ASPAS;
                aspa = *p;
                len = 0;
            } else {
                estado = PALAVRA;
                buf[0] = *p;
                len = 1;
            }
            break;
        case PALAVRA:
            if (isspace(*p)) {
                buf[len] = '\0';
                if (n < max_tokens) tokens[n++] = strdup(buf);
                estado = FORA;
            } else if (*p == '"' || *p == '\'') {
                estado = ASPAS;
                aspa = *p;
            } else {
                buf[len++] = *p;
            }
            break;
        case ASPAS:
            if (*p == aspa) {
                estado = PALAVRA;
            } else {
                buf[len++] = *p;
            }
            break;
        }
    }

    if (estado == ASPAS) {
        fprintf(stderr, "Erro sintático: aspas abertas no fim do comando.\n");
        return -1;
    }
    if (estado == PALAVRA) {
        buf[len] = '\0';
        if (n < max_tokens) tokens[n++] = strdup(buf);
    }

    tokens[n] = NULL;
    return n;
}