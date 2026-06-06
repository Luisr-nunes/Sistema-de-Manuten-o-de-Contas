#ifndef CLIENTE_H
#define CLIENTE_H

#define MAX_NOME 50
#define MAX_CPF 15

typedef struct {
    int id;
    char nome[MAX_NOME];
    char cpf[MAX_CPF];
    double saldo;
    int ativo; // 1 = ativo, 0 = inativo (exclusao logica)
} Cliente;

#endif // CLIENTE_H
