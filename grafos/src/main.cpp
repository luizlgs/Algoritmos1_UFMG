#include "../include/core.hpp"
#include "../include/Node.hpp"

//dfs algorithm

void new_edge(Node *u, Node *v) {
    u->new_adj(v);
}

//dfs algorithm
void dfs(Node* node, std::vector<int*>& topological_sort){
    node->set_discovered();

    for(int i=0; i<node->get_adj().size(); i++){
        if(!node->get_adj()[i]->get_discovered())
            dfs(node->get_adj()[i], topological_sort);
    }
    node->set_ended();
    int *id = new int(node->get_id());
    topological_sort.insert(topological_sort.begin(), id);
    //std::cout << node->get_id() << std::endl;
}

void sqared_graph(std::vector<Node*> graph){ //imprime duplicatas pois podem ter mais de um caminho de tamanho 2 entre dois vertices
    for(int i=1; i<graph.size(); i++){
        std::vector<Node*> current_adj1 = graph[i]->get_adj();
        for(int j=0; j<current_adj1.size(); j++){
            std::vector<Node*> current_adj2 = current_adj1[j]->get_adj();
            for(int k=0; k<current_adj2.size(); k++){
                std::cout << "(" << graph[i]->get_id() << ", " << current_adj2[k]->get_id() << ")\n";
            }
        }
    }
}


int main(){
    int n = 9;
    std::vector<Node*> graph(n+1); //indice 0 nao será um vertice
    std::vector<int*> topological_sort;

    for (int i = 1; i <= n; i++){
        graph[i] = new Node(i);
    }


    new_edge(graph[1], graph[2]);
    new_edge(graph[2], graph[3]);
    new_edge(graph[3], graph[5]);
    new_edge(graph[5], graph[8]);
    new_edge(graph[8], graph[9]);
    new_edge(graph[2], graph[4]);
    new_edge(graph[4], graph[5]);
    new_edge(graph[2], graph[5]);
    new_edge(graph[1], graph[6]);
    new_edge(graph[6], graph[7]);
    new_edge(graph[7], graph[8]);
    new_edge(graph[1], graph[9]);

    /*for(auto i=1; i<graph.size(); i++){
        for(int j=0; j<graph[i]->get_adj().size(); j++){
            std::cout << graph[i]->get_adj()[j]->get_id() << " ";
        }
        std::cout << std::endl;
    }*/

    dfs(graph[1], topological_sort);
    //sqared_graph(graph);

    for(auto i:topological_sort){
        std::cout << *i << std::endl;
    }

    return 0;
}