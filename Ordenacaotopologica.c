#include <stdio.h>
#include <stdlib.h>

#define BRANCO 0
#define CINZA  1
#define PRETO  2

// Estrutura do nó da lista de adjacência
typedef struct No {
    int vertice;
    struct No* proximo;
} No;

// Estrutura do Grafo
typedef struct Grafo {
    int numVertices;
    No** listasAdj;
    int* estado;
} Grafo;

// Vetores globais auxiliares para gerenciar a ordenação topológica
int* pilhaTopologica;
int topoPilha = -1;

No* criarNo(int v) {
    No* novoNo = (No*)malloc(sizeof(No));
    novoNo->vertice = v;
    novoNo->proximo = NULL;
    return novoNo;
}

Grafo* criarGrafo(int vertices) {
    Grafo* grafo = (Grafo*)malloc(sizeof(Grafo));
    grafo->numVertices = vertices;
    grafo->listasAdj = (No**)malloc(vertices * sizeof(No*));
    grafo->estado = (int*)malloc(vertices * sizeof(int));

    for (int i = 0; i < vertices; i++) {
        grafo->listasAdj[i] = NULL;
        grafo->estado[i] = BRANCO;
    }
    return grafo;
}

// Adiciona uma aresta direcionada (u ➜ v)
void adicionarAresta(Grafo* grafo, int u, int v) {
    No* novoNo = criarNo(v);
    novoNo->proximo = grafo->listasAdj[u];
    grafo->listasAdj[u] = novoNo;
}

// Função auxiliar da DFS que realiza a ordenação
int ordenacaoTopologicaAux(Grafo* grafo, int u) {
    grafo->estado[u] = CINZA;

    No* temp = grafo->listasAdj[u];
    while (temp != NULL) {
        int v = temp->vertice;

        // Se detectar um ciclo, a ordenação topológica é impossível!
        if (grafo->estado[v] == CINZA) {
            return 0; 
        }

        if (grafo->estado[v] == BRANCO) {
            if (!ordenacaoTopologicaAux(grafo, v)) {
                return 0;
            }
        }
        temp = temp->proximo;
    }

    grafo->estado[u] = PRETO;
    
    // Sacada do algoritmo: quando o vértice é finalizado, vai para a pilha
    pilhaTopologica[++topoPilha] = u;
    return 1;
}

// Função principal que orquestra a Ordenação Topológica
void ordenacaoTopologica(Grafo* grafo) {
    pilhaTopologica = (int*)malloc(grafo->numVertices * sizeof(int));
    topoPilha = -1;

    // Garante a varredura por todos os componentes do grafo
    for (int i = 0; i < grafo->numVertices; i++) {
        if (grafo->estado[i] == BRANCO) {
            if (!ordenacaoTopologicaAux(grafo, i)) {
                printf("[ERRO] O grafo possui um ciclo! Ordenacao topologica impossivel.\n");
                free(pilhaTopologica);
                return;
            }
        }
    }

    // Exibe o resultado desempilhando os elementos (do último finalizado ao primeiro)
    printf("Ordenacao Topologica: ");
    while (topoPilha >= 0) {
        printf("%d ", pilhaTopologica[topoPilha--]);
    }
    printf("\n");

    free(pilhaTopologica);
}

// --- TESTE DO ALGORITMO ---
int main() {
    // Criando um grafo com 6 vértices (representando dependências de tarefas)
    int vertices = 6;
    Grafo* g = criarGrafo(vertices);

    // Configurando as dependências:
    // 5 ➜ 2, 5 ➜ 0, 4 ➜ 0, 4 ➜ 1, 2 ➜ 3, 3 ➜ 1
    adicionarAresta(g, 5, 2);
    adicionarAresta(g, 5, 0);
    adicionarAresta(g, 4, 0);
    adicionarAresta(g, 4, 1);
    adicionarAresta(g, 2, 3);
    adicionarAresta(g, 3, 1);

    printf("--- Executando Ordenacao Topologica ---\n");
    ordenacaoTopologica(g);

    // Liberando memória
    for (int i = 0; i < vertices; i++) {
        No* temp = g->listasAdj[i];
        while (temp != NULL) {
            No* inverter = temp;
            temp = temp->proximo;
            free(inverter);
        }
    }
    free(g->listasAdj);
    free(g->estado);
    free(g);

    return 0;
}