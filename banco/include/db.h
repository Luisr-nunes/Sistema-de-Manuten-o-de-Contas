#ifndef DB_H
#define DB_H

#include "cliente.h"

typedef enum {
    DB_OK,
    DB_ERRO_ABERTURA,
    DB_ERRO_ESCRITA,
    DB_ERRO_LEITURA,
    DB_NAO_ENCONTRADO,
    DB_DUPLICADO
} DbStatus;

// Inicializa o arquivo do banco de dados se necessario
DbStatus db_inicializar(void);

// Insere um novo cliente (evitando duplicados e reutilizando espaco deletado logico se possivel)
DbStatus db_inserir(const Cliente *cliente);

// Busca um cliente pelo ID
DbStatus db_buscar(int id, Cliente *cliente);

// Atualiza os dados de um cliente existente
DbStatus db_atualizar(const Cliente *cliente);

// Realiza a exclusao logica (ativo = 0) do cliente
DbStatus db_excluir(int id);

// Carrega todos os clientes ativos em um array
DbStatus db_listar(Cliente *clientes, int max_clientes, int *total_retornado);

#endif // DB_H
