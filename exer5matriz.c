#include <stdio.h>
#include <stdlib.h>

typedef struct GrafoMatriz {
    int numVertices;
    int** matriz;
} GrafoMatriz;

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

// --- FUNÇÃO PRINCIPAL ITERATIVA (MATRIZ) ---
void DFSIterativaMatriz(GrafoMatriz* G, int verticeInicial) {
    int* visitado = (int*)calloc(G->numVertices, sizeof(int));
    Pilha* p = criarPilha(G->numVertices * G->numVertices); // Garante espaço para as arestas

    // Começa inserindo o vértice inicial na pilha
    push(p, verticeInicial);

    printf("Ordem de visita (Iterativa - Matriz): ");

    while (!pilhaVazia(p)) {
        int u = pop(p);

        // Se u ainda não foi visitado, processa ele agora
        if (visitado[u] == 0) {
            visitado[u] = 1;
            printf("%d ", u);

            // Para manter a mesma ordem da recursiva (esquerda para a direita),
            // varremos os vizinhos de trás para frente (do maior para o menor),
            // já que a pilha inverte a ordem de saída.
            for (int w = G->numVertices - 1; w >= 0; w--) {
                if (G->matriz[u][w] == 1 && visitado[w] == 0) {
                    push(p, w);
                }
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
    GrafoMatriz g;
    g.numVertices = n;
    
    g.matriz = (int**)malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) g.matriz[i] = (int*)calloc(n, sizeof(int));
    
    // Configurando conexões: 0 ligado a 1 e 2; 1 ligado a 3; 2 ligado a 4
    g.matriz[0][1] = 1; g.matriz[0][2] = 1;
    g.matriz[1][3] = 1;
    g.matriz[2][4] = 1;

    DFSIterativaMatriz(&g, 0);

    for (int i = 0; i < n; i++) free(g.matriz[i]);
    free(g.matriz);
    return 0;
}