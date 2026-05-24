#include <stdio.h>
#include <stdlib.h>

// Estrutura do nó da lista de adjacência
typedef struct No {
    int vertice;
    int idAresta; // Identificador único para marcar a aresta como usada
    struct No* proximo;
} No;

typedef struct Grafo {
    int numVertices;
    int numArestas;
    No** listasAdj;
    int* grau;
} Grafo;

// Estruturas auxiliares de Lista/Pilha para o algoritmo
typedef struct ListaNos {
    int vertice;
    struct ListaNos* proximo;
} ListaNos;

No* criarNo(int v, int id) {
    No* novoNo = (No*)malloc(sizeof(No));
    novoNo->vertice = v;
    novoNo->idAresta = id;
    novoNo->proximo = NULL;
    return novoNo;
}

Grafo* criarGrafo(int vertices) {
    Grafo* grafo = (Grafo*)malloc(sizeof(Grafo));
    grafo->numVertices = vertices;
    grafo->numArestas = 0;
    grafo->listasAdj = (No**)malloc(vertices * sizeof(No*));
    grafo->grau = (int*)calloc(vertices, sizeof(int));
    for (int i = 0; i < vertices; i++) grafo->listasAdj[i] = NULL;
    return grafo;
}

// Adiciona aresta não-direcionada vinculando ambas ao mesmo ID de aresta
void adicionarAresta(Grafo* G, int u, int v) {
    int id = G->numArestas++;
    
    No* novoNo = criarNo(v, id);
    novoNo->proximo = G->listasAdj[u];
    G->listasAdj[u] = novoNo;

    novoNo = criarNo(u, id);
    novoNo->proximo = G->listasAdj[v];
    G->listasAdj[v] = novoNo;

    G->grau[u]++;
    G->grau[v]++;
}

// Funções utilitárias para gerenciar o caminho final de Hierholzer
void inserirInicio(ListaNos** topo, int v) {
    ListaNos* novo = (ListaNos*)malloc(sizeof(ListaNos));
    novo->vertice = v;
    novo->proximo = *topo;
    *topo = novo;
}

// --- ALGORITMO DE HIERHOLZER ---
void encontrarCicloEuleriano(Grafo* G) {
    // 1. Verificar condição de Euler: todos os vértices devem ter grau par
    for (int i = 0; i < G->numVertices; i++) {
        if (G->grau[i] % 2 != 0) {
            printf("[ERRO] O grafo nao eh Euleriano (vertice %d possui grau impar).\n", i);
            return;
        }
    }

    // Vetor para marcar se uma aresta (por ID) já foi percorrida
    int* arestaUsada = (int*)calloc(G->numArestas, sizeof(int));
    
    // Pilha temporária para a DFS de caminhos e lista para armazenar o circuito final
    int* pilhaCaminho = (int*)malloc((G->numArestas + 1) * sizeof(int));
    int topoPilha = 0;
    
    ListaNos* circuitoFinal = NULL;

    // Começamos a partir do vértice 0 (ou qualquer vértice de grau > 0)
    pilhaCaminho[topoPilha] = 0;
    int verticeAtual = 0;

    while (topoPilha >= 0) {
        verticeAtual = pilhaCaminho[topoPilha];

        // Procurar uma aresta não utilizada saindo do vértice atual
        No* temp = G->listasAdj[verticeAtual];
        while (temp != NULL && arestaUsada[temp->idAresta]) {
            temp = temp->proximo;
        }

        if (temp != NULL) {
            // Se achou uma aresta livre, marca como usada e avança empilhando o vizinho
            arestaUsada[temp->idAresta] = 1;
            topoPilha++;
            pilhaCaminho[topoPilha] = temp->vertice;
        } else {
            // Se o vértice não tem mais arestas livres, ele faz parte do circuito final
            inserirInicio(&circuitoFinal, verticeAtual);
            topoPilha--; // Desempilha para fazer o backtracking
        }
    }

    // Exibir o circuito obtido
    printf("Ciclo Euleriano Encontrado (Hierholzer): ");
    ListaNos* percorrer = circuitoFinal;
    while (percorrer != NULL) {
        printf("%d", percorrer->vertice);
        if (percorrer->proximo != NULL) printf(" ➜ ");
        ListaNos* apagar = percorrer;
        percorrer = percorrer->proximo;
        free(apagar);
    }
    printf("\n");

    free(arestaUsada);
    free(pilhaCaminho);
}

// --- TESTE ---
int main() {
    // Criando um grafo euleriano clássico com 5 vértices em formato de "gravata borboleta"
    // Vértice 0 é o nó central de cruzamento das duas asas
    int vertices = 5;
    Grafo* g = criarGrafo(vertices);

    // Asa esquerda da gravata (Triângulo 0-1-2)
    adicionarAresta(g, 0, 1);
    adicionarAresta(g, 1, 2);
    adicionarAresta(g, 2, 0);

    // Asa direita da gravata (Triângulo 0-3-4)
    adicionarAresta(g, 0, 3);
    adicionarAresta(g, 3, 4);
    adicionarAresta(g, 4, 0);

    printf("--- Executando Algoritmo de Hierholzer ---\n");
    encontrarCicloEuleriano(g);

    // Liberando memória do grafo
    for (int i = 0; i < vertices; i++) {
        No* temp = g->listasAdj[i];
        while (temp != NULL) {
            No* apagar = temp;
            temp = temp->proximo;
            free(apagar);
        }
    }
    free(g->listasAdj);
    free(g->grau);
    free(g);

    return 0;
}