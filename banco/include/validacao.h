#ifndef VALIDACAO_H
#define VALIDACAO_H

// Interface de validacao de inputs.
// Nao contem dependencias de db.h ou menu.h ("nao conhece db nem menu").

// Le uma string ate max_tamanho - 1, descartando caracteres excedentes no buffer de entrada.
void ler_string(const char *mensagem, char *destino, int max_tamanho);

// Le um numero inteiro no intervalo fechado [min_val, max_val] com validacao.
int ler_inteiro(const char *mensagem, int min_val, int max_val);

// Le um numero decimal double no intervalo fechado [min_val, max_val] com validacao.
double ler_double(const char *mensagem, double min_val, double max_val);

#endif // VALIDACAO_H
