#include <stdio.h>
#include <vector>
#include <math.h>
#include <algorithm>
#include <stdexcept>

int max(int a, int b){
    return a>b ? a : b;
}

int min(int a, int b){
    return a<b ? a : b;
}

typedef struct Node {
    int id, status; //status = 0(nao visitado), 1(visitado)
    Node *pai;
    std::vector<Node*> adj;
} Node;

void limpar_grafo(int tam, struct Node* grafo[], int c){
    for(int i=1; i<tam; i++){
        grafo[i]->status = 0;
        if(c == 1)
            grafo[i]->pai = nullptr;
    }
}

void dfs(Node *no, int nivel_atual, int *max_dist, Node*& no_mais_longe, int dfs_number, Node*& no_centro, int diametro){
    int n = nivel_atual+1;
    no->status = 1;

    if(dfs_number != 1){ 
        if(dfs_number == 2 && n == diametro/2){
            no_centro = no;
            throw std::runtime_error("Busca concluida");
        }
        else if(dfs_number == 3 && n ==diametro/2){
            no_centro->id = -10;
            no_centro->pai = nullptr;
            no_centro->adj.push_back(no);
            no_centro->adj.push_back(no->pai);

            no->adj.erase(std::remove(no->adj.begin(), no->adj.end(), no->pai), no->adj.end());
            no->pai->adj.erase(std::remove(no->pai->adj.begin(), no->pai->adj.end(), no), no->pai->adj.end());
            
            throw std::runtime_error("Busca concluida");
        }
        dfs(no->pai, n, max_dist, no_mais_longe, dfs_number, no_centro, diametro);
    }
    else{
        if(dfs_number == 1 && n > *max_dist){
            no_mais_longe = no;
            *max_dist = n;
        }
        
        if(no->adj.size() == 0){
            return;
        }
        
        for(int i=0; i<no->adj.size(); i++){
            if(no->adj[i]->status == 0){
                no->adj[i]->pai = no;
                dfs(no->adj[i], n, max_dist, no_mais_longe, dfs_number, no_centro, diametro);
            }
            
        }
    }
}

void dfs_number2(Node *no, int nivel_atual, std::vector<int>*& max_ramos, int num_ramo, int diametro){ // essa dfs recebe como raiz o no central
    no->status = 1;
    int n = nivel_atual + 1, num_filhos=0;
    
    for(int i=0; i<no->adj.size(); i++){
        if(no->adj[i]->pai == no) num_filhos++;
        
        if(no->adj[i]->status == 0){
            dfs_number2(no->adj[i], n, max_ramos, num_ramo, diametro);
        }
    }
    if(num_filhos==0 && n == diametro/2){
        (*max_ramos)[num_ramo] += 1;
        return;
    }
}

//void dfs_numbe3(Node *no, int nivel_atual, std::vector<int> tam_ramos){}

int soma_produto_pares(std::vector<int>* nums){
    int soma, soma_quadrados;
    soma = soma_quadrados = 0;
    for(int i:*nums){
        soma += i;
        soma_quadrados += i*i;
    }

    //formula soma dos produtos para a par de um conjunto
    return ((soma*soma)-soma_quadrados)/2;
}

int main() {
    int nodes;
    scanf("%d", &nodes);
    struct Node* grafo[nodes+1]; //indice 0 vazio e os demais tem o mesmo indice que o seu id
    
    //inicializacao dos das arestas em uma matriz nodes-1 x 2
    for(int i=1; i<nodes+1; i++){
        grafo[i] = new Node();
        grafo[i]->id = i;
        grafo[i]->pai = nullptr;
    }
    int u, v;
    for(int i=1; i<nodes; i++){
        scanf("%d %d", &u, &v);
        grafo[u]->id = u;
        grafo[u]->status = 0;
        grafo[u]->adj.push_back(grafo[v]);

        grafo[v]->id = v;
        grafo[v]->status = 0;
        grafo[v]->adj.push_back(grafo[u]);
    }

    Node *no_centro = new Node();
    
    Node *no_mais_longe1 = new Node();
    int *max_dist1 = new int(0);
    dfs(grafo[1], -1, max_dist1, no_mais_longe1, 1, no_centro, 0); //os ultimos tres parametro nao sao usados

    limpar_grafo(nodes+1, grafo, 1);
    
    Node *no_mais_longe2 = new Node();
    int *max_dist2 = new int(0);
    dfs(no_mais_longe1, -1, max_dist2, no_mais_longe2, 1, no_centro, 0); //diametro vai para max_dist_2
    
    limpar_grafo(nodes+1, grafo, 0);

    printf("%d\n", *max_dist2+1);

    if(*max_dist2%2 == 0){
        try{
            dfs(no_mais_longe2, -1, max_dist2, no_mais_longe1, 2, no_centro, *max_dist2); //no centro vai para no_centro
        } catch(const std::runtime_error& e){
            limpar_grafo(nodes+1, grafo, 0);
            std::vector<int>* max_ramos = new std::vector<int>;
            int *maior=new int(0);

            no_centro->status = 1;
            for(int i=0; i<no_centro->adj.size(); i++){
                max_ramos->push_back(0);
                dfs_number2(no_centro->adj[i], 0, max_ramos, i, *max_dist2);
            }

            int caminhos = soma_produto_pares(max_ramos);
            printf("%d\n", caminhos);
            
        }
    }
    else{
        try{
            dfs(no_mais_longe2, -1, max_dist2, no_mais_longe1, 3, no_centro, *max_dist2); //é criado um nó centro novo com 
        } catch (const std::runtime_error& e){
            limpar_grafo(nodes+1, grafo, 0);

            /*printf("\n");
            for(auto i:no_centro->adj[0]->adj){
                printf("vizinho de %d: %d\n", no_centro->adj[0]->id, i->id);
            }
            printf("\n");
            for(auto i:no_centro->adj[1]->adj){
                printf("vizinho de %d: %d\n", no_centro->adj[1]->id, i->id);
            }
            printf("\n");*/
            
            std::vector<int>* max_ramos = new std::vector<int>;
            max_ramos->push_back(0);
            max_ramos->push_back(0);
            dfs_number2(no_centro->adj[0], -1, max_ramos, 0, (*max_dist2)-1);
            dfs_number2(no_centro->adj[1], -1, max_ramos, 1, (*max_dist2)-1);

            int caminhos = (*max_ramos)[0] * (*max_ramos)[1];
            printf("%d\n", caminhos);
            
        }
    }
    
}