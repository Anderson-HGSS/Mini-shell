
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


static void tratador_sigchld(int sig) {
    (void)sig;
    
    int errno_salvo = errno;
    int status;
    pid_t pid;
    
    while((pid = waitpid(-1, &status, WNOHANG)) > 0 );

    errno = errno_salvo;

}


static void instalar_sinais(void) {
    struct sigaction sa;
    
    memset(&sa, 0, sizeof sa);
    
    sigemptyset(&sa.sa_mask);
    
    sa.sa_handler = tratador_sigchld;
    sa.sa_flags = SA_RESTART; /* read/fgets não falham com EINTR */

    sigaction(SIGCHLD, &sa, NULL);

    // Aqui a gente trata o Ctrl + C
    /* TODO: SIGINT -> SIG_IGN (ou tratador que só escreve "\n") */
    
    sa.sa_handler = SIG_IGN;
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);
}

int main(void)
{
    char linha[MAX_LINE];
    char *listaTokens[MAX_LINE];
    int entre_aspas[MAX_LINE];
    LinhaComando linha_cmd;
    char erro[256];

    /*(Passo 08): instalar tratadores de sinal*/
    instalar_sinais();
    
    for (;;)
    {
        printf("mysh> ");
        fflush(stdout); /* o prompt não termina com \n! */

        if (fgets(linha, sizeof linha, stdin) == NULL)
        {
            printf("\nShell finalizado!\n"); /* CTRL + D */
            break;
        }

        linha[strcspn(linha, "\n")] = '\0'; /* remove o '\n' final, se houver */
        if (linha[0] == '\0')
            continue; /* ignora linha vazia */

        /* 1. tokenizar */
        int num_tokens = tokenizar_linha(linha, listaTokens, entre_aspas, MAX_LINE);
        if (num_tokens < 0)
        {
            fprintf(stderr, "mysh: erro de sintaxe\n");
            continue;
        }
        if (num_tokens == 0) /* linha só com espaços */
            continue;

        /* 2. interpretar */
        if (interpretar_tokens(listaTokens, entre_aspas, num_tokens,
                               &linha_cmd, erro, sizeof erro) < 0)
        {
            fprintf(stderr, "\nmysh: %s\n", erro);
        }


        /* 3. executar */
        else if (linha_cmd.n_cmds == 1)
        {

            if (verfComandointerno(&linha_cmd.comandos[0]) == 0){
                executaExterno(&linha_cmd.comandos[0],linha_cmd.background);            
            }

        }

        else{
            executaPipeline(&linha_cmd);
        }

        /* 4. liberar os tokens (só depois de executar) */
        for (int i = 0; i < num_tokens; i++)
            free(listaTokens[i]);
    }

    return 0;
}