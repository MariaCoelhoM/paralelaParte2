#include <stdio.h>
#include <stdlib.h>

// Definição da estrutura para a Matriz
typedef struct GrafoMatriz {
    int numVertices;
    int** matriz;
} GrafoMatriz;

// Função auxiliar recursiva para a busca na Matriz
int existeCaminhoMatrizAux(GrafoMatriz* G, int u, int t, int* visitado) {
    // Se chegamos no vértice de destino, caminho encontrado!
    if (u == t) return 1;

    visitado[u] = 1; // Marcar o vértice atual como visitado

    // Percorre todos os possíveis vértices w para encontrar vizinhos
    for (int w = 0; w < G->numVertices; w++) {
        // Se existe a aresta (u -> w) E w não foi visitado
        if (G->matriz[u][w] == 1 && visitado[w] == 0) {
            // Se a sub-busca a partir de w encontrar o destino, propaga o 1
            if (existeCaminhoMatrizAux(G, w, t, visitado) == 1) {
                return 1;
            }
        }
    }
    return 0; // Se saiu do loop e não achou t por esse caminho
}

// Função principal exigida pelo exercício
int existeCaminhoMatriz(GrafoMatriz* G, int s, int t) {
    // Aloca e inicializa o vetor de visitados com 0 (falso)
    int* visitado = (int*)calloc(G->numVertices, sizeof(int));
    
    int resultado = existeCaminhoMatrizAux(G, s, t, visitado);
    
    free(visitado); // Libera a memória alocada
    return resultado;
}

// --- FUNÇÃO MAIN PARA TESTAR O PROGRAMA ---
int main() {
    int n = 4; // Criando um grafo com 4 vértices (0, 1, 2 e 3)
    
    GrafoMatriz g;
    g.numVertices = n;
    
    // Alocação dinâmica da matriz de adjacência
    g.matriz = (int**)malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        g.matriz[i] = (int*)calloc(n, sizeof(int));
    }
    
    // Configurando as arestas do caminho: 0 ➜ 1 ➜ 2 ➜ 3
    g.matriz[0][1] = 1; 
    g.matriz[1][2] = 1;
    g.matriz[2][3] = 1;

    printf("--- Testando Busca em Matriz de Adjaciencia ---\n");

    // Teste 1: Existe caminho entre 0 e 3? (Deve ser SIM)
    int s1 = 0, t1 = 3;
    if (existeCaminhoMatriz(&g, s1, t1) == 1) {
        printf("[SUCESSO] Existe caminho de %d ate %d!\n", s1, t1);
    } else {
        printf("[FALHA] Nao existe caminho de %d ate %d.\n", s1, t1);
    }

    // Teste 2: Existe caminho de volta entre 3 e 0? (Deve ser NÃO, pois o grafo é direcionado)
    int s2 = 3, t2 = 0;
    if (existeCaminhoMatriz(&g, s2, t2) == 1) {
        printf("[SUCESSO] Existe caminho de %d ate %d!\n", s2, t2);
    } else {
        printf("[FALHA] Nao existe caminho de %d ate %d.\n", s2, t2);
    }

    // Liberando a memória alocada da matriz
    for (int i = 0; i < n; i++) {
        free(g.matriz[i]);
    }
    free(g.matriz);

    return 0;
}