CC ?= cc
CFLAGS ?= -std=c99 -Wall -Wextra -pedantic

all: build/hash

build/hash: src/main.c src/tabela_hash.c src/tabela_hash.h
	@mkdir -p build
	$(CC) $(CFLAGS) src/main.c src/tabela_hash.c -o $@

build/testes: tests/test_tabela_hash.c src/tabela_hash.c src/tabela_hash.h
	@mkdir -p build
	$(CC) $(CFLAGS) tests/test_tabela_hash.c src/tabela_hash.c -o $@ -lm

run: build/hash
	./build/hash

test: build/testes
	./build/testes

clean:
	rm -rf build

.PHONY: all run test clean
