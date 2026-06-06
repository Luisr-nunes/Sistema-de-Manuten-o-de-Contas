#include "db.h"
#include <stdio.h>
#include <string.h>

static const char *DB_ARQUIVO = "contas.dat";

DbStatus db_inicializar(void) {
    // Abre no modo "ab" para criar o arquivo caso nao exista
    FILE *f = fopen(DB_ARQUIVO, "ab");
    if (f == NULL) {
        return DB_ERRO_ABERTURA;
    }
    fclose(f);
    return DB_OK;
}

DbStatus db_inserir(const Cliente *cliente) {
    // Abre para leitura e escrita binaria.
    FILE *f = fopen(DB_ARQUIVO, "r+b");
    if (f == NULL) {
        // Se falhar, tenta "wb" (caso o arquivo nao exista por algum motivo) e depois reabre "r+b"
        f = fopen(DB_ARQUIVO, "wb");
        if (f == NULL) return DB_ERRO_ABERTURA;
        fclose(f);
        f = fopen(DB_ARQUIVO, "r+b");
        if (f == NULL) return DB_ERRO_ABERTURA;
    }

    Cliente temp;
    long slot_inativo_offset = -1;

    // Garante que o ID nao seja duplicado e aproveita o primeiro espaco inativo encontrado
    rewind(f);
    while (fread(&temp, sizeof(Cliente), 1, f) == 1) {
        if (temp.ativo) {
            if (temp.id == cliente->id) {
                fclose(f);
                return DB_DUPLICADO;
            }
        } else {
            if (slot_inativo_offset == -1) {
                slot_inativo_offset = ftell(f) - sizeof(Cliente);
            }
        }
    }

    // Posiciona o ponteiro de arquivo
    if (slot_inativo_offset != -1) {
        // Reutiliza slot inativo
        if (fseek(f, slot_inativo_offset, SEEK_SET) != 0) {
            fclose(f);
            return DB_ERRO_ESCRITA;
        }
    } else {
        // Vai para o final do arquivo
        if (fseek(f, 0, SEEK_END) != 0) {
            fclose(f);
            return DB_ERRO_ESCRITA;
        }
    }

    // Escreve o novo registro
    if (fwrite(cliente, sizeof(Cliente), 1, f) != 1) {
        fclose(f);
        return DB_ERRO_ESCRITA;
    }

    fclose(f);
    return DB_OK;
}

DbStatus db_buscar(int id, Cliente *cliente) {
    FILE *f = fopen(DB_ARQUIVO, "rb");
    if (f == NULL) {
        return DB_ERRO_ABERTURA;
    }

    Cliente temp;
    int encontrado = 0;
    while (fread(&temp, sizeof(Cliente), 1, f) == 1) {
        if (temp.ativo && temp.id == id) {
            *cliente = temp;
            encontrado = 1;
            break;
        }
    }

    fclose(f);
    return encontrado ? DB_OK : DB_NAO_ENCONTRADO;
}

DbStatus db_atualizar(const Cliente *cliente) {
    FILE *f = fopen(DB_ARQUIVO, "r+b");
    if (f == NULL) {
        return DB_ERRO_ABERTURA;
    }

    Cliente temp;
    long offset = -1;
    while (fread(&temp, sizeof(Cliente), 1, f) == 1) {
        if (temp.ativo && temp.id == cliente->id) {
            offset = ftell(f) - sizeof(Cliente);
            break;
        }
    }

    if (offset == -1) {
        fclose(f);
        return DB_NAO_ENCONTRADO;
    }

    // Vai para a posicao do registro
    if (fseek(f, offset, SEEK_SET) != 0) {
        fclose(f);
        return DB_ERRO_ESCRITA;
    }

    // Sobreescreve o registro
    if (fwrite(cliente, sizeof(Cliente), 1, f) != 1) {
        fclose(f);
        return DB_ERRO_ESCRITA;
    }

    fclose(f);
    return DB_OK;
}

DbStatus db_excluir(int id) {
    FILE *f = fopen(DB_ARQUIVO, "r+b");
    if (f == NULL) {
        return DB_ERRO_ABERTURA;
    }

    Cliente temp;
    long offset = -1;
    while (fread(&temp, sizeof(Cliente), 1, f) == 1) {
        if (temp.ativo && temp.id == id) {
            offset = ftell(f) - sizeof(Cliente);
            temp.ativo = 0; // Marca como inativo (exclusao logica)
            break;
        }
    }

    if (offset == -1) {
        fclose(f);
        return DB_NAO_ENCONTRADO;
    }

    // Vai para a posicao do registro
    if (fseek(f, offset, SEEK_SET) != 0) {
        fclose(f);
        return DB_ERRO_ESCRITA;
    }

    // Sobreescreve com o registro inativado
    if (fwrite(&temp, sizeof(Cliente), 1, f) != 1) {
        fclose(f);
        return DB_ERRO_ESCRITA;
    }

    fclose(f);
    return DB_OK;
}

DbStatus db_listar(Cliente *clientes, int max_clientes, int *total_retornado) {
    FILE *f = fopen(DB_ARQUIVO, "rb");
    if (f == NULL) {
        return DB_ERRO_ABERTURA;
    }

    Cliente temp;
    int contador = 0;
    while (fread(&temp, sizeof(Cliente), 1, f) == 1) {
        if (temp.ativo) {
            if (contador < max_clientes) {
                clientes[contador] = temp;
                contador++;
            } else {
                break;
            }
        }
    }

    *total_retornado = contador;
    fclose(f);
    return DB_OK;
}
