/* Executa as tarefas do exercicio. As mensagens nao usam acentos para
 * aparecerem corretamente tambem no terminal do Windows. */
#include "tabela_hash.h"

#define TAMANHO(v) ((int)(sizeof(v) / sizeof((v)[0])))

static const int CHAVES[] = {190, 322, 172, 89, 13, 4, 769, 61, 15, 76};

/* O enunciado nao diz o que remover. Estas chaves cobrem todos os casos:
 * meio, fim, inicio, unica e duas ausentes. */
static const int REMOCOES[] = {769, 61, 190, 322, 20, 100};

static void calcular_hashes(void) {
    int i, k;

    printf("=== 1. Calculo de h(k) = k mod %d ===\n\n", M);
    for (i = 0; i < TAMANHO(CHAVES); i++) {
        k = CHAVES[i];
        printf("h(%3d) = %3d mod %d = %d    (%3d = %d*%3d + %d)\n",
               k, k, M, funcao_hash(k), k, M, k / M, funcao_hash(k));
    }
}

static void desenhar_resultante(const TabelaHash *tabela) {
    printf("\n=== 2. Tabela hash resultante ===\n\n");
    imprimir_tabela(tabela, stdout);
}

static int simular_insercoes(TabelaHash *tabela) {
    int i, h, colidiu, colisoes = 0;

    printf("\n=== 3a. Simulacao das insercoes ===\n");
    for (i = 0; i < TAMANHO(CHAVES); i++) {
        h = funcao_hash(CHAVES[i]);
        colidiu = tabela->posicoes[h] != NULL;
        colisoes += colidiu;

        printf("\n%2d) inserir %d: h = %d, %s\n", i + 1, CHAVES[i], h,
               colidiu ? "COLISAO, encadeia no fim" : "posicao livre");
        printf("    antes:  ");
        imprimir_cadeia(tabela, h, stdout);
        if (inserir(tabela, CHAVES[i]) != OK) return 0;
        printf("\n    depois: ");
        imprimir_cadeia(tabela, h, stdout);
        printf("\n    n = %d, alfa = %.4f\n", tabela->quantidade, fator_de_carga(tabela));
    }
    printf("\nTotal de insercoes com colisao: %d de %d\n", colisoes, TAMANHO(CHAVES));
    return 1;
}

static void simular_remocoes(TabelaHash *tabela) {
    int i, h;
    Status status;

    printf("\n=== 3b. Simulacao das remocoes ===\n");
    for (i = 0; i < TAMANHO(REMOCOES); i++) {
        h = funcao_hash(REMOCOES[i]);
        printf("\n%2d) remover %d: h = %d\n", i + 1, REMOCOES[i], h);
        printf("    antes:  ");
        imprimir_cadeia(tabela, h, stdout);
        status = remover(tabela, REMOCOES[i]);
        printf("\n    depois: ");
        imprimir_cadeia(tabela, h, stdout);
        printf("%s", status == OK ? "" : "  (chave ausente, nada muda)");
        printf("\n    n = %d, alfa = %.4f\n", tabela->quantidade, fator_de_carga(tabela));
    }
    printf("\nTabela apos as remocoes:\n\n");
    imprimir_tabela(tabela, stdout);
}

static void analisar(const TabelaHash *tabela, const char *momento) {
    const No *no;
    int i, n, comparacoes, soma = 0, vazias = 0, maior = 0;

    printf("\n%s\n", momento);
    printf("  alfa = n / m = %d / %d = %.4f\n", tabela->quantidade, M, fator_de_carga(tabela));
    for (i = 0; i < M; i++) {
        n = tamanho_cadeia(tabela, i);
        if (n == 0) vazias++;
        if (n > maior) maior = n;
        for (no = tabela->posicoes[i]; no != NULL; no = no->proximo) {
            buscar(tabela, no->chave, &comparacoes);
            soma += comparacoes;
        }
    }
    printf("  posicoes vazias: %d de %d\n", vazias, M);
    printf("  maior cadeia: %d\n", maior);
    printf("  media de comparacoes, busca com sucesso: %d / %d = %.4f\n",
           soma, tabela->quantidade, (double)soma / tabela->quantidade);
}

int main(void) {
    TabelaHash completa, simulada;
    int i, certo = 1;

    inicializar_tabela(&completa);
    inicializar_tabela(&simulada);

    calcular_hashes();
    for (i = 0; certo && i < TAMANHO(CHAVES); i++) certo = inserir(&completa, CHAVES[i]) == OK;
    if (certo) {
        desenhar_resultante(&completa);
        certo = simular_insercoes(&simulada);
    }

    if (certo) {
        simular_remocoes(&simulada);
        printf("\n=== 4. Fator de carga e distribuicao ===\n");
        analisar(&completa, "Final das insercoes:");
        analisar(&simulada, "Final das remocoes:");
    } else {
        printf("Erro: memoria insuficiente.\n");
    }

    liberar_tabela(&completa);
    liberar_tabela(&simulada);
    return certo ? 0 : 1;
}
