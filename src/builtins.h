#ifndef BUILTINS_H
#define BUILTINS_H

#include "parser.h"

/*
 * Verifica se o comando passado é um builtin (cd, pwd, exit, export, help).
 * Retorna o status de saída do comando (0 para sucesso) ou -1 se não for um comando interno.
 * Substitui a necessidade de chamar verfComandointerno() e verfDestino() separadamente,
 * centralizando a execução lógica.
 */
int executar_builtin(Comando *cmd);

/* 
 * Assinaturas baseadas no relatório de projeto (podem ser implementadas
 * publicamente, embora no builtins.c atual tenham sido declaradas como static
 * para melhor proteção de escopo).
 */
int verfComandointerno(char *cmdName);
int verfDestino(Comando *cmd);
int builtincd(char **args);
int builtinexit(char **args);
int builtinpwd(char **args);
int builtinexport(char **args);
int builtinhelp(char **args);

#endif /* BUILTINS_H */