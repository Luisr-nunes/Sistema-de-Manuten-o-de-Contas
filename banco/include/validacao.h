#ifndef VALIDACAO_H
#define VALIDACAO_H

#include <stddef.h>

int    val_ler_int(const char *prompt);

double val_ler_double(const char *prompt);

int    val_ler_string(const char *prompt, char *buf, size_t max);

int    val_numero_conta(int numero);

int    val_saldo(double saldo);

int    val_saldo(double saldo);

#endif