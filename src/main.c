// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <unistd.h>
// #include <signal.h>
// #include <errno.h>
// #include <sys/wait.h>
// #include "parser.h"
// #include "executor.h"
// #include "builtins.h"

// static char ultima[MAX_LINE] = "";

// static void tratador_sigchld(int sig) {
//     (void)sig;
//     int errno_salvo = errno;
//     int st;
//     /* Recolhe processos terminados em background para evitar processos zumbi */
//     while (waitpid(-1, &st, WNOHANG) > 0) {
//     }
//     errno = errno_salvo;
// }

// static void tratador_sigint(int sig) {
//     (void)sig;
//     write(STDOUT_FILENO, "\nmysh> ", 7);
// }

// static void instalar_sinais(void) {
//     struct sigaction sa;
//     memset(&sa, 0, sizeof sa);
//     sigemptyset(&sa.sa_mask);

//     sa.sa_handler = tratador_sigchld;
//     sa.sa_flags = SA_RESTART;
//     sigaction(SIGCHLD, &sa, NULL);

//     sa.sa_handler = tratador_sigint;
//     sa.sa_flags = SA_RESTART;
//     sigaction(SIGINT, &sa, NULL);
// }

// int main(void) {
//     char linha[MAX_LINE];
//     instalar_sinais();

//     for (;;) {
//         printf("mysh> ");
//         fflush(stdout);

//         if (fgets(linha, sizeof linha, stdin) == NULL) {
//             printf("\n"); /* Trata o Ctrl+D (EOF) */
//             break;
//         }

//         /* Remover o '\n' final */
//         linha[strcspn(linha, "\n")] = '\0';

//         /* Ignorar linha vazia */
//         if (linha[0] == '\0') {
//             continue;
//         }

//         /* Funcionalidade de Histórico (!!) */
//         if (strcmp(linha, "!!") == 0) {
//             if (ultima[0] == '\0') {
//                 fprintf(stderr, "Erro: Nenhum comando no historico.\n");
//                 continue;
//             }
//             strcpy(linha, ultima);
//             printf("%s\n", linha); /* Ecoa o comando */
//         }

//         strcpy(ultima, linha);

//         LinhaComando cmd_line;
//         memset(&cmd_line, 0, sizeof(LinhaComando));

//         /* 1. Detectar background '&' no final da linha */
//         int len = strlen(linha);
//         while (len > 0 && (linha[len-1] == ' ' || linha[len-1] == '\t')) {
//             linha[--len] = '\0';
//         }

//         if (len > 0 && linha[len-1] == '&') {
//             cmd_line.background = 1;
//             linha[len-1] = '\0';
//         }

//         char *rest = linha;
//         char *token_pipe;
//         int i = 0;

//         while ((token_pipe = strtok_r(rest, "|", &rest)) != NULL && i < MAX_CMDS) {
//             Comando *cmd = &cmd_line.comandos[i];
//             char *tokens[MAX_ARGS + 1];

//             int n_toks = tokenizar_linha(token_pipe, tokens, MAX_ARGS);

//             if (n_toks > 0) {
//                 int arg_idx = 0;

//                 for (int t = 0; t < n_toks; t++) {
//                     if (strcmp(tokens[t], "<") == 0 && t + 1 < n_toks) {
//                         cmd->arq_entrada = tokens[++t];
//                     } else if (strcmp(tokens[t], ">") == 0 && t + 1 < n_toks) {
//                         cmd->arq_saida = tokens[++t];
//                         cmd->append = 0;
//                     } else if (strcmp(tokens[t], ">>") == 0 && t + 1 < n_toks) {
//                         cmd->arq_saida = tokens[++t];
//                         cmd->append = 1;
//                     } else {
//                         cmd->argv[arg_idx++] = tokens[t];
//                     }
//                 }
//                 cmd->argv[arg_idx] = NULL;
//                 cmd->argc = arg_idx;
//                 i++;
//             }
//         }
//         cmd_line.n_cmds = i;

//         /* --- INÍCIO DA EXECUÇÃO --- */
//         if (cmd_line.n_cmds > 0) {
//             /* Se for apenas um comando e for um comando interno (builtin), executa-o no processo pai */
//             if (cmd_line.n_cmds == 1 && executar_builtin(&cmd_line.comandos[0]) == 0) {
//                 continue;
//             } else {
//                 /* Caso contrário, é um comando externo ou pipeline -> vai para o executor.c */
//                 executaPipeline(&cmd_line);
//             }
//         }
//     }
//     return 0;
// }
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
            printf("\n"); // Tratamento do CTRL + D (mata o shell)
            break;
        }

        linha[strlen(linha) - 1] = '\0'; /*remover o ’\n’ final*/
        if (linha[0] == '\0')
        { /* ignorar linha vazia*/
            continue;
        }

        int num_tokens = tokenizar_linha(linha, listaTokens, MAX_CMDS);

        if (num_tokens < 0)
        {
            fprintf(stderr, "mysh: erro de sintaxe\n");
            continue;
        }

        for (int i = 0; i < num_tokens; i++)
        {
            printf("token[%d]: %s\n", i, listaTokens[i]);
        }

        // Supondo que listaTokens seja um array de strings (char* listaTokens[])
        Comando cmd = {
            .argv = *listaTokens, // Preenche o array de argumentos
            .argc = num_tokens,
            .arq_entrada = NULL,
            .arq_saida = NULL,
            .append = 0
        };

        executar_comando_simples(&cmd);
    }

    return 0;
}