# Análise de Complexidade: Busca em Profundidade (DFS) com Matriz de Adjacência

A complexidade de tempo da Busca em Profundidade (DFS) utilizando uma Matriz de Adjacência é $O(V^2)$, onde $V$ representa o número de vértices (nós) do grafo.

Abaixo, explico detalhadamente como esse cálculo é feito e por que ele chega a esse resultado.

---

## Como é calculada a complexidade nesse caso?

Para entender o cálculo, precisamos analisar o comportamento do algoritmo DFS linha por linha quando ele interage com a estrutura da matriz. O cálculo é baseado em duas regras fundamentais:

### 1. Cada vértice é visitado exatamente uma vez
Graças ao vetor de controle `visitado`, o algoritmo garante que a função principal de busca (`DFS-visita`) seja executada exatamente uma vez para cada um dos $V$ vértices do grafo. Se um vértice já foi marcado, ele é ignorado.

### 2. A busca pelos vizinhos varre a linha inteira da matriz
Sempre que o algoritmo está posicionado em um vértice $u$ e precisa descobrir quem são os seus vizinhos (para executar o trecho *para cada vértice w em adj(u)*), ele é obrigado a olhar a linha correspondente a $u$ na matriz de adjacência.

Como a matriz é de tamanho $V \times V$, cada linha possui exatamente $V$ colunas. Portanto, para encontrar os vizinhos de um único vértice, o algoritmo precisa checar todas as $V$ posições daquela linha, independentemente de haver uma aresta ali (valor `1`) ou não (valor `0`).

---

## Conclusão

Se temos $V$ vértices, e para cada um deles precisamos realizar uma varredura de tamanho $V$ na matriz para encontrar suas conexões, o número total de operações básicas de checagem é dado pela multiplicação:

$$\text{Total de operações} = V \times V = V^2$$

Portanto:

* **Complexidade de Tempo:** $O(V^2)$
* **Complexidade de Espaço (Memória):** $O(V^2)$ para armazenar a própria matriz de adjacência, além de $O(V)$ para o vetor de visitados e a pilha de recursão.