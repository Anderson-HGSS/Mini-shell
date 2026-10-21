#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "parser.h"

/* 
 * Função principal responsável por executar a linha de comando interpretada, 
 * lidando com pipelines, redirecionamentos de entrada/saída e execução em background.
 */
void executaPipeline(LinhaComando *linha);

/* 
 * Assinaturas adicionais baseadas no relatório de projeto (disponibilizadas 
 * caso decida expandir a modularidade do seu executor.c no futuro).
 */
int verificaComando(Comando *cmd);
int executaExterno(Comando *cmd);
void trataComandos(Comando *cmd);
int redirecionaEntrada(const char *arquivo);
int redirecionaSaida(const char *arquivo);
int redirecionaAppend(const char *arquivo);
void executaBackground(Comando *cmd);

#endif /* EXECUTOR_H */