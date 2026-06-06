#ifndef DB_H
#define DB_H

#include "cliente.h"

typedef enum {
    DB_OK            =  0,
    DB_ERR_ARQUIVO   = -1,  
    DB_ERR_IO        = -2,  
    DB_ERR_NAO_FOUND = -3,  
    DB_ERR_DUPLICADO = -4,   
    DB_ERR_SALDO     = -5    
} DbStatus;

int db_total_registros(void);

DbStatus db_buscar(int numero, Cliente *out, int *pos_out);

DbStatus db_inserir(const Cliente *c);

DbStatus db_atualizar(int pos, const Cliente *c);


typedef void (*db_iter_fn)(const Cliente *c, int pos, void *userdata);
int db_listar(db_iter_fn callback, void *userdata);

#endif 