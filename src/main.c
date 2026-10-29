
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/wait.h>
#include "parser.h"
#include "executor.h"
#include "builtins.h"

int main(void)
{
    char linha[MAX_LINE];
    char *listaTokens[MAX_CMDS];
    /* TODO (Passo 8): instalar tratadores de sinal aqui */

    for (;;)
    {
        printf("mysh> ");
        fflush(stdout); /* o prompt não termina com \n! */

        if (fgets(linha, sizeof linha, stdin) == NULL)
        {
            printf("\nShell finalizado!\n"); // Tratamento do CTRL + D (mata o shell)
            break;
        }

        linha[strlen(linha) - 1] = '\0'; /*remover o ’\n’ final*/
        if (linha[0] == '\0')
        { /* ignorar linha vazia*/
            continue;
        }

        int num_tokens = tokenizar_linha(linha, listaTokens, MAX_CMDS);

        if (num_tokens < 0){
            fprintf(stderr, "mysh: erro de sintaxe\n");
            continue;
        }
        

        for (int i = 0; i < num_tokens; i++)
        {
            printf("token[%d]: %s\n", i, listaTokens[i]);
        }

        // Supondo que listaTokens seja um array de strings (char* listaTokens[])
        Comando cmd = {
            .argc = num_tokens,
            .arq_entrada = NULL,
            .arq_saida = NULL,
            .append = 0
        };
        
        cmd.argv[num_tokens] = NULL;

        for (int i = 0; i < num_tokens; i++) {
            cmd.argv[i] = listaTokens[i];
        } // Gambiarra por causa dos ponteiros das listas

        int num_comando = verfComandointerno(&cmd);
        
        if(num_comando == 0){
            executaExterno(&cmd);
        
        }
    
    }
    return 0;
}