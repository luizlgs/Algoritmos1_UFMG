#include <climits>
#include <stdio.h>
#include <vector>
#include <algorithm>

//Inicializa a matriz que representa o número de saltos a partir de cada pedra baseado no número de saltos longos restantes
void inicializa_saltos(long long* saltos, long long tam) {
    for(long long i=0; i<tam; i++) {
        saltos[i] = -1;
    }
}

//Preenche a matriz k+1 x n+2 onde a i-esima linha e a j-esima coluna representa o número mínimo de saltos necessários para 
//chegar na margem a partir da j-esima pedra com i saltos longos restantes
long long calc_saltos(long long** saltos, long long posicao, long long *rio, long long final, long long saltos_curtos, long long saltos_longos, long long saltos_longos_totais, long long saltos_longos_restantes) {
    //Caso em que ja foi usado todos os saltos longos restantes
    if(saltos_longos_restantes < 0) {
        return LLONG_MAX;
    }
    //Caso em que ja chegamos na margem oposta, o número de saltos restantes é 0
    else if(posicao == final) {
        saltos[saltos_longos_restantes][posicao] = 0;
        return 0;
    }
    //Caso para pedra a qual o mínimos de saltos ainda não foi calculado
    else if(saltos[saltos_longos_restantes][posicao] == -1) {
        long long pedra_c; //Índice da pedra mais longe que é possível de se chegar com um salto curto
        std::vector<long> pedras_l; //Índices das pedras que são possíveis de chegar depois de pedra_c com salto longo

        //Cálculo da pedra_c
        long i=posicao+1;
        for(; i<= final && rio[i] <= rio[posicao]+saltos_curtos; i++);
        pedra_c = i-1;

        //Cálculo das pedras_l
        for(long j = i; j<= final && rio[j] <= rio[posicao]+saltos_longos && j <= final; j++) {
            pedras_l.push_back(j);
        }

        //Número mínimo de passos necessários partindo da pedra de índice pedra_c
        std::vector<long long> passos;
        if(pedra_c != posicao){
            long long v1 = calc_saltos(saltos, pedra_c, rio, final, saltos_curtos, saltos_longos, saltos_longos_totais, saltos_longos_totais);
            passos.push_back(v1);
        }

        //Números mínimo de passos necessários partindo de cada pedra dos índices armazenados em pedras_l
        if(!pedras_l.empty()){
            for(auto i:pedras_l) {
                long long v = calc_saltos(saltos, i, rio, final, saltos_curtos, saltos_longos, saltos_longos_totais, saltos_longos_restantes-1);
                passos.push_back(v);
            }
        }

        //Caso em que é possível chegar em alguma pedra a partir da pedra atual
        if(!passos.empty()){
            auto menor = std::min_element(passos.begin(), passos.end());
    
            if(*menor != LLONG_MAX) {
                saltos[saltos_longos_restantes][posicao] = 1 + *menor;
            }
            else {
                saltos[saltos_longos_restantes][posicao] = LLONG_MAX;
            }
        }
        //Se não for possivel chegar em nenhuma pedra a partir daqui, é salvo o valor máximo para representar a 
        //impossibilidade de chegar a margem oposta a partir do contexto da chamada atual
        else {
            saltos[saltos_longos_restantes][posicao] = LLONG_MAX;
        }
    }
    //É retornado o número minimo de passos necessários para chegar na margem oposta no contexto da chamada atual
    return saltos[saltos_longos_restantes][posicao];
}

int main() {
    long long n, m, k;
    scanf("%lld %lld %lld", &n, &m, &k);
    long long rio[n+2];
    rio[0] = 1; //A primeira pedra representa a margem de início com valor 1
    rio[n+1] = m; //A margem contrária vai ser representada como uma última pedra

    //Preenche o vetor que vai representar as pedras do rio
    for(long long i=1; i<n+1; i++) {
        scanf("%lld", &rio[i]);
    }

    //Recebimento dos valores de pulo curto e longo
    long long x, y;
    scanf("%lld %lld", &x, &y);

    //Inicialização da matriz de saltos
    long long** saltos = new long long*[k+1];
    for(int i=0; i<k+1; i++) {
        saltos[i] = new long long[n+2];
        inicializa_saltos(saltos[i], n+2);
    }

    calc_saltos(saltos, 0, rio, n+1, x, y, k, k);

    //Caso em que não é possível chegar ao outro lado da margem
    if(saltos[k][0] == LLONG_MAX){
        saltos[k][0] = -1;
    }
    
    printf("%lld\n", saltos[k][0]);

    //Desalocando os valores alocados para a matriz de saltos
    for(int i=0; i<k+1; i++) {
        free(saltos[i]);
    }
    free(saltos);
    
    return 0;
}