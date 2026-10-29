#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "parser.h"


void executaPipeline(LinhaComando *linha);


int verificaComando(Comando *cmd);
int executaExterno(Comando *cmd);
void trataComandos(Comando *cmd);
int redirecionaEntrada(const char *arquivo);
int redirecionaSaida(const char *arquivo);
int redirecionaAppend(const char *arquivo);
void executaBackground(Comando *cmd);


#endif /* EXECUTOR_H */