#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include "parser.h"


static void aplicar_redirecionamentos(const Comando *cmd)
{
    if (cmd->arq_entrada != NULL)
    {
        int fd = open(cmd->arq_entrada, O_RDONLY);
        if (fd < 0)
        {
            perror(cmd->arq_entrada);
            _exit(1);
        }
        /* TODO: dup2 para STDIN_FILENO e close(fd) */
        dup2(fd, 0);
        close(fd);
    }
    
    if (cmd->arq_saida != NULL)
    {
        int flags = O_WRONLY | O_CREAT/* | TODO: O_TRUNC ou O_APPEND */;
        int fd;
        
        if(cmd->append == 0){

            fd = open(cmd->arq_saida, flags | O_APPEND, 0644);
        
        }else if(cmd->append == 1){
        
            fd = open(cmd->arq_saida, flags | O_TRUNC, 0644);
        
        }
        
        if(fd < 0){
            perror(cmd->arq_saida);
            _exit(1);
        }

        dup2(fd, 1);
        close(fd);


        /* TODO: open com modo 0644, tratar erro, dup2, close */
    }
}


int executaExterno(Comando *cmd)
{

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("mysh: fork");
        return -1;
    }

    if (pid == 0)
    {

        /* ---- FILHO ---- */
        /* TODO (Passo 8): restaurar SIGINT para SIG_DFL */
        /* TODO (Passo 6): aplicar redirecionamentos
         */


        aplicar_redirecionamentos(cmd);

        // Aqui executa os comandos externos
        execvp(cmd->argv[0], cmd->argv);
        fprintf(stderr, "mysh: %s: comando não encontrado\n", cmd->argv[0]);

        _exit(127);
    }

    /* ---- PAI ---- */
    int status;
    waitpid(pid, &status, 0);
    if (WIFEXITED(status))
    {
        int codigo_saida = WEXITSTATUS(status);
        printf("o processo terminou com o código %d \n", codigo_saida);
    }
}