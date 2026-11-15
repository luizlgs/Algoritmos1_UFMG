/*
void dijkstra_artur(Node* no_inicial){ //no_inicial é o nó que representa a sala de artur
     std::priority_queue<std::pair<long long, Node*>, std::vector<std::pair<long long, Node*>>, std::greater<std::pair<long long, Node*>>> q;
     no_inicial->tempo_chegada_artur = 0;
     q.push({0, no_inicial});
     while(!q.empty()){
         auto aux = q.top();
         q.pop();
         if(aux.second->visitado) continue;
         aux.second->visitado = true;
         for(auto& i:aux.second->adj){
             if(i.second->visitado) continue;
             long long dist_q_front = aux.second->tempo_chegada_artur;
             if(i.second->tempo_chegada_artur > dist_q_front + i.first){
                 i.second->tempo_chegada_artur = dist_q_front + i.first;
                 q.push({i.second->tempo_chegada_artur, i.second});
             }
         }
     }
 }
 */