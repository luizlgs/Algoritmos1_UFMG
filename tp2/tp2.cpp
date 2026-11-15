#include <climits>
#include <cstdio>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <math.h>
#include <algorithm>
#include <queue>

//representa cada sala no grafo da universidade
typedef struct Node {
    long long id;
    long long tempo_chegada_amigo = LLONG_MAX;
    long long dist_artur = LLONG_MAX;
    bool visitado = false;
    std::vector<std::pair<long long, Node*>> adj;
} Node;

//reseta visitado e dist_artur para rodar dijkstra
void reset_grafo(std::vector<Node*>& grafo, long long tam_grafo){
    for(long long i=1; i<=tam_grafo; i++){
        grafo[i]->visitado = false;
        grafo[i]->dist_artur = LLONG_MAX;
    }
}
//no_inicial é o nó que representa a sala de artur.
//esse é o dijkstra usado pra encontrar o caminho minimo de artur ate a saida (caso antecedencia = -1) ou
//testar antecedencias no intervalo de 0 ate antecedencia trivial da distancia minima ate a saida
bool dijkstra_artur_dist(Node* no_inicial, long long antecedencia, long long salas){
    //o reset de visitado e dist_artur é feito antes da chamada da função
    no_inicial->dist_artur = 0;
    std::priority_queue<std::pair<long long, Node*>, std::vector<std::pair<long long, Node*>>, std::greater<std::pair<long long, Node*>>> q;
    q.push({0, no_inicial});
    while(!q.empty()){
        auto aux = q.top();
        if(aux.second->id == salas) return true;
        q.pop();
        if(aux.second->visitado) continue;
        aux.second->visitado = true;
        for(auto& i:aux.second->adj){
            if(i.second->visitado) continue;
            if(antecedencia == -1){ //caso esteja apenas calculando a distancia de artur ate a saida
                if(i.second->dist_artur > aux.second->dist_artur + i.first){
                    i.second->dist_artur = aux.second->dist_artur + i.first;
                    q.push({i.second->dist_artur, i.second});
                }   
            }
            else{
                long long nova_dist = aux.second->dist_artur + i.first;
                if(nova_dist - antecedencia <= i.second->tempo_chegada_amigo && i.second->dist_artur > nova_dist){
                    i.second->dist_artur = nova_dist;
                    q.push({nova_dist, i.second});
                }
            }
        }
    }
    return false;
}   

//salva os tempos de bloqueio (momento o qual a sala ficará indisponivel) em tempo_chegada_amigo de cada vertice.
//aqui o algoritmo adiciona inicialmente todos os amigos na fila do dijkstra para calcular as distancias minimas de uma
//só vez
void dijkstra_amigos(std::vector<Node*>& amigos){
    std::priority_queue<std::pair<long long, Node*>, std::vector<std::pair<long long, Node*>>, std::greater<std::pair<long long, Node*>>> q;
    for(auto i : amigos){ //inicializando a fila de amigos
        q.push({0, i});
        i->tempo_chegada_amigo = 0;
    }
    while(!q.empty()){
        auto aux = q.top();
        q.pop();
        if(aux.second->visitado) continue;
        aux.second->visitado = true;
        for(auto& i : aux.second->adj){
            if(i.second->visitado) continue;
            long long nova_dist = aux.second->tempo_chegada_amigo + 2 * i.first;
            if(i.second->tempo_chegada_amigo > nova_dist){
                i.second->tempo_chegada_amigo = nova_dist;
                q.push({nova_dist, i.second});
            }
        }
    }
}

//testa se a antecedencia funciona no intervado de 0 ate a antecedencia trivial (distancia minima do artur ate o
//vertice de saida)
long long testa_antecedencia(std::vector<Node*>& grafo, long long antecedencia, long long num_nos){
    long long inicio = 0, fim = antecedencia;
    long long antecedencia_final = antecedencia;
    while(inicio <= fim){
        long long meio = inicio + (fim - inicio) / 2;
        reset_grafo(grafo, num_nos);
        if(dijkstra_artur_dist(grafo[1], meio, num_nos)){
            antecedencia_final = meio;
            fim = meio - 1;
        } else {
            inicio = meio + 1;
        }
    }
    return antecedencia_final;
}

int main(){
    //n = salas
    //m = corredores
    //k=amigos
    long long n, m, k;
    scanf("%lld %lld %lld", &n, &m, &k);
    std::vector<Node*> grafo(n+1); //criação do grafo, mantendo a posição 0 nula para começar o id 1
    for(long long i=1; i<=n; i++){ //inicialização dos nós com os respectivos ids
        grafo[i] = new Node();
        grafo[i]->id = i;
    }

    //preenchimento das adjacencias
    for(long long i=0; i<m; i++){
        long long n1, n2, peso;
        scanf("%lld %lld %lld", &n1, &n2, &peso);
        std::pair<long long, Node*> no1 {peso, grafo[n1]};
        std::pair<long long, Node*> no2 {peso, grafo[n2]};

        grafo[n1]->adj.push_back(no2);
        grafo[n2]->adj.push_back(no1);
    }
    
    //salvando a posicao inicial de cada amigo no grafo
    std::vector<Node*> amigos;
    for(long long i=0; i<k; i++){
        long long sala_amigo;
        scanf("%lld", &sala_amigo);
        amigos.push_back(grafo[sala_amigo]);
    }

    //dijkstra para achar o caminho minimo da sala de artur ate a saida (o valor trivial de antecedencia)
    dijkstra_artur_dist(grafo[1], -1, n);
    long long min_dist = grafo[n]->dist_artur;

    //reset para o fazer o dijkstra que calacula o tempo de bloqueio das salas
    reset_grafo(grafo, n);
    dijkstra_amigos(amigos);

    //calculo da antecedencia por testes no intervalo encontrado
    long long antecedencia_final = testa_antecedencia(grafo, min_dist, n);
    printf("%lld\n", antecedencia_final);
    
    return 0;
    
}