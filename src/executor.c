#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include "parser.h"

static int proximo_job = 1;

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
        int flags = O_WRONLY | O_CREAT /* | TODO: O_TRUNC ou O_APPEND */;
        int fd;

        if (cmd->append == 0)
        {

            fd = open(cmd->arq_saida, flags | O_APPEND, 0644);
        }
        else
        {

            fd = open(cmd->arq_saida, flags | O_TRUNC, 0644);
        }

        if (fd < 0)
        {
            perror(cmd->arq_saida);
            _exit(1);
        }

        dup2(fd, 1);
        close(fd);

        /* TODO: open com modo 0644, tratar erro, dup2, close */
    }
}

void executaPipeline(LinhaComando *linha)
{
    int pipes[MAX_CMDS - 1][2];
    pid_t pids[MAX_CMDS];
    int n = linha->n_cmds;

    for (int i = 0; i < n - 1; i++)
    {
        if (pipe(pipes[i]) < 0)
        {
            perror("mysh: pipe");
            for (int j = 0; j < i; j++)
            {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }
            return;
        }
    }

    /*filhos criados*/
    int criados = 0;
    for (int i = 0; i < n; i++)
    {
        pids[i] = fork();
        if (pids[i] < 0)
        {
            perror("mysh: fork");
            break;
        }
        criados++;

        if (pids[i] == 0)
        {
            /* filho*/
            signal(SIGINT, SIG_DFL);

            if (i > 0)
            {
                dup2(pipes[i - 1][0], STDIN_FILENO);
            }

            if (i < n - 1)
            {
                dup2(pipes[i][1], STDOUT_FILENO);
            }

            for (int j = 0; j < n - 1; j++)
            {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }

            /* redirecionamentos vêm DEPOIS: '<' e '>' têm prioridade sobre o pipe */
            aplicar_redirecionamentos(&linha->comandos[i]);

            execvp(linha->comandos[i].argv[0], linha->comandos[i].argv);
            fprintf(stderr, "mysh: %s: comando não encontrado\n", linha->comandos[i].argv[0]);

            _exit(127);
        }
    }

    /* ---- PAI ---- */
    for (int j = 0; j < n - 1; j++)
    {
        close(pipes[j][0]);
        close(pipes[j][1]);
    }

    if (!linha->background)
    {
        int interrompido = 0;
        for (int i = 0; i < criados; i++)
        {
            int status;
            if (waitpid(pids[i], &status, 0) > 0 &&
                WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
                interrompido = 1;
        }
        /*tratamento \n do Ctrl + C*/
        if (interrompido)
            write(STDOUT_FILENO, "\n", 1);
    }

    else if (criados > 0)
    {
        printf("[%d] %d\n", proximo_job++, pids[criados - 1]);
    }
}

int executaExterno(Comando *cmd, int background)
{

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("mysh: fork");
        return -1;
    }

    if (pid == 0)
    {

        // Aqui o comando é 'executado' em background
        if (background)
        {
            int fd = open("/dev/null", O_RDONLY); // buraco negro
            dup2(fd, 0);
            close(fd);
        }

        signal(SIGINT, SIG_DFL);
        aplicar_redirecionamentos(cmd);

        // Aqui executa os comandos externos
        execvp(cmd->argv[0], cmd->argv);
        fprintf(stderr, "mysh: %s: comando não encontrado\n", cmd->argv[0]);

        _exit(127);
    }

    /* ---- PAI ---- */
    if (background)
    {
        printf("[%d] %d\n", proximo_job++, pid);
        return 0;
    }

    int status;
    if (waitpid(pid, &status, 0) < 0)
        return -1;

    if (WIFSIGNALED(status))
    {
        /*tratamento \n do Ctrl + C*/
        if (WTERMSIG(status) == SIGINT){
            write(STDOUT_FILENO, "\n", 1);
        }
        return 128 + WTERMSIG(status);
    }

    return WEXITSTATUS(status);
}
