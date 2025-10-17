/*
 * FreeFire.c
 * Nível: Comparação de Estruturas (Vetores vs Listas Encadeadas)
 *
 * Este programa implementa duas versões de uma mochila de loot inicial:
 *  - Versão Vetor (lista sequencial com capacidade limitada)
 *  - Versão Lista Encadeada (alocação dinâmica de nós)
 *
 * Em ambas as versões são fornecidas operações de inserir, remover, listar e
 * buscar (busca sequencial). Além disso, para a versão vetorial,
 * implementamos ordenação (selection sort) e busca binária. Contadores de
 * comparações são usados para comparar a eficiência das buscas.
 *
 * Bibliotecas: stdio.h, stdlib.h, string.h, time.h (medição opcional)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_ITENS 10
#define MAX_NOME 30
#define MAX_TIPO 20

/* Struct Item: representa um objeto coletado pelo jogador */
typedef struct {
    char nome[MAX_NOME];
    char tipo[MAX_TIPO];
    int quantidade;
} Item;

/* Nó para a lista encadeada */
typedef struct No {
    Item dados;
    struct No *proximo;
} No;

/* --- Dados para o vetor (lista sequencial) --- */
Item vetor[MAX_ITENS];
int numVetor = 0;

/* Contadores de comparações (para análise) */
long comparacoesBuscaSeqVetor = 0;
long comparacoesBuscaBinVetor = 0;
long comparacoesBuscaSeqLista = 0;

/* --- Dados para a lista encadeada --- */
No *head = NULL;

/* Funções utilitárias para leitura */
/* lê uma linha do stdin e remove o '\n' final, se presente */
void lerLinha(char *buffer, int tamanho) {
    if (fgets(buffer, tamanho, stdin) == NULL) {
        buffer[0] = '\0';
        return;
    }
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len-1] == '\n') buffer[len-1] = '\0';
}

/* limpa o restante da linha no stdin */
void limparEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

/* Imprime um item de forma legível */
void imprimirItem(const Item *it, int id) {
    printf("%2d | %-28s | %-12s | %4d\n", id, it->nome, it->tipo, it->quantidade);
}

/* ======================== FUNÇÕES - VETOR ======================== */

/* Lista os itens do vetor */
void listarItensVetor() {
    printf("\n--- Mochila (VETOR) [%d/%d] ---\n", numVetor, MAX_ITENS);
    if (numVetor == 0) {
        printf("Mochila vazia.\n");
        return;
    }
    printf("ID | %-28s | %-12s | QTD\n", "NOME", "TIPO");
    for (int i = 0; i < numVetor; i++) imprimirItem(&vetor[i], i+1);
}

/* Insere item no vetor (se houver espaço) */
void inserirItemVetor() {
    if (numVetor >= MAX_ITENS) {
        printf("Mochila (vetor) cheia. Remova um item antes de inserir.\n");
        return;
    }
    Item novo;
    printf("Nome: "); lerLinha(novo.nome, MAX_NOME);
    if (strlen(novo.nome) == 0) { printf("Nome inválido.\n"); return; }
    printf("Tipo (arma/munição/cura): "); lerLinha(novo.tipo, MAX_TIPO);
    char buf[32];
    printf("Quantidade: ");
    if (fgets(buf, sizeof(buf), stdin) == NULL) novo.quantidade = 1;
    else novo.quantidade = atoi(buf);
    if (novo.quantidade <= 0) novo.quantidade = 1;
    vetor[numVetor++] = novo;
    printf("Item '%s' adicionado ao vetor.\n", novo.nome);
    listarItensVetor();
}

/* Busca sequencial no vetor; retorna índice ou -1 */
int buscarSequencialVetor(const char *nome) {
    comparacoesBuscaSeqVetor = 0;
    for (int i = 0; i < numVetor; i++) {
        comparacoesBuscaSeqVetor++;
        if (strcmp(vetor[i].nome, nome) == 0) return i;
    }
    return -1;
}

/* Remove item do vetor por nome (desloca elementos) */
void removerItemVetor() {
    if (numVetor == 0) { printf("Mochila (vetor) vazia.\n"); return; }
    char nome[MAX_NOME];
    printf("Nome do item a remover: "); lerLinha(nome, MAX_NOME);
    if (strlen(nome) == 0) { printf("Nome inválido.\n"); return; }
    int idx = buscarSequencialVetor(nome);
    if (idx == -1) { printf("Item '%s' não encontrado no vetor. (comparacoes=%ld)\n", nome, comparacoesBuscaSeqVetor); return; }
    for (int i = idx; i < numVetor - 1; i++) vetor[i] = vetor[i+1];
    numVetor--;
    printf("Item '%s' removido do vetor.\n", nome);
    listarItensVetor();
}

/* Ordenação por nome (selection sort) no vetor - estável o suficiente para poucos itens */
void ordenarVetorPorNome() {
    if (numVetor < 2) { printf("Nada a ordenar.\n"); return; }
    for (int i = 0; i < numVetor - 1; i++) {
        int min = i;
        for (int j = i + 1; j < numVetor; j++) {
            if (strcmp(vetor[j].nome, vetor[min].nome) < 0) min = j;
        }
        if (min != i) {
            Item tmp = vetor[i]; vetor[i] = vetor[min]; vetor[min] = tmp;
        }
    }
    printf("Vetor ordenado por nome.\n");
    listarItensVetor();
}

/* Busca binária no vetor (assume vetor ordenado por nome) */
int buscarBinariaVetor(const char *nome) {
    comparacoesBuscaBinVetor = 0;
    int lo = 0, hi = numVetor - 1;
    while (lo <= hi) {
        int mid = (lo + hi) / 2;
        comparacoesBuscaBinVetor++;
        int cmp = strcmp(vetor[mid].nome, nome);
        if (cmp == 0) return mid;
        else if (cmp < 0) lo = mid + 1;
        else hi = mid - 1;
    }
    return -1;
}

/* ==================== FUNÇÕES - LISTA ENCADEADA ==================== */

/* Insere no início da lista (rápido) */
void inserirItemLista() {
    Item novo;
    printf("Nome: "); lerLinha(novo.nome, MAX_NOME);
    if (strlen(novo.nome) == 0) { printf("Nome inválido.\n"); return; }
    printf("Tipo (arma/munição/cura): "); lerLinha(novo.tipo, MAX_TIPO);
    char buf[32];
    printf("Quantidade: "); if (fgets(buf, sizeof(buf), stdin) == NULL) novo.quantidade = 1; else novo.quantidade = atoi(buf);
    if (novo.quantidade <= 0) novo.quantidade = 1;
    No *n = (No*)malloc(sizeof(No));
    if (!n) { printf("Erro de alocação.\n"); return; }
    n->dados = novo;
    n->proximo = head;
    head = n;
    printf("Item '%s' inserido na lista encadeada.\n", novo.nome);
}

/* Busca sequencial na lista (retorna ponteiro para o nó ou NULL) */
No* buscarSequencialLista(const char *nome) {
    comparacoesBuscaSeqLista = 0;
    No *cur = head;
    while (cur) {
        comparacoesBuscaSeqLista++;
        if (strcmp(cur->dados.nome, nome) == 0) return cur;
        cur = cur->proximo;
    }
    return NULL;
}

/* Remove pela primeira ocorrência do nome */
void removerItemLista() {
    if (!head) { printf("Mochila (lista) vazia.\n"); return; }
    char nome[MAX_NOME];
    printf("Nome do item a remover (lista): "); lerLinha(nome, MAX_NOME);
    if (strlen(nome) == 0) { printf("Nome inválido.\n"); return; }
    No *cur = head, *prev = NULL;
    while (cur) {
        comparacoesBuscaSeqLista++;
        if (strcmp(cur->dados.nome, nome) == 0) break;
        prev = cur; cur = cur->proximo;
    }
    if (!cur) { printf("Item '%s' não encontrado na lista. (comparacoes=%ld)\n", nome, comparacoesBuscaSeqLista); return; }
    if (!prev) head = cur->proximo; else prev->proximo = cur->proximo;
    free(cur);
    printf("Item '%s' removido da lista.\n", nome);
}

/* Lista todos os itens da lista encadeada */
void listarItensLista() {
    printf("\n--- Mochila (LISTA) ---\n");
    if (!head) { printf("Mochila vazia.\n"); return; }
    printf("ID | %-28s | %-12s | QTD\n", "NOME", "TIPO");
    No *cur = head; int id = 1;
    while (cur) { imprimirItem(&cur->dados, id++); cur = cur->proximo; }
}

/* ======================== MENU E INTEGRAÇÃO ======================== */

void mostrarComparacoes() {
    printf("\nComparações realizadas:\n");
    printf("Busca sequencial (vetor): %ld\n", comparacoesBuscaSeqVetor);
    printf("Busca binária (vetor): %ld\n", comparacoesBuscaBinVetor);
    printf("Busca sequencial (lista): %ld\n", comparacoesBuscaSeqLista);
}

void menuPrincipal() {
    while (1) {
        printf("\n=== Mochila: Vetor vs Lista Encadeada ===\n");
        printf("1 - Trabalhar com VETOR\n");
        printf("2 - Trabalhar com LISTA ENCADEADA\n");
        printf("3 - Mostrar comparações\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        char line[8]; lerLinha(line, sizeof(line));
        int op = atoi(line);
        if (op == 0) { printf("Saindo...\n"); return; }
        if (op == 1) {
            while (1) {
                printf("\n-- Modo VETOR --\n");
                printf("1 Inserir | 2 Remover | 3 Listar | 4 Buscar Seq | 5 Ordenar | 6 Buscar Binaria | 0 Voltar\n");
                printf("Escolha: "); lerLinha(line, sizeof(line)); int vop = atoi(line);
                if (vop == 0) break;
                if (vop == 1) inserirItemVetor();
                else if (vop == 2) removerItemVetor();
                else if (vop == 3) listarItensVetor();
                else if (vop == 4) {
                    char nome[MAX_NOME]; printf("Nome para busca seq (vetor): "); lerLinha(nome, MAX_NOME);
                    int idx = buscarSequencialVetor(nome);
                    if (idx == -1) printf("Não encontrado. Comparações=%ld\n", comparacoesBuscaSeqVetor);
                    else { printf("Encontrado no índice %d\n", idx+1); imprimirItem(&vetor[idx], idx+1); }
                }
                else if (vop == 5) ordenarVetorPorNome();
                else if (vop == 6) {
                    char nome[MAX_NOME]; printf("Nome para busca binária (vetor): "); lerLinha(nome, MAX_NOME);
                    int idx = buscarBinariaVetor(nome);
                    if (idx == -1) printf("Não encontrado. Comparações=%ld\n", comparacoesBuscaBinVetor);
                    else { printf("Encontrado no índice %d (binária)\n", idx+1); imprimirItem(&vetor[idx], idx+1); }
                }
                else printf("Opção inválida.\n");
            }
        } else if (op == 2) {
            while (1) {
                printf("\n-- Modo LISTA --\n");
                printf("1 Inserir | 2 Remover | 3 Listar | 4 Buscar Seq | 0 Voltar\n");
                printf("Escolha: "); char line2[8]; lerLinha(line2, sizeof(line2)); int lop = atoi(line2);
                if (lop == 0) break;
                if (lop == 1) inserirItemLista();
                else if (lop == 2) removerItemLista();
                else if (lop == 3) listarItensLista();
                else if (lop == 4) {
                    char nome[MAX_NOME]; printf("Nome para busca seq (lista): "); lerLinha(nome, MAX_NOME);
                    No *res = buscarSequencialLista(nome);
                    if (!res) printf("Não encontrado. Comparações=%ld\n", comparacoesBuscaSeqLista);
                    else { printf("Encontrado (lista):\n"); imprimirItem(&res->dados, 0); }
                }
                else printf("Opção inválida.\n");
            }
        } else if (op == 3) {
            mostrarComparacoes();
        } else printf("Opção inválida.\n");
    }
}

int main() {
    /* Inicializa variáveis e chama o menu principal. */
    numVetor = 0;
    head = NULL;
    menuPrincipal();
    /* Limpeza: liberar lista */
    No *cur = head;
    while (cur) { No *t = cur->proximo; free(cur); cur = t; }
    return 0;
}
