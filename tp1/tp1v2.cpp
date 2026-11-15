#include <stdio.h>
#include <vector>
#include <utility>

int max(int a, int b){
    return a>b ? a : b;
}

int min(int a, int b){
    return a<b ? a : b;
}

typedef struct Node {
    int id, status; //status = 0(nao visitado), 1(visitado)
    Node *pai;
    std::vector<int> adj;
} Node;

void limpar_grafo(int tam, struct Node* grafo[], int c){
    for(int i=1; i<tam; i++){
        grafo[i]->status = 0;
        if(c == 1)
            grafo[i]->pai = nullptr;
    }
}

//dfs normal pra encontrar o vertice mais longe da primeira raiz e o diametro
void dfs(Node *no, int nivel_atual, int *max_dist, Node*& no_mais_longe, int dfs_number, Node*& no_centro, int diametro, std::vector<Node*>& grafo){
    if(dfs_number != 1){
        int n = nivel_atual+1;
        while(true){
            no->status = 1;
            if((dfs_number == 2 || dfs_number == 3) && n == diametro/2){
                no_centro = no;
                return;
            }
            no = no->pai;
            n++;
        }
    }
    else{
        std::vector<std::pair<Node*,int>> pilha;
        no->status = 1;
        pilha.push_back({no, nivel_atual+1});

        while(!pilha.empty()){
            Node *atual = pilha.back().first;
            int n = pilha.back().second;
            pilha.pop_back();

            if(n > *max_dist){
                no_mais_longe = atual;
                *max_dist = n;
            }

            for(int i=0; i<(int)atual->adj.size(); i++){
                Node *viz = grafo[atual->adj[i]];
                if(viz->status == 0){
                    viz->pai = atual;
                    viz->status = 1;
                    pilha.push_back({viz, n+1});
                }
            }
        }
    }
}

//dfs que vai encontrar a quantidade de ciclos maximos a partir do nó central da arvore
void dfs_central(Node *no, int nivel_atual, std::vector<int>*& max_ramos, int num_ramo, int diametro, std::vector<Node*>& grafo){ // essa dfs recebe como raiz o no central
    std::vector<std::pair<Node*,int>> pilha;
    no->status = 1;
    pilha.push_back({no, nivel_atual+1});

    while(!pilha.empty()){
        Node *atual = pilha.back().first;
        int n = pilha.back().second;
        pilha.pop_back();

        if(n == diametro/2){
            (*max_ramos)[num_ramo] += 1;
            continue;
        }

        for(int i=0; i<(int)atual->adj.size(); i++){
            Node *viz = grafo[atual->adj[i]];
            if(viz->status == 0){
                viz->status = 1;
                pilha.push_back({viz, n+1});
            }
        }
    }
}

long long soma_produto_pares(std::vector<int>* nums){
    long long soma, soma_quadrados;
    soma = soma_quadrados = 0;
    for(int i:*nums){
        soma += i;
        soma_quadrados += (long long)i*i;
    }

    //formula soma dos produtos para a par de um conjunto
    return ((soma*soma)-soma_quadrados)/2;
}

int main() {
    int nodes;
    scanf("%d", &nodes);
    std::vector<Node*> grafo(nodes+1); //referencia os nóes presentes em grafo_real
    std::vector<Node> grafo_real(nodes+1); //indice 0 vazio e os demais tem o mesmo indice que o seu id
    
    //inicializacao das arestas em uma matriz Nodes 1 x 2
    for(int i=1; i<nodes+1; i++){
        grafo[i] = &grafo_real[i];
        grafo[i]->id = i;
        grafo[i]->pai = nullptr;
    }
    
    int u, v;
    for(int i=1; i<nodes; i++){
        scanf("%d %d", &u, &v);
        grafo[u]->adj.push_back(v);
        grafo[v]->adj.push_back(u);
    }

    Node *no_centro = new Node();

    Node *no_mais_longe1 = new Node();
    int *max_dist1 = new int(0);

    //os ultimos tres parametro nao sao usados
    dfs(grafo[1], -1, max_dist1, no_mais_longe1, 1, no_centro, 0, grafo); //acha o no mais longe da raiz a partir da raiz da primeira dfs

    limpar_grafo(nodes+1, grafo.data(), 1);

    Node *no_mais_longe2 = new Node();
    int *max_dist2 = new int(0);
    
    //dfs do nó mais longe da raiz anterior para encontrar o diametro do grafo
    dfs(no_mais_longe1, -1, max_dist2, no_mais_longe2, 1, no_centro, 0, grafo); //diametro vai para max_dist_2

    limpar_grafo(nodes+1, grafo.data(), 0);

    printf("%d\n", *max_dist2+1);

    if(*max_dist2%2 == 0){
        dfs(no_mais_longe2, -1, max_dist2, no_mais_longe1, 2, no_centro, *max_dist2, grafo); //no centro vai para no_centro
        limpar_grafo(nodes+1, grafo.data(), 0);

        //max_ramos tera a quantidade de ramos de tamanho diametro/2 para cada ramificacao da raiz
        std::vector<int>* max_ramos = new std::vector<int>;

        no_centro->status = 1;
        for(int i=0; i<(int)no_centro->adj.size(); i++){
            max_ramos->push_back(0); //valor inicial para o ramo i
            dfs_central(grafo[no_centro->adj[i]], 0, max_ramos, i, *max_dist2, grafo);
        }

        long long caminhos = soma_produto_pares(max_ramos);
        printf("%lld\n", caminhos);
    }
    else{
        dfs(no_mais_longe2, -1, max_dist2, no_mais_longe1, 3, no_centro, *max_dist2, grafo); //os dois nos da aresta central sao no_centro e no_centro->pai
        limpar_grafo(nodes+1, grafo.data(), 0);

        Node *centro_b = no_centro->pai;
        no_centro->status = 1;
        centro_b->status = 1;

        //max ramos tera a primeira posicao com o numero de ramos de tamanho floor(diametro/2) do lado esquero e a segunda posicao com o mesmo numero para o lado direito
        std::vector<int>* max_ramos = new std::vector<int>;
        max_ramos->push_back(0);
        max_ramos->push_back(0);
        dfs_central(no_centro, -1, max_ramos, 0, (*max_dist2)-1, grafo);
        dfs_central(centro_b, -1, max_ramos, 1, (*max_dist2)-1, grafo);

        long long caminhos = (long long)(*max_ramos)[0] * (*max_ramos)[1];
        printf("%lld\n", caminhos);

    }

}
