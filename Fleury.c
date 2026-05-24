#include <stdio.h>
#include <stdlib.h>

typedef struct GrafoMatriz {
    int numVertices;
    int** matriz;
} GrafoMatriz;

GrafoMatriz* criarGrafo(int vertices) {
    GrafoMatriz* G = (GrafoMatriz*)malloc(sizeof(GrafoMatriz));
    G->numVertices = vertices;
    G->matriz = (int**)malloc(vertices * sizeof(int*));
    for (int i = 0; i < vertices; i++) {
        G->matriz[i] = (int*)calloc(vertices, sizeof(int));
    }
    return G;
}

void adicionarAresta(GrafoMatriz* G, int u, int v) {
    G->matriz[u][v] = 1;
    G->matriz[v][u] = 1; // Grafo não-direcionado
}

// DFS simples apenas para contar quantos vértices conseguimos alcançar
void dfsContar(GrafoMatriz* G, int u, int* visitado, int* contador) {
    visitado[u] = 1;
    (*contador)++;
    for (int v = 0; v < G->numVertices; v++) {
        if (G->matriz[u][v] && !visitado[v]) {
            dfsContar(G, v, visitado, contador);
        }
    }
}

// Função que verifica se a aresta u-v é uma ponte válida para ser percorrida
int ehArestaValida(GrafoMatriz* G, int u, int v) {
    // Caso 1: Se v for o único vizinho de u, não temos escolha, temos que ir por ela
    int totalVizinhos = 0;
    for (int i = 0; i < G->numVertices; i++) {
        if (G->matriz[u][i]) totalVizinhos++;
    }
    if (totalVizinhos == 1) return 1;

    // Caso 2: Se houver mais opções, checamos se u-v é uma ponte
    // 2.1. Conta quantos vértices alcançamos com a aresta no grafo
    int* visitado = (int*)calloc(G->numVertices, sizeof(int));
    int contComAresta = 0;
    dfsContar(G, u, visitado, &contComAresta);

    // 2.2. Remove a aresta temporariamente
    G->matriz[u][v] = G->matriz[v][u] = 0;

    // 2.3. Conta novamente quantos vértices alcançamos sem ela
    for (int i = 0; i < G->numVertices; i++) visitado[i] = 0;
    int contSemAresta = 0;
    dfsContar(G, u, visitado, &contSemAresta);

    // 2.4. Coloca a aresta de volta na matriz
    G->matriz[u][v] = G->matriz[v][u] = 1;
    free(visitado);

    // Se o número de vértices alcançáveis diminuiu, era uma ponte!
    // Retornamos 0 (falso) se for ponte, indicando que não devemos ir por aí se houver opção.
    return (contComAresta > contSemAresta) ? 0 : 1;
}

// --- ALGORITMO DE FLEURY ---
void rodarFleury(GrafoMatriz* G, int u) {
    // Percorre todos os possíveis vizinhos do vértice atual u
    for (int v = 0; v < G->numVertices; v++) {
        // Se a aresta u-v existe e passa no teste de não ser uma ponte prejudicial
        if (G->matriz[u][v] && ehArestaValida(G, u, v)) {
            printf("%d ➜ %d  ", u, v);
            
            // "Queima" a aresta removendo-a definitivamente do grafo
            G->matriz[u][v] = G->matriz[v][u] = 0;
            
            // Avança para o próximo vértice
            rodarFleury(G, v);
            return;
        }
    }
}

int main() {
    // Mesma gravata borboleta do teste anterior (0 é o centro)
    int vertices = 5;
    GrafoMatriz* g = criarGrafo(vertices);

    adicionarAresta(g, 0, 1);
    adicionarAresta(g, 1, 2);
    adicionarAresta(g, 2, 0);
    adicionarAresta(g, 0, 3);
    adicionarAresta(g, 3, 4);
    adicionarAresta(g, 4, 0);

    printf("--- Executando Algoritmo de Fleury ---\n");
    printf("Caminho percorrido: ");
    
    // Começa o Fleury a partir do vértice 0
    rodarFleury(g, 0);
    printf("\n");

    // Liberação de memória
    for (int i = 0; i < vertices; i++) free(g->matriz[i]);
    free(g->matriz);
    free(g);

    return 0;
}