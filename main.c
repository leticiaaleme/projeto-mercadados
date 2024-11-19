#include <stdio.h>
#include "mercadados.h"
#include "interface.h"

enum {
    OP_NAO_SELECIONADA = 0,
    OP_ADICIONAR,
    OP_ALTERAR,
    OP_EXCLUIR,
    OP_PROCURAR,
    OP_RELATORIO,
    OP_SAIR
};

void menu() {
    printf("\n");
    printf(BCYAN BOLD"\t\t==========================\n");
    printf("\t\t||" BWHITE BOLD "\tMercadaDos" RESET BCYAN BOLD "\t||\n");
    printf("\t\t==========================\n\n" RESET);
    printf(BCYAN BOLD "==========================================\n");
    printf("||" BWHITE BOLD "Bem vindo ao seu registro de entregas." RESET BCYAN BOLD "||\n");
    printf("||" BWHITE "Selecione a sua opcao:" RESET BCYAN BOLD "\t\t||\n");
    printf("==========================================" RESET "\n\n");
    printf("1. Adicionar\n");
    printf("2. Alterar\n");
    printf("3. Excluir\n");
    printf("4. Procurar\n");
    printf("5. Relatorio\n");
    printf("6. Sair\n");
    printf(BOLD "Opcao: " RESET);
}

int main() {
    NodoLista* listaNomes = NULL;
    NodoArvore* arvoreCPFs = NULL;
    NodoListaDinamica* listaDados = NULL;
    int opcao = OP_NAO_SELECIONADA;

    while (opcao != OP_SAIR) {
        menu();
        scanf("%d", &opcao);

        switch (opcao) {
            case OP_ADICIONAR:
                adicionarRegistro(&listaNomes, &arvoreCPFs, &listaDados);
                break;
            case OP_ALTERAR:
                alterarRegistro(&listaNomes, arvoreCPFs, listaDados);
                break;
            case OP_EXCLUIR:
                excluirRegistro(&listaNomes, &arvoreCPFs, listaDados);
                break;
            case OP_PROCURAR:
                procurarRegistro(arvoreCPFs);
                break;
            case OP_RELATORIO:
                printf(YELLOW "Relatorio: \n" RESET);
                imprimirListaDinamicaOrdenada(listaNomes, listaDados);
                break;
            case OP_SAIR:
                printf("Saindo...\n");
                break;
            default:
                printf(RED "Opcao invalida. Por favor, tente novamente.\n" RESET);
        }
    }

    return 0;
}
