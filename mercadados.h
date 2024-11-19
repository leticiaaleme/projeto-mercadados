#ifndef BANCO_DE_DADOS_H
#define BANCO_DE_DADOS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Dados {
    char cpf[12];
    char nome[100];
    char endereco[200];
    char telefone[15];
    char email[100];
} Dados;

typedef struct NodoLista {
    char nome[100];
    struct NodoLista *anterior;
    struct NodoLista *proximo;
} NodoLista;

typedef struct NodoArvore {
    char cpf[12];
    Dados *dados;
    struct NodoArvore *esquerda;
    struct NodoArvore *direita;
} NodoArvore;

typedef struct NodoListaDinamica {
    Dados* dados;
    struct NodoListaDinamica* proximo;
} NodoListaDinamica;

NodoLista* criarNodoLista(char* nome);
NodoArvore* criarNodoArvore(Dados* dados);
Dados* criarDados(char* cpf, char* nome, char* endereco, char* telefone, char* email);
NodoListaDinamica* criarNodoListaDinamica(Dados* dados);

NodoArvore* inserirNaArvore(NodoArvore* raiz, Dados* dados);
NodoLista* inserirNaLista(NodoLista* head, char* nome);
NodoListaDinamica* inserirNaListaDinamica(NodoListaDinamica* head, Dados* dados);
void inserirDados(NodoLista** lista, NodoArvore** arvore, NodoListaDinamica** listaDados, Dados* dados);

void imprimirListaDinamicaOrdenada(NodoLista* listaNomes, NodoListaDinamica* listaDados);

NodoArvore* buscarNaArvore(NodoArvore* raiz, char* cpf);
NodoArvore* removerDaArvore(NodoArvore* raiz, char* cpf);
void adicionarRegistro(NodoLista** listaNomes, NodoArvore** arvoreCPFs, NodoListaDinamica** listaDados);
void alterarRegistro(NodoLista** listaNomes, NodoArvore* arvoreCPFs, NodoListaDinamica* listaDados);
void excluirRegistro(NodoLista** listaNomes, NodoArvore** arvoreCPFs, NodoListaDinamica* listaDados);
void procurarRegistro(NodoArvore* arvoreCPFs);

#endif // BANCO_DE_DADOS_H
