#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "parser.h"

typedef int (*FuncBuiltin)(Comando *cmd);

static int builtin_cd(Comando *cmd);
static int builtin_pwd(Comando *cmd);
static int builtin_exit(Comando *cmd);
static int builtin_export(Comando *cmd);
static int builtin_help(Comando *cmd);

static const struct {
    const char *nome;
    FuncBuiltin f;
} BUILTINS[] = {
    {"cd", builtin_cd},
    {"pwd", builtin_pwd},
    {"exit", builtin_exit},
    {"export", builtin_export},
    {"help", builtin_help},
    {NULL, NULL}
};

static int builtin_cd(Comando *cmd) {
    const char *destino = cmd->argv[1] ? cmd->argv[1] : getenv("HOME");
    if (destino == NULL) {
        fprintf(stderr, "cd: destino não fornecido e HOME não definido\n");
        return 1;
    }
    if (chdir(destino) != 0) {
        perror("cd");
    }
    return 0;
}

static int builtin_pwd(Comando *cmd) {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s\n", cwd);
    } else {
        perror("pwd");
    }
    return 0;
}

static int builtin_exit(Comando *cmd) {
    exit(0);
    return 0;
}

static int builtin_export(Comando *cmd) {
    if (cmd->argv[1] != NULL) {
        char *eq = strchr(cmd->argv[1], '=');
        if (eq != NULL) {
            *eq = '\0';
            setenv(cmd->argv[1], eq + 1, 1);
        }
    }
    return 0;
}

static int builtin_help(Comando *cmd) {
    printf("Mini-Shell - Trabalho SO (Fedora Linux 44)\n");
    printf("Builtins suportados: cd, pwd, exit, export, help\n");
    return 0;
}

/* Função extra para chamar o builtin no executor */
int executar_builtin(Comando *cmd) {
    if (cmd->argv[0] == NULL) return 0;

    // Faz a busca pelo comando interno e chama ele
    for (int i = 0; BUILTINS[i].nome != NULL; i++) {
        if (strcmp(cmd->argv[0], BUILTINS[i].nome) == 0) {
            return BUILTINS[i].f(cmd);
        }
    }
    return -1; /* Não é builtin */
}