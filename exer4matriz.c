#include <stdio.h>
#include <stdlib.h>

typedef struct GrafoMatriz {
    int numVertices;
    int** matriz;
} GrafoMatriz;

// Função auxiliar recursiva que reconstrói o caminho
int encontrarCaminhoMatrizAux(GrafoMatriz* G, int u, int t, int* visitado, int* caminho, int* indiceCaminho) {
    visitado[u] = 1;
    caminho[*indiceCaminho] = u; // Salva o vértice atual no caminho
    (*indiceCaminho)++;

    // Se chegamos ao destino, o caminho está completo
    if (u == t) return 1;

    // Varre a linha da matriz para achar vizinhos de u
    for (int w = 0; w < G->numVertices; w++) {
        if (G->matriz[u][w] == 1 && visitado[w] == 0) {
            // Se a busca a partir de w encontrar o destino, propaga o sucesso
            if (encontrarCaminhoMatrizAux(G, w, t, visitado, caminho, indiceCaminho)) {
                return 1;
            }
        }
    }

    // Se chegou aqui, o vértice u não leva ao destino t (Backtracking)
    (*indiceCaminho)--; // Remove o vértice do caminho
    return 0;
}

// Função principal exigida pelo exercício
void exibirCaminhoMatriz(GrafoMatriz* G, int s, int t) {
    int* visitado = (int*)calloc(G->numVertices, sizeof(int));
    int* caminho = (int*)malloc(G->numVertices * sizeof(int));
    int indiceCaminho = 0;

    printf("Buscando caminho de %d ate %d (Matriz):\n", s, t);

    if (encontrarCaminhoMatrizAux(G, s, t, visitado, caminho, &indiceCaminho)) {
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
    GrafoMatriz g;
    g.numVertices = n;
    
    g.matriz = (int**)malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) g.matriz[i] = (int*)calloc(n, sizeof(int));
    
    // Configurando um grafo de teste: 0 ➜ 1 ➜ 2 ➜ 4 (e um desvio sem saída 1 ➜ 3)
    g.matriz[0][1] = 1;
    g.matriz[1][2] = 1;
    g.matriz[1][3] = 1; // Beco sem saída
    g.matriz[2][4] = 1;

    exibirCaminhoMatriz(&g, 0, 4);

    for (int i = 0; i < n; i++) free(g.matriz[i]);
    free(g.matriz);
    return 0;
}