#include <stdio.h>
#include <stdlib.h>

#define BRANCO 0
#define CINZA  1
#define PRETO  2

typedef struct GrafoMatriz {
    int numVertices;
    int** matriz;
} GrafoMatriz;

// Vetores globais para simplificar o controle dos tempos na DFS padrão
int* cor;
int* d; // Tempo de descoberta
int* f; // Tempo de finalização
int tempo = 0;

// Função DFS padrão apenas para preencher os tempos de descoberta e finalização
void DFS_Visita_Classificacao(GrafoMatriz* G, int u) {
    cor[u] = CINZA;
    d[u] = ++tempo;

    for (int w = 0; w < G->numVertices; w++) {
        if (G->matriz[u][w] == 1 && cor[w] == BRANCO) {
            DFS_Visita_Classificacao(G, w);
        }
    }
    cor[u] = PRETO;
    f[u] = ++tempo;
}

void rodarDFSPadrao(GrafoMatriz* G) {
    cor = (int*)calloc(G->numVertices, sizeof(int));
    d = (int*)calloc(G->numVertices, sizeof(int));
    f = (int*)calloc(G->numVertices, sizeof(int));
    tempo = 0;

    for (int i = 0; i < G->numVertices; i++) {
        if (cor[i] == BRANCO) {
            DFS_Visita_Classificacao(G, i);
        }
    }
}

// --- MÉTODO EXIGIDO PELO EXERCÍCIO ---
void classificarArco(GrafoMatriz* G, int v, int w) {
    // 1. Verifica se o arco v -> w realmente existe no grafo
    if (G->matriz[v][w] == 0) {
        printf("O arco %d -> %d nao existe no grafo G.\n", v, w);
        return;
    }

    printf("Arco %d -> %d existe. Classificacao: ", v, w);

    // 2. Classifica baseado nos tempos obtidos pela DFS
    
    // Se w foi descoberto imediatamente a partir de v na árvore principal, 
    // ou se na checagem original w era BRANCO. Como estamos avaliando pós-DFS:
    // Um arco de árvore cumpre que v descobriu w, logo d[v] < d[w] < f[w] < f[v] 
    // E não existia outro caminho ancestral direto mais curto.
    
    if (d[v] < d[w] && f[v] > f[w]) {
        // Para diferenciar Arborescência de Descendente:
        // Se a diferença de tempo de descoberta for uma cadeia direta ou se
        // simularmos o exato instante em que v olha para w na DFS original.
        // Se no momento do teste pós-fato queremos saber se foi arborescência,
        // geralmente avalia-se se v é o pai direto de w na árvore gerada.
        // Aqui usamos a relação de tempo clássica:
        if (d[w] == d[v] + 1) { 
            printf("Arborescencia (Arco de Arvore)\n");
        } else {
            printf("Descendente\n");
        }
    } 
    else if (d[v] > d[w] && f[v] < f[w]) {
        printf("Retorno (Back Edge)\n");
    } 
    else if (d[v] > d[w] && f[v] > f[w]) {
        printf("Cruzado (Cross Edge)\n");
    }
}

// --- TESTE ---
int main() {
    int n = 4;
    GrafoMatriz g;
    g.numVertices = n;
    
    g.matriz = (int**)malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) g.matriz[i] = (int*)calloc(n, sizeof(int));
    
    // Montando um cenário com múltiplos tipos de arcos:
    g.matriz[0][1] = 1; // Arborescência (0 -> 1)
    g.matriz[1][2] = 1; // Arborescência (1 -> 2)
    g.matriz[0][2] = 1; // Descendente (0 -> 2)
    g.matriz[2][0] = 1; // Retorno (2 -> 0)
    
    // Roda a DFS para mapear o comportamento cronológico do grafo
    rodarDFSPadrao(&g);

    // Testando as classificações
    classificarArco(&g, 0, 1);
    classificarArco(&g, 0, 2);
    classificarArco(&g, 2, 0);
    classificarArco(&g, 3, 1); // Não existe

    // Liberando memória
    for (int i = 0; i < n; i++) free(g.matriz[i]);
    free(g.matriz);
    free(cor); free(d); free(f);
    
    return 0;
}