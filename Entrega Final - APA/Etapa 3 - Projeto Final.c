#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#define TRUE 1
#define FALSE 0
#define MAX 100
#define MAX2 1000

typedef struct {
    int numero_cidades; // armazena a quantidade de cidades que serão visitadas;
    char dimensao[MAX]; // armazena a string "dimension" presente no arquivo .txt;
    char nome[MAX]; // armazena a string "name" presente no arquivo .txt;
    char codigo[MAX]; // armazena a string que indica o código da instância presente no arquivo .txt;
    char capacidade[MAX]; // armazena a string "capacity" presente no arquivo .txt;
    int carga_maxima; // armazena a capacidade total que cada veículo poderá transportar;
    char demanda[MAX]; // armazena a string "demand_section" presente no arquivo .txt;
    int demanda_por_cidade[MAX]; // armazena a demanda necessária para cada cidade;
    char nome_matriz[MAX]; // armazena a string "edge_weight_section" presente no arquivo .txt;
    int** elementos; // armazena a matriz bidimensional com todas as distancias entre as cidades;
} Matriz;

typedef struct {
    int* rotas;
    int* melhor_rota;
    int tamanho_rota;
} Rota;

Matriz m;
Rota r;
Rota* rota_arm;
int tamanho_total = 1;
int numero_rotas = 1;
int distancia_total = 0;
int custo_swap = 0;
int custo_twoopt = 0;
int custo_reinsertion = 0;

void ler_arquivo(char* arquivo) {

    FILE* fp = fopen(arquivo, "r");
    fscanf(fp,"%s %s\n", &m.nome, &m.codigo);
    fscanf(fp,"%s %d\n", &m.dimensao, &m.numero_cidades);
    fscanf(fp,"%s %d\n", &m.capacidade, &m.carga_maxima);
    fscanf(fp,"%s\n", &m.demanda);

    for (int a = 0; a < m.numero_cidades; a++){
        fscanf(fp, "%d %d", &a, &m.demanda_por_cidade[a]);
    }

    fscanf(fp,"%s\n", &m.nome_matriz);

    m.elementos = malloc(m.numero_cidades * sizeof(int*));

    for(int i = 0; i < m.numero_cidades; i++) {
        m.elementos[i] = malloc(m.numero_cidades * sizeof(int));
        for(int j = 0; j < m.numero_cidades; j++) {
            fscanf(fp, "%d ", &m.elementos[i][j]);
        }
    }

    fclose(fp);
}


void vizinho_proximo() {

    int inserido[m.numero_cidades];
    rota_arm = (Rota*)malloc(m.numero_cidades * sizeof(Rota));

    for(int i = 0; i < m.numero_cidades; i++){
        rota_arm[i].rotas = (int*)malloc(m.numero_cidades * sizeof(int));
        rota_arm[i].tamanho_rota = 0;
        inserido[i] = FALSE;
    }

    inserido[0] = TRUE;
    int i = 0;
    int a = 0;
    int x = 0;
    int aux = 0;
    int visitado = 0;
    int veiculos = 1;

    printf("\n_________________________________________________________\n\n");

    rota_arm[x].rotas[a] = 0;
    rota_arm[x].tamanho_rota = 1;

    while(visitado < m.numero_cidades - 1) {
        int referencia = MAX2;
        int vizinho = 0;

        for(int j = 0; j < m.numero_cidades; j++) {
            if(!inserido[j] && m.elementos[i][j] < referencia && m.elementos[i][j] > 0) {
                vizinho = j;
                referencia = m.elementos[i][j];
            }
        }

        aux += m.demanda_por_cidade[vizinho];

        if(aux <= m.carga_maxima){
            inserido[vizinho] = TRUE;
            rota_arm[x].rotas[a + 1] = vizinho;
            i = vizinho;
            visitado++;
            rota_arm[x].tamanho_rota++;
            a++;
            tamanho_total++;
            distancia_total = distancia_total + referencia;

            if(visitado == m.numero_cidades - 1){
                distancia_total = distancia_total + m.elementos[i][0];
            }
        }

        else if(aux > m.carga_maxima) {
            rota_arm[x].rotas[a + 1] = 0;
            rota_arm[x].tamanho_rota++;
            tamanho_total++;
            distancia_total = distancia_total + m.elementos[i][0];
            i = 0;
            aux = 0;
            a = 0;
            veiculos++;
            x++;
            rota_arm[x].rotas[a] = 0;
            rota_arm[x].tamanho_rota++;
            tamanho_total++;
            numero_rotas++;
        }

    }

    rota_arm[x].rotas[a + 1] = 0;
    rota_arm[x].tamanho_rota++;
    tamanho_total++;

    for(int w = 0; w < numero_rotas; w++){
        printf("Rota[%d]: ", w);
        for(int y = 0; y < rota_arm[w].tamanho_rota; y++){
            printf("%d ", rota_arm[w].rotas[y]);
        }
        printf("\n");
    }

    printf("\n_________________________________________________________\n");
    printf("\nNumero de veiculos: %d\n", veiculos);
    printf("\n_________________________________________________________\n");
    printf("\nDistancia total percorrida = %d\n", distancia_total);
    printf("\n_________________________________________________________\n");
}

void swap(){

    int distanciaTotal = distancia_total;
    int ref = distanciaTotal;
    int ex1=0, ex2=0, mex1=-1, mex2=-1;
    int melhor_rota = 0;

    for(int q = 0; q < numero_rotas; q++){
        if(rota_arm[q].tamanho_rota <= 4){
            continue;
        }

        for(ex1 = 0; ex1 < rota_arm[q].tamanho_rota; ex1++){
            if(rota_arm[q].rotas[ex1] == 0){
                continue;
            }

            for(ex2 = ex1 + 1; ex2 < rota_arm[q].tamanho_rota; ex2++){
                if(rota_arm[q].rotas[ex2] == 0){
                    continue;
                }
                    int refRota = 0;
                    if(ex2 == ex1+1){
                    refRota = distanciaTotal - m.elementos[rota_arm[q].rotas[ex1-1]][rota_arm[q].rotas[ex1]] - m.elementos[rota_arm[q].rotas[ex1]][rota_arm[q].rotas[ex1+1]]

                                             - m.elementos[rota_arm[q].rotas[ex2-1]][rota_arm[q].rotas[ex2]] - m.elementos[rota_arm[q].rotas[ex2]][rota_arm[q].rotas[ex2+1]]

                                             + m.elementos[rota_arm[q].rotas[ex1-1]][rota_arm[q].rotas[ex2]] + m.elementos[rota_arm[q].rotas[ex2]][rota_arm[q].rotas[ex1]]

                                             + m.elementos[rota_arm[q].rotas[ex2]][rota_arm[q].rotas[ex1]] + m.elementos[rota_arm[q].rotas[ex1]][rota_arm[q].rotas[ex2+1]];

                    } else {
                        refRota = distanciaTotal - m.elementos[rota_arm[q].rotas[ex1-1]][rota_arm[q].rotas[ex1]] - m.elementos[rota_arm[q].rotas[ex1]][rota_arm[q].rotas[ex1+1]]

                                                 - m.elementos[rota_arm[q].rotas[ex2-1]][rota_arm[q].rotas[ex2]] - m.elementos[rota_arm[q].rotas[ex2]][rota_arm[q].rotas[ex2+1]]

                                                 + m.elementos[rota_arm[q].rotas[ex1-1]][rota_arm[q].rotas[ex2]] + m.elementos[rota_arm[q].rotas[ex2]][rota_arm[q].rotas[ex1+1]]

                                                 + m.elementos[rota_arm[q].rotas[ex2-1]][rota_arm[q].rotas[ex1]] + m.elementos[rota_arm[q].rotas[ex1]][rota_arm[q].rotas[ex2+1]];
                    }

                if(refRota < ref){
                    ref = refRota;
                    mex1 = ex1;
                    mex2 = ex2;
                    melhor_rota = q;
                }
            }
        }
    }

    distanciaTotal = ref;

    printf("\n\nCusto final do Swap: %d\n", distanciaTotal);
    custo_swap = distanciaTotal;

    if(mex1 != -1){
        int aux = rota_arm[melhor_rota].rotas[mex2];
        rota_arm[melhor_rota].rotas[mex2] = rota_arm[melhor_rota].rotas[mex1];
        rota_arm[melhor_rota].rotas[mex1] = aux;
    }


    printf("\nMelhor rota do Swap: \n");
    for(int w = 0; w < numero_rotas; w++){
        printf("Rota[%d]: ", w);
        for(int y = 0; y < rota_arm[w].tamanho_rota; y++){
            printf("%d ", rota_arm[w].rotas[y]);
        }
        printf("\n");
    }

}

void twoopt(){

    int distanciaTotal = distancia_total;
    int ref = distanciaTotal;
    int ex1=0, ex2=0, mex1, mex2;
    int melhor_rota;

    for(int q = 0; q < numero_rotas; q++){
        if(rota_arm[q].tamanho_rota <= 5){
            continue;
        }

        for(ex1 = 0; ex1 < rota_arm[q].tamanho_rota; ex1++){
            if(rota_arm[q].rotas[ex1] == 0){
                continue;
            }

            for(ex2 = ex1 + 2; ex2 < rota_arm[q].tamanho_rota; ex2++){
                if(rota_arm[q].rotas[ex2] == 0){
                    continue;
                }
                    int refRota = 0;
                    refRota = distanciaTotal - m.elementos[rota_arm[q].rotas[ex1-1]][rota_arm[q].rotas[ex1]] - m.elementos[rota_arm[q].rotas[ex2]][rota_arm[q].rotas[ex2+1]]
                    +  m.elementos[rota_arm[q].rotas[ex1-1]][rota_arm[q].rotas[ex2]] + m.elementos[rota_arm[q].rotas[ex1]][rota_arm[q].rotas[ex2+1]];

                if(refRota < ref){
                    ref = refRota;
                    mex1 = ex1;
                    mex2 = ex2;
                    melhor_rota = q;
                }
            }
        }
    }

    distanciaTotal = ref;

    printf("\n\nCusto final do 2-opt: %d\n", distanciaTotal);
    custo_twoopt = distanciaTotal;

    int tam = (mex2 - mex1) + 1;
    int aux[tam];
    int j = mex2;

    for(int i = 0; i < tam; i++){
        aux[i] = rota_arm[melhor_rota].rotas[j];
        j--;

    }
    j = mex1;

    for(int i = 0; i < tam; i++){
        rota_arm[melhor_rota].rotas[j] = aux[i];
        j++;
    }

    printf("\nMelhor rota do 2-opt: \n");
    for(int w = 0; w < numero_rotas; w++){
        printf("Rota[%d]: ", w);
        for(int y = 0; y < rota_arm[w].tamanho_rota; y++){
            printf("%d ", rota_arm[w].rotas[y]);
        }
        printf("\n");
    }
}

void reinsertion(){

    int distanciaTotal = distancia_total;
    int ref = distanciaTotal;
    int ex1=0, ex2=0, mex1=-1, mex2=-1;
    int melhor_rota = 0;

    for(int q = 0; q < numero_rotas; q++){
        if(rota_arm[q].tamanho_rota <= 5){
            continue;
        }

        for(ex1 = 0; ex1 < rota_arm[q].tamanho_rota; ex1++){
            if(rota_arm[q].rotas[ex1] == 0){
                continue;
            }

            for(ex2 = ex1 + 2; ex2 < rota_arm[q].tamanho_rota; ex2++){
                if(rota_arm[q].rotas[ex2] == 0){
                    continue;
                }

                    int refRota1 = 0;
                    refRota1 = distanciaTotal - m.elementos[rota_arm[q].rotas[ex1-1]][rota_arm[q].rotas[ex1]] - m.elementos[rota_arm[q].rotas[ex1]][rota_arm[q].rotas[ex1+1]] - m.elementos[rota_arm[q].rotas[ex2]][rota_arm[q].rotas[ex2+1]]

                                             + m.elementos[rota_arm[q].rotas[ex1-1]][rota_arm[q].rotas[ex1+1]] + m.elementos[rota_arm[q].rotas[ex2]][rota_arm[q].rotas[ex1]] + m.elementos[rota_arm[q].rotas[ex1]][rota_arm[q].rotas[ex2+1]];


                if(refRota1 < ref){
                    ref = refRota1;
                    mex1 = ex1;
                    mex2 = ex2;
                    melhor_rota = q;
                }
            }
        }

        for(ex1 = rota_arm[q].tamanho_rota; ex1 > 0; ex1--){
            if(rota_arm[q].rotas[ex1] == 0){
                continue;
            }

            for(ex2 = ex1 - 2; ex2 > 0; ex2--){
                if(rota_arm[q].rotas[ex2] == 0){
                    continue;
                }

                    int refRota2 = 0;
                    refRota2 = distanciaTotal - m.elementos[rota_arm[q].rotas[ex1-1]][rota_arm[q].rotas[ex1]] - m.elementos[rota_arm[q].rotas[ex1]][rota_arm[q].rotas[ex1+1]] - m.elementos[rota_arm[q].rotas[ex2]][rota_arm[q].rotas[ex2+1]]

                                             + m.elementos[rota_arm[q].rotas[ex1-1]][rota_arm[q].rotas[ex1+1]] + m.elementos[rota_arm[q].rotas[ex2]][rota_arm[q].rotas[ex1]] + m.elementos[rota_arm[q].rotas[ex1]][rota_arm[q].rotas[ex2+1]];


                if(refRota2 < ref){
                    ref = refRota2;
                    mex1 = ex1;
                    mex2 = ex2;
                    melhor_rota = q;
                }
            }
        }
    }

    distanciaTotal = ref;

    printf("\n\nCusto final do Re-Insertion: %d\n", distanciaTotal);
    custo_reinsertion = distanciaTotal;

    if(mex1 != -1){
        int aux = rota_arm[melhor_rota].rotas[mex1];
        if(mex1<mex2){
            for(int i = mex1; i < mex2; i++){
                rota_arm[melhor_rota].rotas[i] = rota_arm[melhor_rota].rotas[i+1];
            }
            rota_arm[melhor_rota].rotas[mex2] = aux;
        }
        if(mex1>mex2){
            for(int i = mex1; i > mex2; i--){
                rota_arm[melhor_rota].rotas[i] = rota_arm[melhor_rota].rotas[i-1];
            }
            rota_arm[melhor_rota].rotas[mex2] = aux;
        }
    }

    printf("\nMelhor rota do Re-Insertion: \n");
    for(int w = 0; w < numero_rotas; w++){
        printf("Rota[%d]: ", w);
        for(int y = 0; y < rota_arm[w].tamanho_rota; y++){
            printf("%d ", rota_arm[w].rotas[y]);
        }
        printf("\n");
    }
}

void vnd(){

    int vizinhancas = 3, cont = 1;
    int custo = 0, custoref = distancia_total;

    for(int i = 0; i < m.numero_cidades; i++){
        rota_arm[i].melhor_rota = (int*)malloc(m.numero_cidades * sizeof(int));
    }

    for(int j = 0; j < numero_rotas; j++){
        for(int k = 0; k < rota_arm[j].tamanho_rota; k++){
            rota_arm[j].melhor_rota[k] = rota_arm[j].rotas[k];
        }
    }

    while(cont <= vizinhancas){

        if(cont == 1){
            custo = 0;
            swap();
            custo = custo_swap;
        }
        else if(cont == 2){
            custo = 0;
            twoopt();
            custo = custo_twoopt;
        }
        else {
            custo = 0;
            reinsertion();
            custo = custo_reinsertion;
        }

        if(custo < custoref){
            cont = 1;
            custoref = custo;

            for(int j = 0; j < numero_rotas; j++){
                for(int k = 0; k < rota_arm[j].tamanho_rota; k++){
                    rota_arm[j].melhor_rota[k] = rota_arm[j].rotas[k];
                }
            }
        } else {
            cont++;
        }

    }

    printf("\n_________________________________________________________\n");
    printf("\n\nCusto final do VND: %d\n", custoref);
    printf("\nMelhor rota do VND: \n");
    for(int w = 0; w < numero_rotas; w++){
        printf("Rota[%d]: ", w);
        for(int y = 0; y < rota_arm[w].tamanho_rota; y++){
            printf("%d ", rota_arm[w].melhor_rota[y]);
        }
        printf("\n");
    }
    printf("\n_________________________________________________________\n");
}

void imprimir_matriz() {

    printf("\n_________________________________________________________\n");
    printf("\n%s %s\n", m.nome, m.codigo);
    printf("%s %d\n", m.dimensao, m.numero_cidades);
    printf("%s %d\n", m.capacidade, m.carga_maxima);
    printf("\n_________________________________________________________\n");
    printf("\nDemanda:\n\n");

    for (int a = 0; a < m.numero_cidades; a++){
        printf("[%d]%d\n", a, m.demanda_por_cidade[a]);
    }

    printf("\n_________________________________________________________\n");
    printf("\nMatriz:\n\n");

    for(int i = 0; i < m.numero_cidades; i++) {
        for(int j = 0; j < m.numero_cidades; j++) {
            if(m.elementos[i][j] < 10){
                printf(" %d ", m.elementos[i][j]);
            } else {
                printf("%d ", m.elementos[i][j]);
            }
        }
        printf("\n");
    }
}

int main(int argc, char *argv[]) {

      int valor;

      printf ("Insira um valor de 1 a 8 para selecionar uma instancia:\n\n");
      printf("1. P-n16-k8\n");
      printf("2. P-n19-k2\n");
      printf("3. P-n20-k2\n");
      printf("4. P-n23-k8\n");
      printf("5. P-n45-k5\n");
      printf("6. P-n50-k10\n");
      printf("7. P-n51-k10\n");
      printf("8. P-n55-k7\n");
      scanf("%d", &valor);

      switch ( valor )
      {
        case 1 :
        ler_arquivo("P-n16-k8.txt");
        break;

        case 2 :
        ler_arquivo("P-n19-k2.txt");
        break;

        case 3 :
        ler_arquivo("P-n20-k2.txt");
        break;

        case 4 :
        ler_arquivo("P-n23-k8.txt");
        break;

        case 5 :
        ler_arquivo("P-n45-k5.txt");
        break;

        case 6 :
        ler_arquivo("P-n50-k10.txt");
        break;

        case 7 :
        ler_arquivo("P-n51-k10.txt");
        break;

        case 8 :
        ler_arquivo("P-n55-k7.txt");
        break;

        default :
        printf ("Valor invalido!\n");
      }

    if(valor <= 8 && valor > 0){
        imprimir_matriz();
        vizinho_proximo();
        vnd();

    }

    return 0;
}
