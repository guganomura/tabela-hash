# Tabela hash com encadeamento externo

- **Função hash:** `h(k) = k mod 7`
- **Colisões:** encadeamento externo, com inserção no final da cadeia
- **Conjunto:** {190, 322, 172, 89, 13, 4, 769, 61, 15, 76}

## 1. Cálculo de h(k)

| Ordem | k | Divisão por 7 | h(k) | Colisão na inserção? |
|:-:|:-:|---|:-:|---|
| 1 | 190 | 190 = 7·27 + 1 | **1** | não |
| 2 | 322 | 322 = 7·46 + 0 | **0** | não |
| 3 | 172 | 172 = 7·24 + 4 | **4** | não |
| 4 | 89 | 89 = 7·12 + 5 | **5** | não |
| 5 | 13 | 13 = 7·1 + 6 | **6** | não |
| 6 | 4 | 4 = 7·0 + 4 | **4** | sim, com 172 |
| 7 | 769 | 769 = 7·109 + 6 | **6** | sim, com 13 |
| 8 | 61 | 61 = 7·8 + 5 | **5** | sim, com 89 |
| 9 | 15 | 15 = 7·2 + 1 | **1** | sim, com 190 |
| 10 | 76 | 76 = 7·10 + 6 | **6** | sim, com 13 e 769 |

São 5 inserções com colisão.

## 2. Tabela hash resultante

```text
+---+
| 0 | -> [322] -> NULL
+---+
| 1 | -> [190] -> [15] -> NULL
+---+
| 2 | -> NULL
+---+
| 3 | -> NULL
+---+
| 4 | -> [172] -> [4] -> NULL
+---+
| 5 | -> [89] -> [61] -> NULL
+---+
| 6 | -> [13] -> [769] -> [76] -> NULL
+---+
```

## 3. Simulação de inserção e remoção

### Inserção

Calcula-se h(k), percorre-se apenas aquela cadeia (para recusar repetidas) e o nó é ligado no final.

| Passo | Inserir | Posição | Cadeia antes | Cadeia depois | n | α |
|:-:|:-:|:-:|---|---|:-:|:-:|
| 1 | 190 | 1 | vazia | 190 | 1 | 0,1429 |
| 2 | 322 | 0 | vazia | 322 | 2 | 0,2857 |
| 3 | 172 | 4 | vazia | 172 | 3 | 0,4286 |
| 4 | 89 | 5 | vazia | 89 | 4 | 0,5714 |
| 5 | 13 | 6 | vazia | 13 | 5 | 0,7143 |
| 6 | 4 | 4 | 172 | 172 → 4 | 6 | 0,8571 |
| 7 | 769 | 6 | 13 | 13 → 769 | 7 | 1,0000 |
| 8 | 61 | 5 | 89 | 89 → 61 | 8 | 1,1429 |
| 9 | 15 | 1 | 190 | 190 → 15 | 9 | 1,2857 |
| 10 | 76 | 6 | 13 → 769 | 13 → 769 → 76 | 10 | 1,4286 |

### Remoção

O enunciado não define as chaves a remover. As escolhidas cobrem todos os casos possíveis.

| Passo | Remover | Posição | Caso | Cadeia antes | Cadeia depois | n | α |
|:-:|:-:|:-:|---|---|---|:-:|:-:|
| 1 | 769 | 6 | meio | 13 → 769 → 76 | 13 → 76 | 9 | 1,2857 |
| 2 | 61 | 5 | fim | 89 → 61 | 89 | 8 | 1,1429 |
| 3 | 190 | 1 | início | 190 → 15 | 15 | 7 | 1,0000 |
| 4 | 322 | 0 | única | 322 | vazia | 6 | 0,8571 |
| 5 | 20 | 6 | ausente, cadeia ocupada | 13 → 76 | 13 → 76 | 6 | 0,8571 |
| 6 | 100 | 2 | ausente, posição vazia | vazia | vazia | 6 | 0,8571 |

Tabela depois das remoções:

```text
+---+
| 0 | -> NULL
+---+
| 1 | -> [15] -> NULL
+---+
| 2 | -> NULL
+---+
| 3 | -> NULL
+---+
| 4 | -> [172] -> [4] -> NULL
+---+
| 5 | -> [89] -> NULL
+---+
| 6 | -> [13] -> [76] -> NULL
+---+
```

Na remoção, se o nó é o primeiro, a posição da tabela passa a apontar para o seguinte. Se está no
meio ou no fim, quem passa a apontar para o seguinte é o nó anterior.

## 4. Fator de carga e distribuição

**Fator de carga final, com as 10 chaves inseridas:**

```text
alfa = n / m = 10 / 7 = 1,4286
```

Há em média 1,43 chave por posição. Como α > 1, há mais chaves do que posições, então colisões são
inevitáveis (princípio da casa dos pombos: pelo menos 10 − 7 = 3). O encadeamento externo continua
funcionando com α > 1, o que seria impossível no endereçamento aberto.

| Posição | 0 | 1 | 2 | 3 | 4 | 5 | 6 |
|---|:-:|:-:|:-:|:-:|:-:|:-:|:-:|
| Chaves | 1 | 2 | 0 | 0 | 2 | 2 | 3 |
| Percentual | 10% | 20% | 0% | 0% | 20% | 20% | 30% |

| Medida | Após inserções | Após remoções |
|---|:-:|:-:|
| n | 10 | 6 |
| **α = n/7** | **1,4286** | **0,8571** |
| Posições vazias | 2 | 3 |
| Maior cadeia | 3 | 2 |
| Comparações por busca com sucesso (média real) | 16/10 = 1,6000 | 8/6 = 1,3333 |
| Valor teórico, 1 + (n−1)/(2m) | 1,6429 | 1,3571 |
| Comparações por busca sem sucesso (média = α) | 1,4286 | 0,8571 |

**A distribuição:**

- Não é perfeitamente uniforme. O ideal com 10 chaves seria três posições com 2 e quatro com 1.
  Aqui há duas vazias e uma com 3.
- Houve 5 colisões, duas a mais que o mínimo de 3, exatamente por causa das duas posições vazias.
- Mesmo assim é compatível com um hash uniforme: com 10 chaves aleatórias esperam-se
  7·(6/7)¹⁰ ≈ 1,5 posições vazias, e a média real de comparações (1,60) ficou junto da teórica (1,64).
- 7 é primo, o que ajuda a evitar padrões, mas é pequeno para 10 chaves. Para manter α ≤ 0,75 seria
  preciso m ≥ 14, por exemplo o primo 17 (α ≈ 0,59), com rehash de todas as chaves.

**Custo das operações:** em média Θ(1 + α) para busca, inserção e remoção. No pior caso, com todas as
chaves na mesma posição, Θ(n). A inserção também percorre a cadeia porque recusa repetidas.

## 5. Implementação em C

```text
src/tabela_hash.h, tabela_hash.c    tabela e cadeias, com nós e ponteiros
src/main.c                          executa as tarefas 1 a 4
tests/test_tabela_hash.c            31 testes
```

| Função | O que faz |
|---|---|
| `funcao_hash` | calcula k mod 7, corrigindo negativos (em C, `-1 % 7` vale `-1`) |
| `inicializar_tabela` | deixa as 7 posições em `NULL` |
| `inserir` | encadeia no final e recusa repetidas |
| `buscar` | percorre só a cadeia de h(k) e conta as comparações |
| `remover` | religa a cadeia e libera o nó |
| `fator_de_carga` | calcula n/m |
| `imprimir_tabela` | desenha as posições e as cadeias |
| `liberar_tabela` | devolve toda a memória |

### Compilação e execução

```bash
make run     # mostra as tarefas 1 a 4
make test    # roda os testes
```

Sem Make:

```bash
cc -std=c99 src/main.c src/tabela_hash.c -o hash
./hash
```

### Testes

| IDs | O que verificam | Resultado |
|---|---|:-:|
| H-01 a H-04 | função hash, negativos e extremos de `int` | 4 de 4 |
| I-01 a I-04 | inserção, colisão e repetidas | 4 de 4 |
| T-00 a T-06 | cada posição da tabela resultante | 7 de 7 |
| B-01 a B-04 | busca e número de comparações | 4 de 4 |
| R-01 a R-07 | todos os casos de remoção | 7 de 7 |
| C-01 a C-05 | fator de carga, cadeias e liberação | 5 de 5 |
| **Total** | | **31 de 31** |
