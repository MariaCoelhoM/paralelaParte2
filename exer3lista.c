#include <stdio.h>
#include <stdlib.h>

// Definição das estruturas para a Lista
typedef struct No {
    int vertice;
    struct No* proximo;
} No;

typedef struct GrafoLista {
    int numVertices;
    No** listasAdj;
} GrafoLista;

// Função auxiliar para criar um nó na lista de adjacência
No* criarNo(int v) {
    No* novoNo = (No*)malloc(sizeof(No));
    novoNo->vertice = v;
    novoNo->proximo = NULL;
    return novoNo;
}

// Função auxiliar para criar o grafo com N vértices
GrafoLista* criarGrafoLista(int vertices) {
    GrafoLista* grafo = (GrafoLista*)malloc(sizeof(GrafoLista));
    grafo->numVertices = vertices;
    grafo->listasAdj = (No**)malloc(vertices * sizeof(No*));
    
    for (int i = 0; i < vertices; i++) {
        grafo->listasAdj[i] = NULL;
    }
    return grafo;
}

// Função auxiliar para adicionar uma aresta direcionada (u -> v)
void adicionarArestaDirecionada(GrafoLista* G, int u, int v) {
    No* novoNo = criarNo(v);
    novoNo->proximo = G->listasAdj[u];
    G->listasAdj[u] = novoNo;
}

// --- FUNÇÃO AUXILIAR RECURSIVA DO EXERCÍCIO ---
int existeCaminhoListaAux(GrafoLista* G, int u, int t, int* visitado) {
    if (u == t) return 1;

    visitado[u] = 1;

    // Ponteiro para começar a navegar pelos vizinhos diretos de u
    No* temp = G->listasAdj[u];
    while (temp != NULL) {
        int w = temp->vertice;

        // Se o vizinho w não foi visitado
        if (visitado[w] == 0) {
            if (existeCaminhoListaAux(G, w, t, visitado) == 1) {
                return 1;
            }
        }
        temp = temp->proximo; // Avança para o próximo vizinho da lista
    }
    return 0;
}

// --- FUNÇÃO PRINCIPAL EXIGIDA PELO EXERCÍCIO ---
int existeCaminhoLista(GrafoLista* G, int s, int t) {
    // Aloca e inicializa o vetor de visitados com 0
    int* visitado = (int*)calloc(G->numVertices, sizeof(int));
    
    int resultado = existeCaminhoListaAux(G, s, t, visitado);
    
    free(visitado);
    return resultado;
}

// --- FUNÇÃO MAIN PARA TESTAR O PROGRAMA ---
int main() {
    int n = 4; // Criando um grafo com 4 vértices (0, 1, 2 e 3)
    GrafoLista* g = criarGrafoLista(n);

    // Montando o mesmo caminho direcionado: 0 ➜ 1 ➜ 2 ➜ 3
    // Como a inserção na lista é no início, vamos inserir na ordem que mantém a lógica
    adicionarArestaDirecionada(g, 0, 1);
    adicionarArestaDirecionada(g, 1, 2);
    adicionarArestaDirecionada(g, 2, 3);

    printf("--- Testando Busca em Lista de Adjaciencia ---\n");

    // Teste 1: Existe caminho entre 0 e 3? (Deve ser SIM)
    int s1 = 0, t1 = 3;
    if (existeCaminhoLista(g, s1, t1) == 1) {
        printf("[SUCESSO] Existe caminho de %d ate %d!\n", s1, t1);
    } else {
        printf("[FALHA] Nao existe caminho de %d ate %d.\n", s1, t1);
    }

    // Teste 2: Existe caminho de volta entre 3 e 0? (Deve ser NÃO)
    int s2 = 3, t2 = 0;
    if (existeCaminhoLista(g, s2, t2) == 1) {
        printf("[SUCESSO] Existe caminho de %d ate %d!\n", s2, t2);
    } else {
        printf("[FALHA] Nao existe caminho de %d ate %d.\n", s2, t2);
    }

    // Liberando a memória alocada para as listas
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