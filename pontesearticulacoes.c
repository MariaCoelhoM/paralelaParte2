#include <stdio.h>
#include <stdlib.h>

#define MIN(a,b) (((a)<(b))?(a):(b))

typedef struct No {
    int vertice;
    struct No* proximo;
} No;

typedef struct Grafo {
    int numVertices;
    No** listasAdj;
} Grafo;

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
    for (int i = 0; i < vertices; i++) grafo->listasAdj[i] = NULL;
    return grafo;
}

// Como o grafo é não-direcionado, adicionamos u-v e v-u
void adicionarAresta(Grafo* G, int u, int v) {
    No* novoNo = criarNo(v);
    novoNo->proximo = G->listasAdj[u];
    G->listasAdj[u] = novoNo;

    novoNo = criarNo(u);
    novoNo->proximo = G->listasAdj[v];
    G->listasAdj[v] = novoNo;
}

// Vetores de controle globais para o algoritmo de Tarjan
int* num;       // Tempo de descoberta
int* low;       // Menor tempo alcançável
int* pai;       // Pai de cada vértice na árvore DFS
int* eArticulacao; // Vetor booleano para marcar se o vértice é articulação
int tempoGlobal = 0;

void tarjanDFS(Grafo* G, int u) {
    num[u] = low[u] = ++tempoGlobal;
    int filhosRaiz = 0;

    No* temp = G->listasAdj[u];
    while (temp != NULL) {
        int v = temp->vertice;

        // Se v é o pai de u, ignora para não voltar pela mesma aresta imediatamente
        if (v == pai[u]) {
            temp = temp->proximo;
            continue;
        }

        if (num[v] > 0) {
            // Aresta de Retorno: v já foi visitado, atualiza o low de u
            low[u] = MIN(low[u], num[v]);
        } else {
            // Aresta de Árvore: v ainda não foi visitado
            pai[v] = u;
            filhosRaiz++;

            tarjanDFS(G, v);

            // Ao retornar da recursão, atualiza o low de u com o low do filho
            low[u] = MIN(low[u], low[v]);

            // --- TESTE DE PONTE ---
            if (low[v] > num[u]) {
                printf("Ponte Detectada: %d ⇄ %d\n", u, v);
            }

            // --- TESTE DE ARTICULAÇÃO (Caso Geral: não é raiz) ---
            if (pai[u] != -1 && low[v] >= num[u]) {
                eArticulacao[u] = 1;
            }
        }
        temp = temp->proximo;
    }

    // --- TESTE DE ARTICULAÇÃO (Caso Especial: se u for a raiz da DFS) ---
    if (pai[u] == -1 && filhosRaiz > 1) {
        eArticulacao[u] = 1;
    }
}

void encontrarPontesEArticulacoes(Grafo* G) {
    num = (int*)calloc(G->numVertices, sizeof(int));
    low = (int*)calloc(G->numVertices, sizeof(int));
    pai = (int*)malloc(G->numVertices * sizeof(int));
    eArticulacao = (int*)calloc(G->numVertices, sizeof(int));
    tempoGlobal = 0;

    for (int i = 0; i < G->numVertices; i++) pai[i] = -1;

    printf("--- Analisando Pontes ---\n");
    for (int i = 0; i < G->numVertices; i++) {
        if (num[i] == 0) {
            tarjanDFS(G, i);
        }
    }

    printf("\n--- Analisando Articulacoes ---\n");
    int encontrouArticulacao = 0;
    for (int i = 0; i < G->numVertices; i++) {
        if (eArticulacao[i]) {
            printf("Vertice Articulacao Encontrado: %d\n", i);
            encontrouArticulacao = 1;
        }
    }
    if (!encontrouArticulacao) printf("Nenhuma articulacao encontrada.\n");

    // Liberação dos vetores dinâmicos auxiliares
    free(num); free(low); free(pai); free(eArticulacao);
}

int main() {
    // Criando um grafo clássico de teste com 5 vértices:
    // Ciclo (0-1-2) conectado a um "rabicho" linear (2-3-4)
    int vertices = 5;
    Grafo* g = criarGrafo(vertices);

    adicionarAresta(g, 0, 1);
    adicionarAresta(g, 1, 2);
    adicionarAresta(g, 2, 0); // fecha o triângulo
    adicionarAresta(g, 2, 3); // ponte que liga o triângulo ao resto
    adicionarAresta(g, 3, 4); // ponte final

    printf("--- Iniciando Algoritmo de Tarjan ---\n");
    encontrarPontesEArticulacoes(g);

    // Liberando a memória do grafo
    for (int i = 0; i < vertices; i++) {
        No* temp = g->listasAdj[i];
        while (temp != NULL) {
            No* apagar = temp;
            temp = temp->proximo;
            free(apagar);
        }
    }
    free(g->listasAdj);
    free(g);

    return 0;
}