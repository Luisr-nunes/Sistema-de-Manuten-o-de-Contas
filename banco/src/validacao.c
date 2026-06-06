#include <stdio.h>
#include <string.h>

#include "../include/validacao.h"


static void limpar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int val_ler_int(const char *prompt) {
    int valor;
    while (1) {
        printf("%s", prompt);
        if (scanf("%d", &valor) == 1) {
            limpar_buffer();
            return valor;
        }
        printf("  [!] Entrada inválida. Digite um número inteiro.\n");
        limpar_buffer();
    }
}

double val_ler_double(const char *prompt) {
    double valor;
    while (1) {
        printf("%s", prompt);
        if (scanf("%lf", &valor) == 1) {
            limpar_buffer();
            return valor;
        }
        printf("  [!] Entrada inválida. Digite um número.\n");
        limpar_buffer();
    }
}

int val_ler_string(const char *prompt, char *buf, size_t max) {
    while (1) {
        printf("%s", prompt);
        if (fgets(buf, (int)max, stdin)) {
            size_t len = strlen(buf);
            while (len > 0 && (buf[len-1] == '\n' || buf[len-1] == '\r'))
                buf[--len] = '\0';

            if (len > 0) return (int)len;
        }
        printf("  [!] Texto não pode ser vazio.\n");
    }
}

int val_numero_conta(int numero) {
    return numero > 0;
}

int val_saldo(double saldo) {
    return saldo >= 0.0;
}