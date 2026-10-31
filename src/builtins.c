#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "parser.h"

typedef int (*FuncBuiltin)(Comando *cmd);
static int builtin_cd(Comando *cmd);
static int builtin_pwd(Comando *cmd);
static int builtin_exit(Comando *cmd);


static const struct{
    const char *nome;
    FuncBuiltin f;
} 

BUILTINS[] = {
    {"cd", builtin_cd},
    {"pwd", builtin_pwd},
    {"exit",builtin_exit},
};

int verfComandointerno(Comando *cmd){

    for(int i = 0; i < sizeof(BUILTINS) / sizeof(BUILTINS[0]); i++){
        if(strcmp(cmd->argv[0], BUILTINS[i].nome) == 0){
            BUILTINS[i].f(cmd);
            return 1;
        }
        
    }
    
    return 0;
}

static int builtin_cd(Comando *cmd){

    const char *destino;

    if(cmd->argv[1]){
        destino = cmd->argv[1];
    }else{
        destino = getenv("HOME");
    }

    if(destino != NULL && chdir(destino) != 0){
        perror("cd");
    }
    
    return 0;
}

static int builtin_pwd(Comando *cmd){
    
    char cwd[MAX_LINE];
    if (getcwd(cwd, sizeof(cwd)) != NULL){
        printf("%s\n", cwd);
       
    }else{
        perror("pwd");
    }
    
    return 0;
}

static int builtin_exit(Comando *cmd){
    exit(0);
}
