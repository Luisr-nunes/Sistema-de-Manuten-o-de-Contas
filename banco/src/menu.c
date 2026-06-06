#include <stdio.h>
#include <string.h>

#include "../include/cliente.h"
#include "../include/db.h"
#include "../include/validacao.h"
#include "../include/menu.h"

static void imprimir_cliente(const Cliente *c, int pos, void *userdata) {
    (void)userdata;
    printf("  +-------------------------------------+\n");
    printf("  | Pos. arquivo : %d\n", pos);
    printf("  | Conta        : %d\n", c->numero);
    printf("  | Titular      : %s\n", c->nome);
    printf("  | Saldo        : R$ %.2f\n", c->saldo);
    printf("  +-------------------------------------+\n");
}

static void imprimir_status(DbStatus s) {
    switch (s) {
        case DB_OK:            break;
        case DB_ERR_ARQUIVO:   puts("  [ERRO] Nao foi possivel abrir o arquivo de dados."); break;
        case DB_ERR_IO:        puts("  [ERRO] Falha de leitura/escrita no arquivo.");        break;
        case DB_ERR_NAO_FOUND: puts("  [ERRO] Conta nao encontrada.");                       break;
        case DB_ERR_DUPLICADO: puts("  [ERRO] Ja existe uma conta com esse numero.");        break;
        case DB_ERR_SALDO:     puts("  [ERRO] Saldo insuficiente para esta operacao.");      break;
    }
}

static void op_cadastrar(void) {
    puts("\n=== CADASTRAR NOVO CLIENTE ===");

    int numero = val_ler_int("Numero da conta: ");
    if (!val_numero_conta(numero)) {
        puts("  [!] Numero de conta deve ser positivo."); return;
    }

    Cliente c;
    memset(&c, 0, sizeof(Cliente));
    c.numero = numero;
    c.ativo  = CONTA_ATIVA;

    val_ler_string("Nome do titular: ", c.nome, MAX_NOME);

    double saldo = val_ler_double("Saldo inicial (R$): ");
    if (!val_saldo(saldo)) {
        puts("  [!] Saldo inicial nao pode ser negativo."); return;
    }
    c.saldo = saldo;

    DbStatus s = db_inserir(&c);
    if (s == DB_OK)
        printf("  [OK] Cliente '%s' (conta %d) cadastrado.\n", c.nome, c.numero);
    else
        imprimir_status(s);
}

static void op_consultar(void) {
    puts("\n=== CONSULTAR CLIENTE ===");

    int numero = val_ler_int("Numero da conta: ");
    Cliente c;
    int pos;

    DbStatus s = db_buscar(numero, &c, &pos);
    if (s == DB_OK)
        imprimir_cliente(&c, pos, NULL);
    else
        imprimir_status(s);
}

static void op_atualizar_saldo(void) {
    puts("\n=== ATUALIZAR SALDO ===");

    int numero = val_ler_int("Numero da conta: ");
    Cliente c;
    int pos;

    DbStatus s = db_buscar(numero, &c, &pos);
    if (s != DB_OK) { imprimir_status(s); return; }

    printf("  Saldo atual: R$ %.2f\n", c.saldo);
    puts("  1 - Deposito   2 - Saque   3 - Definir valor exato");

    int op = val_ler_int("  Opcao: ");
    if (op < 1 || op > 3) { puts("  [!] Opcao invalida."); return; }

    double valor = val_ler_double("  Valor (R$): ");
    if (valor < 0) { puts("  [!] Valor nao pode ser negativo."); return; }

    switch (op) {
        case 1:
            c.saldo += valor;
            break;
        case 2:
            if (valor > c.saldo) { imprimir_status(DB_ERR_SALDO); return; }
            c.saldo -= valor;
            break;
        case 3:
            if (!val_saldo(valor)) { puts("  [!] Saldo invalido."); return; }
            c.saldo = valor;
            break;
    }

    s = db_atualizar(pos, &c);
    if (s == DB_OK)
        printf("  [OK] Saldo atualizado. Novo saldo: R$ %.2f\n", c.saldo);
    else
        imprimir_status(s);
}

static void op_encerrar_conta(void) {
    puts("\n=== ENCERRAR CONTA ===");

    int numero = val_ler_int("Numero da conta: ");
    Cliente c;
    int pos;

    DbStatus s = db_buscar(numero, &c, &pos);
    if (s != DB_OK) { imprimir_status(s); return; }

    imprimir_cliente(&c, pos, NULL);

    int conf = val_ler_int("  Confirmar encerramento? (1=Sim / 0=Nao): ");
    if (conf != 1) { puts("  Operacao cancelada."); return; }

    c.ativo = CONTA_INATIVA;
    s = db_atualizar(pos, &c);

    if (s == DB_OK)
        printf("  [OK] Conta %d encerrada.\n", numero);
    else
        imprimir_status(s);
}

static void op_listar(void) {
    puts("\n=== TODOS OS CLIENTES ===");

    int total = db_listar(imprimir_cliente, NULL);

    if (total == (int)DB_ERR_ARQUIVO)
        imprimir_status(DB_ERR_ARQUIVO);
    else if (total == 0)
        puts("  Nenhum cliente ativo encontrado.");
    else
        printf("\n  Total de clientes ativos: %d\n", total);
}

static void op_restaurar_leitura(void) {
    puts("\n=== RESTAURAR LEITURA (rewind) ===");
    puts("  Voltando ao inicio do arquivo e relendo todos os registros...\n");

    int total = db_listar(imprimir_cliente, NULL);

    if (total == (int)DB_ERR_ARQUIVO)
        imprimir_status(DB_ERR_ARQUIVO);
    else if (total == 0)
        puts("  Nenhum cliente ativo encontrado.");
    else
        printf("\n  [OK] Releitura concluida. Total: %d cliente(s).\n", total);
}

static void imprimir_menu(void) {
    puts("\n+==========================================+");
    puts("|        SISTEMA BANCARIO -- MENU          |");
    puts("+==========================================+");
    puts("|  1. Cadastrar novo cliente               |");
    puts("|  2. Consultar cliente                    |");
    puts("|  3. Atualizar saldo                      |");
    puts("|  4. Encerrar conta                       |");
    puts("|  5. Listar todos os clientes             |");
    puts("|  6. Restaurar leitura (rewind)           |");
    puts("|  7. Encerrar programa                    |");
    puts("+==========================================+");
}

void menu_executar(void) {
    int opcao;
    do {
        imprimir_menu();
        opcao = val_ler_int("Opcao: ");

        switch (opcao) {
            case 1: op_cadastrar();         break;
            case 2: op_consultar();         break;
            case 3: op_atualizar_saldo();   break;
            case 4: op_encerrar_conta();    break;
            case 5: op_listar();            break;
            case 6: op_restaurar_leitura(); break;
            case 7: puts("\nEncerrando o sistema. Ate logo!\n"); break;
            default: puts("  [!] Opcao invalida. Escolha entre 1 e 7.");
        }

    } while (opcao != 7);
}