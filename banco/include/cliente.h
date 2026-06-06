#ifndef CLIENTE_H
#define CLIENTE_H

#define ARQUIVO_BANCO   "contas.dat"
#define MAX_NOME        50
#define CONTA_ATIVA     1
#define CONTA_INATIVA   0

typedef struct {
    int    numero;         
    char   nome[MAX_NOME]; 
    double saldo;            
    int    ativo;            
} Cliente;

#endif