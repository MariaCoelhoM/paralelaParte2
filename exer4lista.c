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

// Função auxiliar recursiva que reconstrói o caminho
int encontrarCaminhoListaAux(GrafoLista* G, int u, int t, int* visitado, int* caminho, int* indiceCaminho) {
    visitado[u] = 1;
    caminho[*indiceCaminho] = u;
    (*indiceCaminho)++;

    if (u == t) return 1;

    No* temp = G->listasAdj[u];
    while (temp != NULL) {
        int w = temp->vertice;
        if (visitado[w] == 0) {
            if (encontrarCaminhoListaAux(G, w, t, visitado, caminho, indiceCaminho)) {
                return 1;
            }
        }
        temp = temp->proximo;
    }

    // Backtracking: se nenhum vizinho levou a t, remove u do caminho
    (*indiceCaminho)--;
    return 0;
}

// Função principal exigida pelo exercício
void exibirCaminhoLista(GrafoLista* G, int s, int t) {
    int* visitado = (int*)calloc(G->numVertices, sizeof(int));
    int* caminho = (int*)malloc(G->numVertices * sizeof(int));
    int indiceCaminho = 0;

    printf("Buscando caminho de %d ate %d (Lista):\n", s, t);

    if (encontrarCaminhoListaAux(G, s, t, visitado, caminho, &indiceCaminho)) {
        printf("Caminho encontrado: ");
        for (int i = 0; i < indiceCaminho; i++) {
            printf("%d", caminho[i]);
            if (i < indiceCaminho - 1) printf(" ➜ ");
        }
        printf("\n");
    } else {
        printf("Nao existe caminho entre os vertices %d e %d.\n", s, t);
    }

    free(visitado);
    free(caminho);
}

// --- TESTE ---
int main() {
    int n = 5;
    GrafoLista* g = criarGrafoLista(n);

    // Montando o mesmo grafo: 0 ➜ 1 ➜ 2 ➜ 4 (com o beco sem saída em 3)
    adicionarAresta(g, 0, 1);
    adicionarAresta(g, 1, 3); // Inserindo os caminhos saindo de 1
    adicionarAresta(g, 1, 2);
    adicionarAresta(g, 2, 4);

    exibirCaminhoLista(g, 0, 4);

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