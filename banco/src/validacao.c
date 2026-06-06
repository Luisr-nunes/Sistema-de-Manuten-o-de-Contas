#include "validacao.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void ler_string(const char *mensagem, char *destino, int max_tamanho) {
    printf("%s", mensagem);
    if (fgets(destino, max_tamanho, stdin) != NULL) {
        // Remove trailing newline character
        size_t len = strlen(destino);
        if (len > 0 && destino[len - 1] == '\n') {
            destino[len - 1] = '\0';
        } else {
            // Buffer did not contain a newline, which means there are extra characters in stdin.
            // Flush the remaining input in the current line of stdin.
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
        }
    } else {
        destino[0] = '\0';
    }
}

int ler_inteiro(const char *mensagem, int min_val, int max_val) {
    char buffer[128];
    int valor;
    char *endptr;

    while (1) {
        printf("%s", mensagem);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            continue;
        }

        // Remove trailing newline if present
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }

        // Convert string to integer
        valor = (int)strtol(buffer, &endptr, 10);

        // Verify if any digit was successfully read and if the entire string was consumed
        if (endptr == buffer || *endptr != '\0') {
            printf("Erro: Digite um numero inteiro valido.\n");
            continue;
        }

        // Check if value is within bounds
        if (valor < min_val || valor > max_val) {
            printf("Erro: O valor deve estar no intervalo [%d, %d].\n", min_val, max_val);
            continue;
        }

        return valor;
    }
}

double ler_double(const char *mensagem, double min_val, double max_val) {
    char buffer[128];
    double valor;
    char *endptr;

    while (1) {
        printf("%s", mensagem);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            continue;
        }

        // Remove trailing newline if present
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }

        // Convert string to double
        valor = strtod(buffer, &endptr);

        // Verify if any number was read and if the entire string was consumed
        if (endptr == buffer || *endptr != '\0') {
            printf("Erro: Digite um valor decimal valido.\n");
            continue;
        }

        // Check if value is within bounds
        if (valor < min_val || valor > max_val) {
            printf("Erro: O valor deve estar no intervalo [%.2f, %.2f].\n", min_val, max_val);
            continue;
        }

        return valor;
    }
}
