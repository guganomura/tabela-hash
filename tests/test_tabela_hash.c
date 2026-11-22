#include <limits.h>
#include <math.h>
#include <string.h>

#include "../src/tabela_hash.h"

static int total, falhas;
static const int CHAVES[] = {190, 322, 172, 89, 13, 4, 769, 61, 15, 76};

static void verificar(const char *id, const char *cenario, int condicao) {
    total++;
    if (!condicao) falhas++;
    printf("[%s] %s %s\n", condicao ? "OK" : "FALHA", id, cenario);
}

static TabelaHash montar(void) {
    TabelaHash t;
    int i;
    inicializar_tabela(&t);
    for (i = 0; i < 10; i++) inserir(&t, CHAVES[i]);
    return t;
}

static int cadeia(const TabelaHash *t, int posicao, const char *esperada) {
    char texto[128];
    FILE *f = tmpfile();
    size_t n;
    if (f == NULL) return 0;
    imprimir_cadeia(t, posicao, f);
    rewind(f);
    n = fread(texto, 1, sizeof texto - 1, f);
    texto[n] = '\0';
    fclose(f);
    return strcmp(texto, esperada) == 0;
}

static void testar_hash(void) {
    const int esperado[] = {1, 0, 4, 5, 6, 4, 6, 5, 1, 6};
    int i, certo = 1;
    for (i = 0; i < 10; i++)
        if (funcao_hash(CHAVES[i]) != esperado[i]) certo = 0;
    verificar("H-01", "h(k) das 10 chaves do exercicio", certo);
    verificar("H-02", "limites 0, 6 e 7", !funcao_hash(0) && funcao_hash(6) == 6 && !funcao_hash(7));
    verificar("H-03", "negativas caem de 0 a 6", funcao_hash(-1) == 6 && funcao_hash(-7) == 0);
    verificar("H-04", "extremos de int", funcao_hash(INT_MAX) == 1 && funcao_hash(INT_MIN) == 5);
}

static void testar_insercao(void) {
    TabelaHash t;
    int i, vazia = 1;
    inicializar_tabela(&t);
    for (i = 0; i < M; i++)
        if (t.posicoes[i] != NULL) vazia = 0;
    verificar("I-01", "tabela inicia vazia", vazia && t.quantidade == 0);
    verificar("I-02", "insercao sem colisao", inserir(&t, 190) == OK && cadeia(&t, 1, "[190] -> NULL"));
    verificar("I-03", "colisao entra no fim da cadeia",
              inserir(&t, 15) == OK && cadeia(&t, 1, "[190] -> [15] -> NULL"));
    verificar("I-04", "chave repetida e rejeitada", inserir(&t, 15) == ERRO_REPETIDA && t.quantidade == 2);
    liberar_tabela(&t);

    t = montar();
    verificar("T-00", "posicao 0", cadeia(&t, 0, "[322] -> NULL"));
    verificar("T-01", "posicao 1", cadeia(&t, 1, "[190] -> [15] -> NULL"));
    verificar("T-02", "posicao 2", cadeia(&t, 2, "NULL"));
    verificar("T-03", "posicao 3", cadeia(&t, 3, "NULL"));
    verificar("T-04", "posicao 4", cadeia(&t, 4, "[172] -> [4] -> NULL"));
    verificar("T-05", "posicao 5", cadeia(&t, 5, "[89] -> [61] -> NULL"));
    verificar("T-06", "posicao 6", cadeia(&t, 6, "[13] -> [769] -> [76] -> NULL"));
    liberar_tabela(&t);
}

static void testar_busca(void) {
    TabelaHash t = montar();
    int c = -1;
    verificar("B-01", "primeira da cadeia, 1 comparacao", buscar(&t, 13, &c) && c == 1);
    verificar("B-02", "terceira da cadeia, 3 comparacoes", buscar(&t, 76, &c) && c == 3);
    verificar("B-03", "ausente em cadeia de 3", !buscar(&t, 20, &c) && c == 3);
    verificar("B-04", "ausente em posicao vazia", !buscar(&t, 100, &c) && c == 0);
    liberar_tabela(&t);
}

static void testar_remocao(void) {
    TabelaHash t = montar();
    verificar("R-01", "meio da cadeia",
              remover(&t, 769) == OK && cadeia(&t, 6, "[13] -> [76] -> NULL"));
    verificar("R-02", "fim da cadeia", remover(&t, 61) == OK && cadeia(&t, 5, "[89] -> NULL"));
    verificar("R-03", "inicio da cadeia", remover(&t, 190) == OK && cadeia(&t, 1, "[15] -> NULL"));
    verificar("R-04", "unico da cadeia", remover(&t, 322) == OK && cadeia(&t, 0, "NULL"));
    verificar("R-05", "ausente em cadeia ocupada",
              remover(&t, 20) == ERRO_AUSENTE && cadeia(&t, 6, "[13] -> [76] -> NULL"));
    verificar("R-06", "ausente em posicao vazia", remover(&t, 100) == ERRO_AUSENTE && t.quantidade == 6);
    verificar("R-07", "reinserir removida vai para o fim",
              inserir(&t, 769) == OK && cadeia(&t, 6, "[13] -> [76] -> [769] -> NULL"));
    liberar_tabela(&t);
}

static void testar_carga(void) {
    TabelaHash t;
    inicializar_tabela(&t);
    verificar("C-01", "tabela vazia, alfa = 0", fator_de_carga(&t) == 0);
    t = montar();
    verificar("C-02", "10 chaves, alfa = 10/7", fabs(fator_de_carga(&t) - 10.0 / 7) < 1e-12);
    verificar("C-03", "cadeias com 3 e 0 nos", tamanho_cadeia(&t, 6) == 3 && !tamanho_cadeia(&t, 2));
    remover(&t, 769);
    remover(&t, 61);
    remover(&t, 190);
    remover(&t, 322);
    verificar("C-04", "apos 4 remocoes, alfa = 6/7", fabs(fator_de_carga(&t) - 6.0 / 7) < 1e-12);
    liberar_tabela(&t);
    verificar("C-05", "liberar zera a tabela", t.quantidade == 0 && cadeia(&t, 6, "NULL"));
}

int main(void) {
    testar_hash();
    testar_insercao();
    testar_busca();
    testar_remocao();
    testar_carga();
    printf("\nTotal: %d testes, %d falhas\n", total, falhas);
    return falhas != 0;
}
