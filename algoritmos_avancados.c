#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/// ----------------------------------------------------------
/// ESTRUTURAS DO NÍVEL NOVATO – Árvore Binária da Mansão
/// ----------------------------------------------------------

typedef struct Sala {
    char nome[50];
    struct Sala *esq;
    struct Sala *dir;
} Sala;

Sala* criarSala(const char *nome) {
    Sala *s = (Sala*) malloc(sizeof(Sala));
    strcpy(s->nome, nome);
    s->esq = s->dir = NULL;
    return s;
}

/// ----------------------------------------------------------
/// ESTRUTURAS DO NÍVEL AVENTUREIRO – Árvore de Busca (BST)
/// ----------------------------------------------------------

typedef struct NoBST {
    char pista[50];
    struct NoBST *esq, *dir;
} NoBST;

NoBST* inserirBST(NoBST* raiz, const char* pista) {
    if (raiz == NULL) {
        NoBST* novo = (NoBST*) malloc(sizeof(NoBST));
        strcpy(novo->pista, pista);
        novo->esq = novo->dir = NULL;
        return novo;
    }
    if (strcmp(pista, raiz->pista) < 0)
        raiz->esq = inserirBST(raiz->esq, pista);
    else if (strcmp(pista, raiz->pista) > 0)
        raiz->dir = inserirBST(raiz->dir, pista);

    return raiz;
}

void emOrdem(NoBST* raiz) {
    if (!raiz) return;
    emOrdem(raiz->esq);
    printf(" - %s\n", raiz->pista);
    emOrdem(raiz->dir);
}

/// ----------------------------------------------------------
/// ESTRUTURAS DO NÍVEL MESTRE – Tabela Hash (Pista → Suspeito)
/// ----------------------------------------------------------

#define TAM_HASH  10

typedef struct HashNode {
    char pista[50];
    char suspeito[50];
    struct HashNode *prox;
} HashNode;

HashNode* hashTable[TAM_HASH];

unsigned int hash(const char* str) {
    int soma = 0;
    for (int i = 0; str[i] != '\0'; i++)
        soma += str[i];
    return soma % TAM_HASH;
}

void inserirNaHash(const char* pista, const char* suspeito) {
    unsigned int h = hash(pista);

    HashNode *novo = (HashNode*) malloc(sizeof(HashNode));
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->prox = hashTable[h];
    hashTable[h] = novo;
}

void mostrarHash() {
    printf("\n=== Associações Pista → Suspeito ===\n");
    for (int i = 0; i < TAM_HASH; i++) {
        HashNode *aux = hashTable[i];
        while (aux) {
            printf("Pista: %-20s | Suspeito: %s\n", aux->pista, aux->suspeito);
            aux = aux->prox;
        }
    }
}

char* suspeitoMaisCitado() {
    typedef struct {
        char nome[50];
        int qtd;
    } Contador;

    Contador lista[50];
    int total = 0;

    // conta ocorrências
    for (int i = 0; i < TAM_HASH; i++) {
        HashNode *aux = hashTable[i];

        while (aux) {
            int encontrado = 0;
            for (int j = 0; j < total; j++) {
                if (strcmp(lista[j].nome, aux->suspeito) == 0) {
                    lista[j].qtd++;
                    encontrado = 1;
                    break;
                }
            }
            if (!encontrado) {
                strcpy(lista[total].nome, aux->suspeito);
                lista[total].qtd = 1;
                total++;
            }
            aux = aux->prox;
        }
    }

    // encontra o maior
    int maior = -1;
    char *suspeitoFinal = "Nenhum";

    for (int i = 0; i < total; i++) {
        if (lista[i].qtd > maior) {
            maior = lista[i].qtd;
            suspeitoFinal = lista[i].nome;
        }
    }

    return suspeitoFinal;
}

/// ----------------------------------------------------------
/// LÓGICA DE EXPLORAÇÃO – NÍVEL NOVATO + AVENTUREIRO + MESTRE
/// ----------------------------------------------------------

void explorarSalas(Sala *atual, NoBST **pistasBST) {
    char opcao;

    while (atual) {
        printf("\nVocê está na sala: **%s**\n", atual->nome);

        // Sistema: salas com pistas pré-definidas
        if (strcmp(atual->nome, "Biblioteca") == 0) {
            printf("Você encontrou uma pista: 'Livro Rasgado'!\n");
            *pistasBST = inserirBST(*pistasBST, "Livro Rasgado");
            inserirNaHash("Livro Rasgado", "Sr. Barnes");
        }
        if (strcmp(atual->nome, "Cozinha") == 0) {
            printf("Você encontrou uma pista: 'Faca Faltando'!\n");
            *pistasBST = inserirBST(*pistasBST, "Faca Faltando");
            inserirNaHash("Faca Faltando", "Sra. Kelly");
        }
        if (strcmp(atual->nome, "Jardim") == 0) {
            printf("Você encontrou uma pista: 'Pegadas Estranhas'!\n");
            *pistasBST = inserirBST(*pistasBST, "Pegadas Estranhas");
            inserirNaHash("Pegadas Estranhas", "Sr. Barnes");
        }

        // Parou em folha
        if (!atual->esq && !atual->dir) {
            printf("\n Fim do caminho nesta direção.\n");
            return;
        }

        printf("Escolha o caminho: (e) esquerda | (d) direita | (s) sair\n> ");
        scanf(" %c", &opcao);

        if (opcao == 'e') atual = atual->esq;
        else if (opcao == 'd') atual = atual->dir;
        else if (opcao == 's') return;
        else printf("Opção inválida!\n");
    }
}

/// ----------------------------------------------------------
/// MAIN – Monta a Mansão e executa os 3 níveis
/// ----------------------------------------------------------

int main() {
    // NÍVEL NOVATO – Construção da Mansão (Árvore Binária)
    Sala *hall = criarSala("Hall de Entrada");
    hall->esq = criarSala("Biblioteca");
    hall->dir = criarSala("Cozinha");
    
    hall->esq->esq = criarSala("Porão");
    hall->esq->dir = criarSala("Jardim");
    
    hall->dir->esq = criarSala("Dispensa");
    hall->dir->dir = criarSala("Sala de Jantar");

    NoBST *pistasBST = NULL;

    printf("===== DETETIVE QUEST – MANSÃO MISTERIOSA =====\n");
    explorarSalas(hall, &pistasBST);

    // NÍVEL AVENTUREIRO – Mostrar pistas encontradas
    printf("\n=== PISTAS COLETADAS (em ordem alfabética) ===\n");
    emOrdem(pistasBST);

    // NÍVEL MESTRE – Exibir tabela e culpado provável
    mostrarHash();

    printf("\n=== SUSPEITO MAIS ASSOCIADO ===\n");
    printf("Provável culpado: **%s**\n", suspeitoMaisCitado());

    return 0;
}


