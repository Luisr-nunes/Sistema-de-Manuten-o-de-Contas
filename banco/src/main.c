#include <stdio.h>
#include <stdlib.h>
 
#include "../include/menu.h"
 
int main(void) {
    puts("╔══════════════════════════════════════════╗");
    puts("║   SISTEMA DE MANUTENÇÃO DE CONTAS        ║");
    puts("║   Arquivo binário  ·  Registros fixos    ║");
    puts("╚══════════════════════════════════════════╝");
 
    menu_executar();
 
    return EXIT_SUCCESS;
}