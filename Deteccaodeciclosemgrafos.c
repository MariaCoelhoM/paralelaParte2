#include <stdio.h>
#include <stdlib.h>

// Definição dos estados (cores) dos vértices
#define BRANCO 0 // Não visitado
#define CINZA  1 // Na pilha de recursão atual (sendo visitado)
#define PRETO  2 // Totalmente processado (ele e seus descendentes)

// Estrutura do nó da lista de adjacência
typedef struct No {
    int vertice;
    struct No* proximo;
} No;

// Estrutura do Grafo
typedef struct Grafo {
    int numVertices;
    No** listasAdj; // Vetor de listas de adjacência
    int* estado;    // Vetor para armazenar a cor de cada vértice
} Grafo;

// Função para criar um novo nó
No* criarNo(int v) {
    No* novoNo = (No*)malloc(sizeof(No));
    novoNo->vertice = v;
    novoNo->proximo = NULL;
    return novoNo;
}

// Função para inicializar um grafo com N vértices
Grafo* criarGrafo(int vertices) {
    Grafo* grafo = (Grafo*)malloc(sizeof(Grafo));
    grafo->numVertices = vertices;

    grafo->listasAdj = (No**)malloc(vertices * sizeof(No*));
    grafo->estado = (int*)malloc(vertices * sizeof(int));

    for (int i = 0; i < vertices; i++) {
        grafo->listasAdj[i] = NULL;
        grafo->estado[i] = BRANCO; // Todos começam como BRANCO
    }
    return grafo;
}

// Função para adicionar uma aresta direcionada (u ➜ v)
void adicionarAresta(Grafo* grafo, int u, int v) {
    No* novoNo = criarNo(v);
    novoNo->proximo = grafo->listasAdj[u];
    grafo->listasAdj[u] = novoNo;
}

// Função auxiliar recursiva da DFS que faz a checagem das cores
int temCicloAux(Grafo* grafo, int u) {
    // Entrou no vértice: muda para CINZA
    grafo->estado[u] = CINZA;

    // Percorre todos os vizinhos de u
    No* temp = grafo->listasAdj[u];
    while (temp != NULL) {
        int v = temp->vertice;

        // Se o vizinho está CINZA, fechamos um ciclo!
        if (grafo->estado[v] == CINZA) {
            return 1; 
        }

        // Se o vizinho é BRANCO, continua o mergulho recursivo
        if (grafo->estado[v] == BRANCO) {
            if (temCicloAux(grafo, v) == 1) {
                return 1;
            }
        }
        temp = temp->proximo;
    }

    // Terminou de explorar todos os caminhos a partir de u: muda para PRETO
    grafo->estado[u] = PRETO;
    return 0;
}

// Função principal que gerencia a verificação em todo o grafo
int possuiCiclo(Grafo* grafo) {
    // O loop garante que mesmo componentes isolados (grafos desconexos) sejam testados
    for (int i = 0; i < grafo->numVertices; i++) {
        if (grafo->estado[i] == BRANCO) {
            if (temCicloAux(grafo, i) == 1) {
                return 1; // Ciclo detectado
            }
        }
    }
    return 0; // Grafo acíclico
}

// --- FUNÇÃO MAIN 
int main() {
    int vertices = 4;
    Grafo* g = criarGrafo(vertices);

    // Vamos criar o seguinte grafo direcionado:
    // 0 ➜ 1 ➜ 2 ➜ 3
    adicionarAresta(g, 0, 1);
    adicionarAresta(g, 1, 2);
    adicionarAresta(g, 2, 3);
    
    // ATENÇÃO: Descomente a linha abaixo para fechar um ciclo (3 ➜ 1) e testar a detecção
    // adicionarAresta(g, 3, 1); 

    printf("--- Executando Deteccao de Ciclos ---\n");
    
    if (possuiCiclo(g)) {
        printf("[ALERTA] O grafo contem pelo menos um ciclo!\n");
    } else {
        printf("[SUCESSO] O grafo eh aciclico (sem ciclos).\n");
    }

    // Liberação da memória alocada
    for (int i = 0; i < vertices; i++) {
        No* temp = g->listasAdj[i];
        while (temp != NULL) {
            No* inverter = temp;
            temp = temp->proximo;
            free(inverter);
        }
    }
    free(g->listasAdj);
    free(g->estado);
    free(g);

    return 0;
}