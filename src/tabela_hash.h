#ifndef TABELA_HASH_H
#define TABELA_HASH_H

#include <stdio.h>

#define M 7 /* numero de posicoes da tabela */

typedef struct No {
    int chave;
    struct No *proximo;
} No;

typedef struct {
    No *posicoes[M]; /* inicio de cada cadeia; NULL se vazia */
    int quantidade;
} TabelaHash;

typedef enum { OK, ERRO_REPETIDA, ERRO_AUSENTE, ERRO_MEMORIA } Status;

/* h(k) = k mod 7, sempre de 0 a 6. */
int funcao_hash(int chave);

void inicializar_tabela(TabelaHash *tabela);

/* Encadeia no final da posicao h(chave). Nao aceita chave repetida. */
Status inserir(TabelaHash *tabela, int chave);

/* Retorna o no, ou NULL. Se comparacoes nao for NULL, recebe quantas fez. */
const No *buscar(const TabelaHash *tabela, int chave, int *comparacoes);

Status remover(TabelaHash *tabela, int chave);

double fator_de_carga(const TabelaHash *tabela);
int tamanho_cadeia(const TabelaHash *tabela, int posicao);

void imprimir_cadeia(const TabelaHash *tabela, int posicao, FILE *saida);
void imprimir_tabela(const TabelaHash *tabela, FILE *saida);

void liberar_tabela(TabelaHash *tabela);

#endif
