

// static void aplicar_redirecionamentos(const Comando *cmd) {
//     if (cmd->arq_entrada != NULL) {
//         int fd = open(cmd->arq_entrada, O_RDONLY);
//         if (fd < 0) {
//             perror(cmd->arq_entrada);
//             _exit(1);
//         }
//         dup2(fd, STDIN_FILENO);
//         close(fd);
//     }
//     if (cmd->arq_saida != NULL) {
//         int flags = O_WRONLY | O_CREAT | (cmd->append ? O_APPEND : O_TRUNC);
//         int fd = open(cmd->arq_saida, flags, 0644);
//         if (fd < 0) {
//             perror(cmd->arq_saida);
//             _exit(1);
//         }
//         dup2(fd, STDOUT_FILENO);
//         close(fd);
//     }
// }

// void executaPipeline(LinhaComando *linha) {
//     int pipes[MAX_CMDS - 1][2];
//     pid_t pids[MAX_CMDS];
//     int n = linha->n_cmds;

//     /* Criar pipes */
//     for (int i = 0; i < n - 1; i++) {
//         if (pipe(pipes[i]) < 0) {
//             perror("mysh: pipe");
//             return;
//         }
//     }

//     for (int i = 0; i < n; i++) {
//         Comando *cmd = &linha->comandos[i];
//         pids[i] = fork();

//         if (pids[i] < 0) {
//             perror("mysh: fork");
//             return;
//         }

//         if (pids[i] == 0) { /* ---- FILHO ---- */
//             /* Passo 8: restaurar SIGINT para o comportamento padrão */
//             struct sigaction sa;
//             sa.sa_handler = SIG_DFL;
//             sigemptyset(&sa.sa_mask);
//             sa.sa_flags = 0;
//             sigaction(SIGINT, &sa, NULL);

//             /* Configurar pipes */
//             if (i > 0) {
//                 dup2(pipes[i - 1][0], STDIN_FILENO);
//             }
//             if (i < n - 1) {
//                 dup2(pipes[i][1], STDOUT_FILENO);
//             }

//             /* Fechar TODAS as pontas de TODOS os pipes no processo filho */
//             for (int j = 0; j < n - 1; j++) {
//                 close(pipes[j][0]);
//                 close(pipes[j][1]);
//             }

//             /* Passo 6: aplicar redirecionamentos de arquivo (<, >, >>) */
//             aplicar_redirecionamentos(cmd);

//             execvp(cmd->argv[0], cmd->argv);

//             /* Mensagem de erro exigida pelo relatório */
//             fprintf(stderr, "%s: comando não encontrado\n", cmd->argv[0]);
//             _exit(127);
//         }
//     }

//     /* ---- PAI ---- */
//     /* Fechar todas as pontas dos pipes no pai */
//     for (int i = 0; i < n - 1; i++) {
//         close(pipes[i][0]);
//         close(pipes[i][1]);
//     }

//     if (!linha->background) {
//         for (int i = 0; i < n; i++) {
//             int status;
//             /* waitpid tratando EINTR */
//             while (waitpid(pids[i], &status, 0) < 0) {
//                 if (errno != EINTR) break;
//             }
//         }
//     }
// }
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include "parser.h"

int executaExterno(Comando *cmd){
    
    pid_t pid = fork();

    if (pid < 0){
        perror("mysh: fork");
        return -1;
    }

    if (pid == 0){

        /* ---- FILHO ---- */
        /* TODO (Passo 8): restaurar SIGINT para SIG_DFL */
        /* TODO (Passo 6): aplicar redirecionamentos
         */
        
        execvp(cmd->argv[0], cmd->argv);
        fprintf(stderr,"mysh: %s: comando não encontrado\n",cmd->argv[0]);
       
        _exit(127);
 
    }

    /* ---- PAI ---- */
    int status;
    waitpid(pid, &status, 0);
    if (WIFEXITED(status)){
        int codigo_saida = WEXITSTATUS(status);
        printf("o processo terminou com o código %d \n", codigo_saida);
    }
    
    
}