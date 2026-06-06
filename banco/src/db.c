#include <stdio.h>
#include <string.h>
 
#include "../include/cliente.h"
#include "../include/db.h"
 
 
static FILE *db_abrir(void) {
    FILE *fp = fopen(ARQUIVO_BANCO, "r+b");
    if (!fp) {
       
        fp = fopen(ARQUIVO_BANCO, "w+b");
    }
    return fp;   
}
 
static long offset_de(int pos) {
    return (long)(pos) * (long)sizeof(Cliente);
}
  
int db_total_registros(void) {
    FILE *fp = db_abrir();
    if (!fp) return 0;
 
    fseek(fp, 0L, SEEK_END);
    long tamanho = ftell(fp);
    fclose(fp);
 
    return (int)(tamanho / (long)sizeof(Cliente));
}
 
DbStatus db_buscar(int numero, Cliente *out, int *pos_out) {
    FILE *fp = db_abrir();
    if (!fp) return DB_ERR_ARQUIVO;
 
    int total = db_total_registros();
    Cliente tmp;
 
    for (int i = 0; i < total; i++) {
        fseek(fp, offset_de(i), SEEK_SET);
        if (fread(&tmp, sizeof(Cliente), 1, fp) != 1) continue;
        if (tmp.ativo == CONTA_ATIVA && tmp.numero == numero) {
            if (out)     *out     = tmp;
            if (pos_out) *pos_out = i;
            fclose(fp);
            return DB_OK;
        }
    }
 
    fclose(fp);
    return DB_ERR_NAO_FOUND;
}
 
DbStatus db_inserir(const Cliente *c) {
    if (db_buscar(c->numero, NULL, NULL) == DB_OK)
        return DB_ERR_DUPLICADO;
 
    FILE *fp = db_abrir();
    if (!fp) return DB_ERR_ARQUIVO;
 
    int   total = db_total_registros();
    int   slot  = total;   
    Cliente tmp;
 
    for (int i = 0; i < total; i++) {
        fseek(fp, offset_de(i), SEEK_SET);
        if (fread(&tmp, sizeof(Cliente), 1, fp) == 1 && tmp.ativo == CONTA_INATIVA) {
            slot = i;
            break;
        }
    }
 
    fseek(fp, offset_de(slot), SEEK_SET);
    DbStatus status = (fwrite(c, sizeof(Cliente), 1, fp) == 1) ? DB_OK : DB_ERR_IO;
 
    fclose(fp);
    return status;
}
 
DbStatus db_atualizar(int pos, const Cliente *c) {
    FILE *fp = db_abrir();
    if (!fp) return DB_ERR_ARQUIVO;
 
    fseek(fp, offset_de(pos), SEEK_SET);
    DbStatus status = (fwrite(c, sizeof(Cliente), 1, fp) == 1) ? DB_OK : DB_ERR_IO;
 
    fclose(fp);
    return status;
}
 
int db_listar(db_iter_fn callback, void *userdata) {
    FILE *fp = db_abrir();
    if (!fp) return DB_ERR_ARQUIVO;
 
    int total = db_total_registros();
 
    rewind(fp);
 
    Cliente c;
    int encontrados = 0;
 
    for (int i = 0; i < total; i++) {
        if (fread(&c, sizeof(Cliente), 1, fp) != 1) break;
        if (c.ativo == CONTA_ATIVA) {
            if (callback) callback(&c, i, userdata);
            encontrados++;
        }
    }
 
    fclose(fp);
    return encontrados;
}
 
