#include <stdio.h>
#include <stdlib.h>

#define FALSO 0
#define VERDADEIRO 1

// Estrutura do nó da lista de adjacência
typedef struct No {
    int vertice;
    struct No* proximo;
} No;

// Estrutura do Grafo
typedef struct Grafo {
    int numVertices;
    No** listasAdj;
} Grafo;

// Estrutura para uma pilha simples (usada para guardar a ordem de finalização)
typedef struct Pilha {
    int topo;
    int* itens;
} Pilha;

// --- FUNÇÕES DA PILHA ---
Pilha* criarPilha(int capacidade) {
    Pilha* p = (Pilha*)malloc(sizeof(Pilha));
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

// --- FUNÇÕES DO GRAFO ---
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
    for (int i = 0; i < vertices; i++) {
        grafo->listasAdj[i] = NULL;
    }
    return grafo;
}

void adicionarAresta(Grafo* G, int u, int v) {
    No* novoNo = criarNo(v);
    novoNo->proximo = G->listasAdj[u];
    G->listasAdj[u] = novoNo;
}

// --- PASSO 2: INVERSÃO DO GRAFO (TRANSPOSIÇÃO) ---
Grafo* obterGrafoTransposto(Grafo* G) {
    Grafo* transposto = criarGrafo(G->numVertices);
    for (int v = 0; v < G->numVertices; v++) {
        No* temp = G->listasAdj[v];
        while (temp != NULL) {
            // Se existia v -> temp->vertice, no transposto fica temp->vertice -> v
            adicionarAresta(transposto, temp->vertice, v);
            temp = temp->proximo;
        }
    }
    return transposto;
}

// --- PASSO 1: DFS AUXILIAR PARA PREENCHER A PILHA ---
void preencherOrdem(Grafo* G, int v, int* visitado, Pilha* p) {
    visitado[v] = VERDADEIRO;
    
    No* temp = G->listasAdj[v];
    while (temp != NULL) {
        if (!visitado[temp->vertice]) {
            preencherOrdem(G, temp->vertice, visitado, p);
        }
        temp = temp->proximo;
    }
    // Quando o vértice termina de ser processado, vai para a pilha
    push(p, v);
}

// --- PASSO 3: DFS AUXILIAR PARA IMPRIMIR A COMPONENTE ---
void DFS_Transposta(Grafo* G_T, int v, int* visitado) {
    visitado[v] = VERDADEIRO;
    printf("%d ", v); // Imprime o vértice que faz parte da componente atual
    
    No* temp = G_T->listasAdj[v];
    while (temp != NULL) {
        if (!visitado[temp->vertice]) {
            DFS_Transposta(G_T, temp->vertice, visitado);
        }
        temp = temp->proximo;
    }
}

// --- FUNÇÃO PRINCIPAL DE KOSARAJU ---
void encontrarSCCs(Grafo* G) {
    Pilha* p = criarPilha(G->numVertices);
    int* visitado = (int*)calloc(G->numVertices, sizeof(int));

    // 1. Preenche a pilha com a ordem de finalização dos vértices
    for (int i = 0; i < G->numVertices; i++) {
        if (!visitado[i]) {
            preencherOrdem(G, i, visitado, p);
        }
    }

    // 2. Cria o grafo invertido (transposto)
    Grafo* G_T = obterGrafoTransposto(G);

    // Reinicializa o vetor de visitados para reutilizá-lo na transposta
    for (int i = 0; i < G->numVertices; i++) {
        visitado[i] = FALSO;
    }

    // 3. Processa todos os vértices desempilhando-os na ordem correta
    int contadorComponentes = 0;
    printf("Componentes Fortemente Conexas encontradas:\n");
    
    while (!pilhaVazia(p)) {
        int v = pop(p);

        // Se o vértice tirado do topo ainda não foi processado na transposta,
        // ele representa o início de uma nova Componente Fortemente Conexa
        if (!visitado[v]) {
            contadorComponentes++;
            printf("Componente %d: { ", contadorComponentes);
            DFS_Transposta(G_T, v, visitado);
            printf("}\n");
        }
    }

    // Limpeza da memória local da função
    free(visitado);
    free(p->itens); free(p);
    
    // Libera o grafo transposto
    for (int i = 0; i < G_T->numVertices; i++) {
        No* temp = G_T->listasAdj[i];
        while (temp != NULL) {
            No* apagar = temp;
            temp = temp->proximo;
            free(apagar);
        }
    }
    free(G_T->listasAdj); free(G_T);
}

// --- TESTE DO ALGORITMO ---
int main() {
    int vertices = 5;
    Grafo* g = criarGrafo(vertices);

    // Montando o grafo de exemplo:
    // Componente A: 0 ⇄ 1 ➜ 2 (2 faz parte de um ciclo isolado 2 ⇄ 3)
    // 4 é um vértice avulso que recebe conexão de 3.
    adicionarAresta(g, 0, 1);
    adicionarAresta(g, 1, 0);
    adicionarAresta(g, 1, 2);
    adicionarAresta(g, 2, 3);
    adicionarAresta(g, 3, 2);
    adicionarAresta(g, 3, 4);

    printf("--- Executando Algoritmo de Kosaraju ---\n");
    encontrarSCCs(g);

    // Liberando a memória do grafo original
    for (int i = 0; i < vertices; i++) {
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