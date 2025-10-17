/*
 * FreeFire.c
 * Sistema simplificado de inventário para o nível iniciante.
 * Implementa a struct Item e operações básicas sobre um vetor sequencial
 * (inserir, remover, listar e busca sequencial).
 *
 * Requisitos cumpridos:
 * - struct Item { char nome[30]; char tipo[20]; int quantidade; }
 * - Vetor com capacidade para 10 itens
 * - Funções: inserirItem(), removerItem(), listarItens(), buscarItem()
 * - Interface simples por menu
 *
 * Bibliotecas: stdio.h, string.h, stdlib.h
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_ITENS 10
#define MAX_NOME 30
#define MAX_TIPO 20

/*
 * Struct Item:
 * Representa um objeto coletado pelo jogador no início do jogo.
 * Campos:
 * - nome: nome do item (ex: "Rifle", "Munição 5.56")
 * - tipo: categoria (ex: "arma", "munição", "cura")
 * - quantidade: quantidade do item no inventário
 */
typedef struct {
    char nome[MAX_NOME];
    char tipo[MAX_TIPO];
    int quantidade;
} Item;

/* Vetor que representa a mochila (inventário) e contador de itens atuais */
Item mochila[MAX_ITENS];
int numItens = 0;

/* Funções utilitárias */
/* limpa o buffer de entrada até o fim da linha */
void limparEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

/* lê uma linha do stdin e remove o '\n' final, se presente */
void lerLinha(char *buffer, int tamanho) {
    if (fgets(buffer, tamanho, stdin) == NULL) {
        buffer[0] = '\0';
        return;
    }
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len-1] == '\n') buffer[len-1] = '\0';
}

/* Exibe todos os itens da mochila em formato legível */
void listarItens() {
    printf("\n--- Itens na mochila (%d/%d) ---\n", numItens, MAX_ITENS);
    if (numItens == 0) {
        printf("Mochila vazia.\n");
        return;
    }
    printf("%-3s %-28s %-12s %-10s\n", "ID", "NOME", "TIPO", "QTD");
    for (int i = 0; i < numItens; i++) {
        printf("%-3d %-28s %-12s %-10d\n", i+1, mochila[i].nome, mochila[i].tipo, mochila[i].quantidade);
    }
}

/* Insere um novo item na mochila (até MAX_ITENS). Lê nome,tipo e quantidade. */
void inserirItem() {
    if (numItens >= MAX_ITENS) {
        printf("Mochila cheia. Não é possível adicionar mais itens.\n");
        return;
    }

    Item novo;
    printf("\nCadastro de novo item:\n");
    printf("Nome: ");
    lerLinha(novo.nome, MAX_NOME);
    if (strlen(novo.nome) == 0) {
        printf("Nome inválido. Operação cancelada.\n");
        return;
    }

    printf("Tipo (ex: arma, munição, cura): ");
    lerLinha(novo.tipo, MAX_TIPO);
    if (strlen(novo.tipo) == 0) strncpy(novo.tipo, "desconhecido", MAX_TIPO-1);

    printf("Quantidade: ");
    if (scanf("%d", &novo.quantidade) != 1) {
        printf("Quantidade inválida. Será atribuída 1 por padrão.\n");
        novo.quantidade = 1;
    }
    limparEntrada(); /* remove o '\n' deixado pelo scanf */

    if (novo.quantidade <= 0) novo.quantidade = 1;

    mochila[numItens++] = novo;
    printf("Item '%s' adicionado com sucesso.\n", novo.nome);
    listarItens();
}

/* Remove um item da mochila pelo nome (busca sequencial) */
void removerItem() {
    if (numItens == 0) {
        printf("Mochila vazia. Nada a remover.\n");
        return;
    }
    char nomeBusca[MAX_NOME];
    printf("\nRemover item - digite o nome: ");
    lerLinha(nomeBusca, MAX_NOME);
    if (strlen(nomeBusca) == 0) {
        printf("Nome inválido. Operação cancelada.\n");
        return;
    }

    int idx = -1;
    for (int i = 0; i < numItens; i++) {
        if (strcmp(mochila[i].nome, nomeBusca) == 0) {
            idx = i;
            break;
        }
    }
    if (idx == -1) {
        printf("Item '%s' não encontrado na mochila.\n", nomeBusca);
        listarItens();
        return;
    }

    /* desloca os itens para preencher a lacuna */
    for (int i = idx; i < numItens - 1; i++) {
        mochila[i] = mochila[i+1];
    }
    numItens--;
    printf("Item '%s' removido com sucesso.\n", nomeBusca);
    listarItens();
}

/* Busca sequencial por nome e exibe os dados do item, se encontrado */
void buscarItem() {
    if (numItens == 0) {
        printf("Mochila vazia.\n");
        return;
    }
    char nomeBusca[MAX_NOME];
    printf("\nBuscar item - digite o nome: ");
    lerLinha(nomeBusca, MAX_NOME);
    if (strlen(nomeBusca) == 0) {
        printf("Nome inválido. Operação cancelada.\n");
        return;
    }

    for (int i = 0; i < numItens; i++) {
        if (strcmp(mochila[i].nome, nomeBusca) == 0) {
            printf("Item encontrado (posição %d):\n", i+1);
            printf("Nome: %s\nTipo: %s\nQuantidade: %d\n", mochila[i].nome, mochila[i].tipo, mochila[i].quantidade);
            listarItens();
            return;
        }
    }
    printf("Item '%s' não encontrado.\n", nomeBusca);
    listarItens();
}

/* Exibe o menu principal */
void exibirMenu() {
    printf("\n=== Mochila inicial - Coleta rápida (Nível Novato) ===\n");
    printf("1 - Cadastrar item\n");
    printf("2 - Remover item\n");
    printf("3 - Listar itens\n");
    printf("4 - Buscar item por nome\n");
    printf("0 - Sair\n");
    printf("Escolha uma opção: ");
}

int main() {
    /* Loop principal do menu. Uso de scanf para inteiros e fgets para strings conforme solicitado. */
    int opcao = -1;
    while (1) {
        exibirMenu();
        if (scanf("%d", &opcao) != 1) {
            printf("Entrada inválida. Tente novamente.\n");
            limparEntrada();
            continue;
        }
        limparEntrada(); /* descartar resto da linha */

        switch (opcao) {
            case 1: inserirItem(); break;
            case 2: removerItem(); break;
            case 3: listarItens(); break;
            case 4: buscarItem(); break;
            case 0: printf("Encerrando. Boa sorte na partida!\n"); return 0;
            default: printf("Opção inválida. Escolha uma opção do menu.\n"); break;
        }
    }
    return 0;
}
