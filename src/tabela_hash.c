#include "tabela_hash.h"

#include <stdlib.h>

int funcao_hash(int chave) {
    int resto = chave % M;
    /* Em C, -1 % 7 vale -1, que nao seria um indice valido. */
    return resto < 0 ? resto + M : resto;
}

void inicializar_tabela(TabelaHash *tabela) {
    int i;

    for (i = 0; i < M; i++) tabela->posicoes[i] = NULL;
    tabela->quantidade = 0;
}

Status inserir(TabelaHash *tabela, int chave) {
    int h = funcao_hash(chave);
    No *atual = tabela->posicoes[h];
    No *ultimo = NULL;
    No *novo;

    /* Percorre a cadeia para recusar repetidas e para achar o ultimo no. */
    while (atual != NULL) {
        if (atual->chave == chave) return ERRO_REPETIDA;
        ultimo = atual;
        atual = atual->proximo;
    }

    novo = malloc(sizeof *novo);
    if (novo == NULL) return ERRO_MEMORIA;
    novo->chave = chave;
    novo->proximo = NULL;

    if (ultimo == NULL) tabela->posicoes[h] = novo; /* posicao livre */
    else ultimo->proximo = novo;                    /* colisao: encadeia */

    tabela->quantidade++;
    return OK;
}

const No *buscar(const TabelaHash *tabela, int chave, int *comparacoes) {
    const No *atual = tabela->posicoes[funcao_hash(chave)];
    int feitas = 0;

    /* So a cadeia de h(chave) e percorrida, nunca a tabela inteira. */
    while (atual != NULL) {
        feitas++;
        if (atual->chave == chave) break;
        atual = atual->proximo;
    }
    if (comparacoes != NULL) *comparacoes = feitas;
    return atual;
}

Status remover(TabelaHash *tabela, int chave) {
    int h = funcao_hash(chave);
    No *atual = tabela->posicoes[h];
    No *anterior = NULL;

    while (atual != NULL && atual->chave != chave) {
        anterior = atual;
        atual = atual->proximo;
    }
    if (atual == NULL) return ERRO_AUSENTE;

    if (anterior == NULL) tabela->posicoes[h] = atual->proximo; /* era o primeiro */
    else anterior->proximo = atual->proximo;                    /* meio ou fim */

    free(atual);
    tabela->quantidade--;
    return OK;
}

double fator_de_carga(const TabelaHash *tabela) {
    return (double)tabela->quantidade / M;
}

int tamanho_cadeia(const TabelaHash *tabela, int posicao) {
    const No *atual;
    int n = 0;

    for (atual = tabela->posicoes[posicao]; atual != NULL; atual = atual->proximo) n++;
    return n;
}

void imprimir_cadeia(const TabelaHash *tabela, int posicao, FILE *saida) {
    const No *atual;

    for (atual = tabela->posicoes[posicao]; atual != NULL; atual = atual->proximo) {
        fprintf(saida, "[%d] -> ", atual->chave);
    }
    fprintf(saida, "NULL");
}

void imprimir_tabela(const TabelaHash *tabela, FILE *saida) {
    int i;

    fprintf(saida, "     +---+\n");
    for (i = 0; i < M; i++) {
        fprintf(saida, "     | %d | -> ", i);
        imprimir_cadeia(tabela, i, saida);
        fprintf(saida, "\n     +---+\n");
    }
}

void liberar_tabela(TabelaHash *tabela) {
    No *atual, *seguinte;
    int i;

    for (i = 0; i < M; i++) {
        for (atual = tabela->posicoes[i]; atual != NULL; atual = seguinte) {
            seguinte = atual->proximo;
            free(atual);
        }
    }
    inicializar_tabela(tabela);
}
