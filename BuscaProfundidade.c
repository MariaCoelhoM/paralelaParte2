#include <stdio.h>
#include <stdlib.h>

// Estrutura para representar um nó da lista de adjacência
typedef struct No {
    int vertice;
    struct No* proximo;
} No;

// Estrutura para representar o Grafo
typedef struct Grafo {
    int numVertices;
    No** listasAdj; // Vetor de ponteiros para as listas de adjacência
    int* visitado;  // Vetor para acompanhar os vértices já visitados
} Grafo;

// Função para criar um novo nó
No* criarNo(int v) {
    No* novoNo = (No*)malloc(sizeof(No));
    novoNo->vertice = v;
    novoNo->proximo = NULL;
    return novoNo;
}

// Função para inicializar o grafo com N vértices
Grafo* criarGrafo(int vertices) {
    Grafo* grafo = (Grafo*)malloc(sizeof(Grafo));
    grafo->numVertices = vertices;

    // Aloca os vetores do grafo
    grafo->listasAdj = (No**)malloc(vertices * sizeof(No*));
    grafo->visitado = (int*)malloc(vertices * sizeof(int));

    // Inicializa as listas vazias e os visitados como 0 (falso)
    for (int i = 0; i < vertices; i++) {
        grafo->listasAdj[i] = NULL;
        grafo->visitado[i] = 0;
    }
    return grafo;
}

// Função para adicionar uma aresta direcionada (u ➜ v)
void adicionarAresta(Grafo* grafo, int u, int v) {
    No* novoNo = criarNo(v);
    
    // Insere o nó no início da lista de adjacência do vértice u
    novoNo->proximo = grafo->listasAdj[u];
    grafo->listasAdj[u] = novoNo;
}

// Função recursiva que faz o "mergulho" da Busca em Profundidade
void DFS_visita(Grafo* grafo, int u) {
    // Marca o vértice atual como visitado e o exibe
    grafo->visitado[u] = 1;
    printf("%d ", u);

    // Percorre todos os vizinhos do vértice u
    No* temp = grafo->listasAdj[u];
    while (temp != NULL) {
        int v = temp->vertice;
        
        // Se o vizinho ainda não foi visitado, mergulha nele recursivamente
        if (grafo->visitado[v] == 0) {
            DFS_visita(grafo, v);
        }
        temp = temp->proximo; // Avança para o próximo vizinho
    }
}

// Função principal da DFS que garante a varredura de todo o grafo
void DFS(Grafo* grafo, int verticeInicial) {
    printf("Ordem de visita da DFS: ");
    
    // Começa a busca a partir do vértice escolhido pelo usuário
    DFS_visita(grafo, verticeInicial);
    
    // (Opcional) Esse loop garante que mesmo se o grafo for desconectado,
    // todos os outros vértices isolados também serão visitados.
    for (int i = 0; i < grafo->numVertices; i++) {
        if (grafo->visitado[i] == 0) {
            DFS_visita(grafo, i);
        }
    }
    printf("\n");
}

int main() {
    // Cria um grafo com 5 vértices (0, 1, 2, 3 e 4)
    Grafo* g = criarGrafo(5);

    // Montando a estrutura do nosso exemplo:
    // Para que a lista encadeada processe o '1' antes do '2', inserimos o '2' primeiro.
    adicionarAresta(g, 0, 2); // 0 ➜ 2
    adicionarAresta(g, 0, 1); // 0 ➜ 1
    adicionarAresta(g, 1, 3); // 1 ➜ 3
    adicionarAresta(g, 2, 4); // 2 ➜ 4

    // Executa a DFS começando pelo vértice 0
    DFS(g, 0);

    // Liberando a memória alocada antes de fechar o programa
    for (int i = 0; i < 5; i++) {
        No* temp = g->listasAdj[i];
        while (temp != NULL) {
            No* inverter = temp;
            temp = temp->proximo;
            free(inverter);
        }
    }
    free(g->listasAdj);
    free(g->visitado);
    free(g);

    return 0;
}