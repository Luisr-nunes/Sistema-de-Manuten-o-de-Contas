#include "menu.h"
#include "db.h"
#include "validacao.h"
#include <stdio.h>
#include <string.h>

static void exibir_status(DbStatus status) {
    switch (status) {
        case DB_OK:
            printf("\n>>> [SUCESSO] Operacao realizada com sucesso!\n");
            break;
        case DB_ERRO_ABERTURA:
            printf("\n>>> [ERRO CRITICO] Falha ao abrir o banco de dados. Verifique permissoes.\n");
            break;
        case DB_ERRO_ESCRITA:
            printf("\n>>> [ERRO CRITICO] Falha ao gravar dados no disco.\n");
            break;
        case DB_ERRO_LEITURA:
            printf("\n>>> [ERRO CRITICO] Falha ao ler dados do disco.\n");
            break;
        case DB_NAO_ENCONTRADO:
            printf("\n>>> [ERRO] Cliente nao encontrado.\n");
            break;
        case DB_DUPLICADO:
            printf("\n>>> [ERRO] ID ja cadastrado no sistema.\n");
            break;
        default:
            printf("\n>>> [ERRO] Ocorreu um erro desconhecido.\n");
            break;
    }
}

static void cadastrar_cliente(void) {
    Cliente c;
    printf("\n--- Cadastrar Novo Cliente ---\n");
    c.id = ler_inteiro("Digite o ID (1 - 999999): ", 1, 999999);

    // Verifica duplicado preliminarmente antes de pedir demais informacoes
    Cliente temp;
    DbStatus status = db_buscar(c.id, &temp);
    if (status == DB_OK) {
        exibir_status(DB_DUPLICADO);
        return;
    }

    ler_string("Digite o Nome do Cliente: ", c.nome, MAX_NOME);
    if (strlen(c.nome) == 0) {
        strcpy(c.nome, "Sem Nome");
    }

    ler_string("Digite o CPF (apenas numeros ou formatado): ", c.cpf, MAX_CPF);
    if (strlen(c.cpf) == 0) {
        strcpy(c.cpf, "000.000.000-00");
    }

    c.saldo = ler_double("Digite o Saldo Inicial: ", 0.0, 999999999.99);
    c.ativo = 1; // Registro ativo

    status = db_inserir(&c);
    exibir_status(status);
}

static void consultar_cliente(void) {
    printf("\n--- Consultar Cliente ---\n");
    int id = ler_inteiro("Digite o ID a buscar: ", 1, 999999);

    Cliente c;
    DbStatus status = db_buscar(id, &c);
    if (status == DB_OK) {
        printf("\n==================================================\n");
        printf(" ID:      %d\n", c.id);
        printf(" Nome:    %s\n", c.nome);
        printf(" CPF:     %s\n", c.cpf);
        printf(" Saldo:   R$ %.2f\n", c.saldo);
        printf("==================================================\n");
    } else {
        exibir_status(status);
    }
}

static void atualizar_cliente(void) {
    printf("\n--- Atualizar Cliente ---\n");
    int id = ler_inteiro("Digite o ID do cliente a atualizar: ", 1, 999999);

    Cliente c;
    DbStatus status = db_buscar(id, &c);
    if (status != DB_OK) {
        exibir_status(status);
        return;
    }

    printf("\nCliente encontrado:\n");
    printf("Nome atual: %s | CPF atual: %s | Saldo atual: R$ %.2f\n\n", c.nome, c.cpf, c.saldo);

    char novo_nome[MAX_NOME];
    ler_string("Digite o Novo Nome (deixe em branco para manter): ", novo_nome, MAX_NOME);
    if (strlen(novo_nome) > 0) {
        strcpy(c.nome, novo_nome);
    }

    char novo_cpf[MAX_CPF];
    ler_string("Digite o Novo CPF (deixe em branco para manter): ", novo_cpf, MAX_CPF);
    if (strlen(novo_cpf) > 0) {
        strcpy(c.cpf, novo_cpf);
    }

    printf("Deseja alterar o saldo? (1 - Sim, 0 - Nao)\n");
    int alterar_saldo = ler_inteiro("Opcao: ", 0, 1);
    if (alterar_saldo) {
        c.saldo = ler_double("Digite o Novo Saldo: ", 0.0, 999999999.99);
    }

    status = db_atualizar(&c);
    exibir_status(status);
}

static void excluir_cliente(void) {
    printf("\n--- Excluir Cliente ---\n");
    int id = ler_inteiro("Digite o ID a excluir: ", 1, 999999);

    Cliente c;
    DbStatus status = db_buscar(id, &c);
    if (status != DB_OK) {
        exibir_status(status);
        return;
    }

    printf("\nCliente encontrado: %s (ID: %d)\n", c.nome, c.id);
    printf("Confirmar exclusao? (1 - Sim, 0 - Nao)\n");
    int confirmar = ler_inteiro("Opcao: ", 0, 1);
    if (confirmar) {
        status = db_excluir(id);
        exibir_status(status);
    } else {
        printf("\nExclusao cancelada.\n");
    }
}

static void listar_clientes(void) {
    printf("\n--- Lista de Clientes ---\n");
    Cliente lista[128];
    int total = 0;
    DbStatus status = db_listar(lista, 128, &total);

    if (status != DB_OK) {
        exibir_status(status);
        return;
    }

    if (total == 0) {
        printf("\nNenhum cliente cadastrado no momento.\n");
        return;
    }

    printf("\n%-6s | %-25s | %-15s | %-12s\n", "ID", "Nome", "CPF", "Saldo");
    printf("-------------------------------------------------------------------\n");
    for (int i = 0; i < total; i++) {
        printf("%-6d | %-25.25s | %-15s | R$ %-10.2f\n", 
               lista[i].id, lista[i].nome, lista[i].cpf, lista[i].saldo);
    }
    printf("-------------------------------------------------------------------\n");
    printf("Total de registros ativos: %d\n", total);
}

void menu_executar(void) {
    // Inicializa o BD
    db_inicializar();

    int opcao = -1;
    while (opcao != 0) {
        printf("\n========================================\n");
        printf("    SISTEMA DE MANUTENCAO DE CONTAS     \n");
        printf("========================================\n");
        printf("  1. Cadastrar Cliente\n");
        printf("  2. Consultar Cliente por ID\n");
        printf("  3. Atualizar Cliente\n");
        printf("  4. Excluir Cliente\n");
        printf("  5. Listar Todos os Clientes\n");
        printf("  0. Sair\n");
        printf("========================================\n");
        opcao = ler_inteiro("Escolha uma opcao: ", 0, 5);

        switch (opcao) {
            case 1:
                cadastrar_cliente();
                break;
            case 2:
                consultar_cliente();
                break;
            case 3:
                atualizar_cliente();
                break;
            case 4:
                excluir_cliente();
                break;
            case 5:
                listar_clientes();
                break;
            case 0:
                printf("\nSaindo do sistema. Ate breve!\n");
                break;
        }
    }
}
