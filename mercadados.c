#include <stdio.h>
#include <stdlib.h>
#include "mercadados.h"
#include "interface.h"

NodoLista* criarNodoLista(char* nome) {
    NodoLista* novo = (NodoLista*)malloc(sizeof(NodoLista));
    strcpy(novo->nome, nome);
    novo->anterior = NULL;
    novo->proximo = NULL;
    return novo;
}

NodoArvore* criarNodoArvore(Dados* dados) {
    NodoArvore* novo = (NodoArvore*)malloc(sizeof(NodoArvore));
    strcpy(novo->cpf, dados->cpf);
    novo->dados = dados;
    novo->esquerda = NULL;
    novo->direita = NULL;
    return novo;
}

Dados* criarDados(char* cpf, char* nome, char* endereco, char* telefone, char* email) {
    Dados* dados = (Dados*)malloc(sizeof(Dados));
    strcpy(dados->cpf, cpf);
    strcpy(dados->nome, nome);
    strcpy(dados->endereco, endereco);
    strcpy(dados->telefone, telefone);
    strcpy(dados->email, email);
    return dados;
}

NodoArvore* inserirNaArvore(NodoArvore* raiz, Dados* dados) {
    if (raiz == NULL) {
        return criarNodoArvore(dados);
    }

    if (strcmp(dados->cpf, raiz->cpf) < 0) {
        raiz->esquerda = inserirNaArvore(raiz->esquerda, dados);
    } else {
        raiz->direita = inserirNaArvore(raiz->direita, dados);
    }

    return raiz;
}

NodoLista* inserirNaLista(NodoLista* head, char* nome) {
    NodoLista* novo = criarNodoLista(nome);

    if (head == NULL) {
        return novo;
    }

    NodoLista* atual = head;
    NodoLista* anterior = NULL;

    while (atual != NULL && strcmp(nome, atual->nome) > 0) {
        anterior = atual;
        atual = atual->proximo;
    }

    if (anterior == NULL) {
        novo->proximo = head;
        head->anterior = novo;
        return novo;
    }

    novo->proximo = atual;
    novo->anterior = anterior;
    anterior->proximo = novo;

    if (atual != NULL) {
        atual->anterior = novo;
    }

    return head;
}

void inserirDados(NodoLista** lista, NodoArvore** arvore, NodoListaDinamica** listaDados, Dados* dados) {
    *lista = inserirNaLista(*lista, dados->nome);
    *arvore = inserirNaArvore(*arvore, dados);
}

NodoListaDinamica* criarNodoListaDinamica(Dados* dados) {
    NodoListaDinamica* novo = (NodoListaDinamica*)malloc(sizeof(NodoListaDinamica));
    novo->dados = dados;
    novo->proximo = NULL;
    return novo;
}

NodoListaDinamica* inserirNaListaDinamica(NodoListaDinamica* head, Dados* dados) {
    NodoListaDinamica* novo = criarNodoListaDinamica(dados);

    if (head == NULL) {
        return novo;
    }

    NodoListaDinamica* atual = head;
    while (atual->proximo != NULL) {
        atual = atual->proximo;
    }
    atual->proximo = novo;

    return head;
}

void imprimirListaDinamicaOrdenada(NodoLista* listaNomes, NodoListaDinamica* listaDados) {
    NodoLista* nodoNomeAtual = listaNomes;

    while (nodoNomeAtual != NULL) {
        NodoListaDinamica* nodoDadosAtual = listaDados;

        while (nodoDadosAtual != NULL) {
            Dados* dados = nodoDadosAtual->dados;

            if (strcmp(nodoNomeAtual->nome, dados->nome) == 0) {
                printf(UNDERLINE "CPF: %s\n Nome: %s\n Endereco: %s\n Telefone: %s\n Email: %s\n" RESET,
                       dados->cpf, dados->nome, dados->endereco, dados->telefone, dados->email);
                break;
            }

            nodoDadosAtual = nodoDadosAtual->proximo;
        }

        nodoNomeAtual = nodoNomeAtual->proximo;
    }
}

void adicionarRegistro(NodoLista** listaNomes, NodoArvore** arvoreCPFs, NodoListaDinamica** listaDados) {
    char cpf[12];
    printf("Digite o CPF: ");
    scanf("%s", cpf);

    NodoArvore* nodo = buscarNaArvore(*arvoreCPFs, cpf);
    if (nodo != NULL) {
        printf(RED "CPF ja cadastrado.\n" RESET);
        return;
    }

    while (getchar() != '\n');

    char nome[100], endereco[200], telefone[15], email[100];

    printf("Digite o Nome: ");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = '\0'; // remover o caractere de nova linha

    printf("Digite o Endereco: ");
    fgets(endereco, sizeof(endereco), stdin);
    endereco[strcspn(endereco, "\n")] = '\0'; // remover o caractere de nova linha

    printf("Digite o Telefone: ");
    fgets(telefone, sizeof(telefone), stdin);
    telefone[strcspn(telefone, "\n")] = '\0'; // remover o caractere de nova linha

    printf("Digite o Email: ");
    fgets(email, sizeof(email), stdin);
    email[strcspn(email, "\n")] = '\0'; // remover o caractere de nova linha

    Dados* dados = criarDados(cpf, nome, endereco, telefone, email);

    *listaNomes = inserirNaLista(*listaNomes, nome);
    *arvoreCPFs = inserirNaArvore(*arvoreCPFs, dados);
    *listaDados = inserirNaListaDinamica(*listaDados, dados);

    printf(GREEN "Registro adicionado com sucesso.\n" RESET);
}

void alterarRegistro(NodoLista** listaNomes, NodoArvore* arvoreCPFs, NodoListaDinamica* listaDados) {
    char cpf[12];
    printf("Digite o CPF: ");
    scanf("%s", cpf);

    NodoArvore* nodo = buscarNaArvore(arvoreCPFs, cpf);
    if (nodo == NULL) {
        printf(RED "CPF nao cadastrado.\n" RESET);
        return;
    }

    while (getchar() != '\n');

    char nome[100], endereco[200], telefone[15], email[100];

    printf("Insira os novos valores ou apenas aperte ENTER:\n");

    printf("Nome: %s \nNovo valor: ", nodo->dados->nome);
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = '\0'; // remover o caractere de nova linha
    if (strlen(nome) > 0) {
        strcpy(nodo->dados->nome, nome);
    }

    printf("Endereco: %s \nNovo valor: ", nodo->dados->endereco);
    fgets(endereco, sizeof(endereco), stdin);
    endereco[strcspn(endereco, "\n")] = '\0'; // remover o caractere de nova linha
    if (strlen(endereco) > 0) {
        strcpy(nodo->dados->endereco, endereco);
    }

    printf("Telefone: %s \nNovo valor: ", nodo->dados->telefone);
    fgets(telefone, sizeof(telefone), stdin);
    telefone[strcspn(telefone, "\n")] = '\0'; // remover o caractere de nova linha
    if (strlen(telefone) > 0) {
        strcpy(nodo->dados->telefone, telefone);
    }

    printf("Email: %s \nNovo valor: ", nodo->dados->email);
    fgets(email, sizeof(email), stdin);
    email[strcspn(email, "\n")] = '\0'; // remover o caractere de nova linha
    if (strlen(email) > 0) {
        strcpy(nodo->dados->email, email);
    }

    NodoLista* nodoNomeAtual = *listaNomes;
    while (nodoNomeAtual != NULL) {
        if (strcmp(nodoNomeAtual->nome, nodo->dados->nome) == 0) {
            // Atualizar o nome se ele foi modificado
            if (strlen(nome) > 0) {
                strcpy(nodoNomeAtual->nome, nome);
            }
            break;
        }
        nodoNomeAtual = nodoNomeAtual->proximo;
    }

    printf(GREEN "Registro alterado com sucesso.\n" RESET);
}


void excluirRegistro(NodoLista** listaNomes, NodoArvore** arvoreCPFs, NodoListaDinamica* listaDados) {
    char cpf[12];
    printf("Digite o CPF: ");
    scanf("%s", cpf);

    NodoArvore* nodo = buscarNaArvore(*arvoreCPFs, cpf);
    if (nodo == NULL) {
        printf(RED "CPF nao cadastrado.\n" RESET);
        return;
    }

    *arvoreCPFs = removerDaArvore(*arvoreCPFs, cpf);

    NodoLista* nodoNomeAtual = *listaNomes;
    NodoLista* anterior = NULL;
    while (nodoNomeAtual != NULL) {
        if (strcmp(nodoNomeAtual->nome, nodo->dados->nome) == 0) {
            if (anterior == NULL) {
                *listaNomes = nodoNomeAtual->proximo;
            } else {
                anterior->proximo = nodoNomeAtual->proximo;
            }
            if (nodoNomeAtual->proximo != NULL) {
                nodoNomeAtual->proximo->anterior = anterior;
            }
            free(nodoNomeAtual);
            break;
        }
        anterior = nodoNomeAtual;
        nodoNomeAtual = nodoNomeAtual->proximo;
    }

    printf(GREEN "Registro excluido com sucesso.\n" RESET);
}

void procurarRegistro(NodoArvore* arvoreCPFs) {
    char cpf[12];
    printf("Digite o CPF: ");
    scanf("%s", cpf);

    NodoArvore* nodo = buscarNaArvore(arvoreCPFs, cpf);
    if (nodo == NULL) {
        printf(RED "CPF nao cadastrado.\n" RESET);
        return;
    }

    Dados* dados = nodo->dados;
    printf(UNDERLINE "CPF: %s\n Nome: %s\n Endereco: %s\n Telefone: %s\n Email: %s\n" RESET,
           dados->cpf, dados->nome, dados->endereco, dados->telefone, dados->email);
}

NodoArvore* buscarNaArvore(NodoArvore* raiz, char* cpf) {
    if (raiz == NULL) {
        return NULL;
    }

    int cmp = strcmp(cpf, raiz->cpf);
    if (cmp == 0) {
        return raiz;
    } else if (cmp < 0) {
        return buscarNaArvore(raiz->esquerda, cpf);
    } else {
        return buscarNaArvore(raiz->direita, cpf);
    }
}

NodoArvore* removerDaArvore(NodoArvore* raiz, char* cpf) {
    if (raiz == NULL) {
        return NULL;
    }

    int cmp = strcmp(cpf, raiz->cpf);
    if (cmp < 0) {
        raiz->esquerda = removerDaArvore(raiz->esquerda, cpf);
    } else if (cmp > 0) {
        raiz->direita = removerDaArvore(raiz->direita, cpf);
    } else {
        if (raiz->esquerda == NULL) {
            NodoArvore* temp = raiz->direita;
            free(raiz);
            return temp;
        } else if (raiz->direita == NULL) {
            NodoArvore* temp = raiz->esquerda;
            free(raiz);
            return temp;
        }

        NodoArvore* temp = raiz->direita;
        while (temp && temp->esquerda != NULL) {
            temp = temp->esquerda;
        }

        strcpy(raiz->cpf, temp->cpf);
        raiz->dados = temp->dados;
        raiz->direita = removerDaArvore(raiz->direita, temp->cpf);
    }

    return raiz;
}
