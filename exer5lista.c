#include <stdio.h>
#include <stdlib.h>

typedef struct No {
    int vertice;
    struct No* proximo;
} No;

typedef struct GrafoLista {
    int numVertices;
    No** listasAdj;
} GrafoLista;

// --- ESTRUTURA DA PILHA AUXILIAR ---
typedef struct Pilha {
    int topo;
    int capacidade;
    int* itens;
} Pilha;

Pilha* criarPilha(int capacidade) {
    Pilha* p = (Pilha*)malloc(sizeof(Pilha));
    p->capacidade = capacidade;
    p->topo = -1;
    p->itens = (int*)malloc(capacidade * sizeof(int));
    return p;
}

void push(Pilha* p, int v) {
    p->itens[++(p->topo)] = v;
}

int pop(Pilha* p) {
    return p->itens[(p->topo)--];
}

int pilhaVazia(Pilha* p) {
    return p->topo == -1;
}

void liberarPilha(Pilha* p) {
    free(p->itens);
    free(p);
}

// --- FUNÇÕES AUXILIARES DO GRAFO ---
No* criarNo(int v) {
    No* novoNo = (No*)malloc(sizeof(No));
    novoNo->vertice = v;
    novoNo->proximo = NULL;
    return novoNo;
}

GrafoLista* criarGrafoLista(int vertices) {
    GrafoLista* grafo = (GrafoLista*)malloc(sizeof(GrafoLista));
    grafo->numVertices = vertices;
    grafo->listasAdj = (No**)malloc(vertices * sizeof(No*));
    for (int i = 0; i < vertices; i++) grafo->listasAdj[i] = NULL;
    return grafo;
}

void adicionarAresta(GrafoLista* G, int u, int v) {
    No* novoNo = criarNo(v);
    novoNo->proximo = G->listasAdj[u];
    G->listasAdj[u] = novoNo;
}

// --- FUNÇÃO PRINCIPAL ITERATIVA (LISTA) ---
void DFSIterativaLista(GrafoLista* G, int verticeInicial) {
    int* visitado = (int*)calloc(G->numVertices, sizeof(int));
    Pilha* p = criarPilha(G->numVertices * G->numVertices);

    push(p, verticeInicial);

    printf("Ordem de visita (Iterativa - Lista): ");

    while (!pilhaVazia(p)) {
        int u = pop(p);

        if (visitado[u] == 0) {
            visitado[u] = 1;
            printf("%d ", u);

            // Como a nossa lista guarda os vizinhos inseridos mais recentemente no início,
            // ao colocar na pilha eles vão sair na ordem correta da busca em profundidade.
            No* temp = G->listasAdj[u];
            while (temp != NULL) {
                int w = temp->vertice;
                if (visitado[w] == 0) {
                    push(p, w);
                }
                temp = temp->proximo;
            }
        }
    }
    printf("\n");

    free(visitado);
    liberarPilha(p);
}

// --- TESTE ---
int main() {
    int n = 5;
    GrafoLista* g = criarGrafoLista(n);

    // Mesma estrutura: 0 ➜ 1 e 2; 1 ➜ 3; 2 ➜ 4
    // Adicionamos o 2 antes do 1 para que o 1 fique no início da lista do 0
    adicionarAresta(g, 0, 2);
    adicionarAresta(g, 0, 1);
    adicionarAresta(g, 1, 3);
    adicionarAresta(g, 2, 4);

    DFSIterativaLista(g, 0);

    for (int i = 0; i < n; i++) {
        No* temp = g->listasAdj[i];
        while (temp != NULL) {
            No* inverter = temp;
            temp = temp->proximo;
            free(inverter);
        }
    }
    free(g->listasAdj);
    free(g);
    return 0;
}