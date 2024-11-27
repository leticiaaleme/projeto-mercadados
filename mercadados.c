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

    FILE* arquivo = fopen("relatorio.txt", "w");
    if (arquivo == NULL) {
        printf(RED "Erro ao criar o arquivo de relatorio.\n" RESET);
        return;
    }

    while (nodoNomeAtual != NULL) {
        NodoListaDinamica* nodoDadosAtual = listaDados;

        while (nodoDadosAtual != NULL) {
            Dados* dados = nodoDadosAtual->dados;

            if (strcmp(nodoNomeAtual->nome, dados->nome) == 0) {
                printf(UNDERLINE " CPF: %s\n Nome: %s\n Endereco: %s\n Telefone: %s\n Email: %s\n\n" RESET,
                       dados->cpf, dados->nome, dados->endereco, dados->telefone, dados->email);

                fprintf(arquivo, "CPF: %s\nNome: %s\nEndereco: %s\nTelefone: %s\nEmail: %s\n\n",
                        dados->cpf, dados->nome, dados->endereco, dados->telefone, dados->email);
                break;
            }

            nodoDadosAtual = nodoDadosAtual->proximo;
        }

        nodoNomeAtual = nodoNomeAtual->proximo;
    }

    fclose(arquivo);
    printf(GREEN "Relatorio gerado em 'relatorio.txt'.\n" RESET);
}


void adicionarRegistro(NodoLista** listaNomes, NodoArvore** arvoreCPFs, NodoListaDinamica** listaDados) {
    char cpf[12]; // Buffer de 12 caracteres para conter 11 dígitos + '\0'

    while (1) {
        printf("Digite o CPF (somente números): ");
        scanf("%s", cpf);

        // Verifica se o CPF tem exatamente 11 caracteres e apenas dígitos numéricos
        if (strlen(cpf) != 11 || strspn(cpf, "0123456789") != 11) {
            printf(RED "CPF invalido. Tente novamente.\n" RESET);
        } else {
            break; // CPF válido
        }
    }

    NodoArvore* nodo = buscarNaArvore(*arvoreCPFs, cpf);
    if (nodo != NULL) {
        printf(RED "CPF ja cadastrado.\n" RESET);
        return;
    }

    // Limpa o buffer de entrada
    while (getchar() != '\n');

    char nome[100], endereco[200], telefone[15], email[100];

    printf("Digite o Nome: ");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = '\0';

    printf("Digite o Endereco: ");
    fgets(endereco, sizeof(endereco), stdin);
    endereco[strcspn(endereco, "\n")] = '\0';

    printf("Digite o Telefone: ");
    fgets(telefone, sizeof(telefone), stdin);
    telefone[strcspn(telefone, "\n")] = '\0';

    printf("Digite o Email: ");
    fgets(email, sizeof(email), stdin);
    email[strcspn(email, "\n")] = '\0';

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

    while (getchar() != '\n'); // Limpar o buffer de entrada

    // Variáveis temporárias para novas informações
    char novoNome[100], novoEndereco[200], novoTelefone[15], novoEmail[100];
    strcpy(novoNome, nodo->dados->nome);
    strcpy(novoEndereco, nodo->dados->endereco);
    strcpy(novoTelefone, nodo->dados->telefone);
    strcpy(novoEmail, nodo->dados->email);

    printf("Insira os novos valores ou apenas aperte ENTER:\n");

    printf("Nome: %s \nNovo nome: ", nodo->dados->nome);
    fgets(novoNome, sizeof(novoNome), stdin);
    novoNome[strcspn(novoNome, "\n")] = '\0'; // Remover '\n'

    printf("Endereco: %s \nNovo endereco: ", nodo->dados->endereco);
    fgets(novoEndereco, sizeof(novoEndereco), stdin);
    novoEndereco[strcspn(novoEndereco, "\n")] = '\0';

    printf("Telefone: %s \nNovo telefone: ", nodo->dados->telefone);
    fgets(novoTelefone, sizeof(novoTelefone), stdin);
    novoTelefone[strcspn(novoTelefone, "\n")] = '\0';

    printf("Email: %s \nNovo email: ", nodo->dados->email);
    fgets(novoEmail, sizeof(novoEmail), stdin);
    novoEmail[strcspn(novoEmail, "\n")] = '\0';

    int confirmar;
    printf(BOLD YELLOW "Confirma Alteracoes? (1 - Sim, 2 - Nao): " RESET);
    scanf("%d", &confirmar);

    if (confirmar == 1) {
        // Atualizar os dados somente após confirmação
        strcpy(nodo->dados->nome, strlen(novoNome) > 0 ? novoNome : nodo->dados->nome);
        strcpy(nodo->dados->endereco, strlen(novoEndereco) > 0 ? novoEndereco : nodo->dados->endereco);
        strcpy(nodo->dados->telefone, strlen(novoTelefone) > 0 ? novoTelefone : nodo->dados->telefone);
        strcpy(nodo->dados->email, strlen(novoEmail) > 0 ? novoEmail : nodo->dados->email);

        // Reorganizar a lista de nomes se o nome foi alterado
        if (strcmp(novoNome, nodo->dados->nome) != 0) {
            NodoLista* atual = *listaNomes;
            NodoLista* anterior = NULL;

            while (atual != NULL) {
                if (strcmp(atual->nome, nodo->dados->nome) == 0) {
                    if (anterior == NULL) {
                        *listaNomes = atual->proximo;
                    } else {
                        anterior->proximo = atual->proximo;
                    }
                    if (atual->proximo != NULL) {
                        atual->proximo->anterior = anterior;
                    }
                    free(atual);
                    break;
                }
                anterior = atual;
                atual = atual->proximo;
            }
            *listaNomes = inserirNaLista(*listaNomes, nodo->dados->nome);
        }

        printf(GREEN "Alteracoes confirmadas.\n" RESET);
    } else {
        printf(RED "Alteracoes canceladas.\n" RESET);
    }
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

    int confirmar;
    printf(BOLD YELLOW "Confirmar Exclusao? (1 - Sim, 2 - Nao): " RESET);
    scanf("%d", &confirmar);

    if (confirmar == 1) {
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
    } else {
        printf(RED "Exclusao cancelada.\n" RESET);
    }
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
    printf(UNDERLINE " CPF: %s\n Nome: %s\n Endereco: %s\n Telefone: %s\n Email: %s\n" RESET,
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
