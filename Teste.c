    #include <conio.h>
#include <locale.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define POP 30



int dis_cidades [10][10] = {{0,90,15,30,40,80,30,90,30,20},
	{90,0,40,100,10,20,50,60,60,50},
	{15,40,0,60,20,80,100,10,90,10},
	{30,100,60,0,40,50,60,80,80,50},
	{40,10,20,40,0,40,80,30,70,70},
	{80,20,80,50,40,0,30,60,10,20},
	{30,50,100,60,80,30,0,50,60,50},
	{90,60,10,80,30,60,50,0,60,20},
	{30,60,90,80,70,10,60,60,0,80},
	{20,50,10,50,70,20,50,20,80,0}};

int TIPO_SEL, QTD_GER, K, POP;
float TX_MUT;
int populacao[POP][10], geracao[POP][10], filho[10];
int apt[POP],ger=2;      // valores de aptidão
float aptip[]; // valores de aptdão inversamente proporcionais
int perfeito, achou=0, total=0;
struct s{
  int individuo[10];
  int apt;
}selecionado;

//------------------------------------------------------------------------------------------------------------------------------

void verifica_gene(int ind, int pos_atual){           // elimina genes repetidos
        for(register int k=0; k<pos_atual; k++)
            if(populacao[ind][k] == populacao[ind][pos_atual]){
                populacao[ind][pos_atual]++;
                populacao[ind][pos_atual] = populacao[ind][pos_atual]%10;
                verifica_gene(ind, pos_atual);
            }
}
//------------------------------------------------------------------------------------------------------------------------------

int indiv_existe(int pos){                           //elimina indivíduos repetidos
     int j;
     for(register int i=0; i<pos; i++){
        for(j=0; j<10; j++){
           if(populacao[i][j] != populacao[pos][j]){
              break;
           }
        }
        if(j==10)return 1;
     }
     return 0;
}

//--------------------------------------------------------------------------------------------------------------------------------


void aptdao() {

    int j, i, h;
    int aptdaotemp;
    for (i = 0; i < POP; i++) {
        for (h = 0; h < 9; h++) {
            j = h + 1;             // J = Proxima Posicao
            aptdaotemp = aptdaotemp + dis_cidades[populacao[i][h]][populacao[i][j]];
        }
        apt[i] = aptdaotemp;
        aptdaotemp = 0;

    }
}
//------------------------------------------------------------------------------

void primeira_geracao(){
for(register int i=0; i<POP; i++){
    for(register int j=0; j<10; j++){
        populacao[i][j] = rand()%10;
        verifica_gene(i, j);
    }
    if(i)
        if(indiv_existe(i))i-=1;
}

aptdao();
printf("\nGERAÇÃO 1\n");
for(register int i=0; i<POP; i++){
    printf("indivíduo %i: ", i+1);
    for(register int j=0; j<10; j++){
        printf("%i | ", populacao[i][j]);
    }
    printf("  Distância: %iKM\n", apt[i]);
}

    int MenorCaminho = 10000;
    int PosicaoMenorCaminho;

    for (int i = 0; i < POP; i++) {
        if (apt[i] < MenorCaminho) {
            MenorCaminho = apt[i];
            PosicaoMenorCaminho = i;
        }
    }

    printf("\nO Melhor percurso foi: %i\n", PosicaoMenorCaminho + 1);

    for (register int j = 0; j < 10; j++) {
        printf("%i - ", populacao[PosicaoMenorCaminho][j]);
    }

    printf("  Distância percorrida: %i km\n", apt[PosicaoMenorCaminho]);


}

//------------------------------------------------------------------------------

void calc_apt_inv(){ //CÁLCULO DA APTIDÃO INVERSAMENTE PROPORCIONAL
     float media, total_inv=0;
     total=0;
     for(register int x=0; x<POP; x++){
             total+=apt[x];
             if(!apt[x])continue;
             total_inv+=(1/(float)apt[x]);
     }
     for(register int x=0; x<POP; x++){
             aptip[x]=(float)(total/(apt[x]*total_inv));
     }
}


//------------------------------------------------------------------------------

void torneio(){
     int candidato, melhor;
     selecionado.apt = 10000;
     for(register int i=0; i<K; i++){
         candidato=rand()%POP;
         if(apt[candidato]<selecionado.apt){
             selecionado.apt = apt[candidato];
             melhor=candidato;
             for(register int k=0; k<10; k++)
                     selecionado.individuo[k] = populacao[candidato][k];

         }
     }
}

//------------------------------------------------------------------------------

void roleta(){
     int achou=0, i=0, sort;
     float soma=0;
     calc_apt_inv();
     do{sort = rand()%total; }while(!sort);
     achou=0;
     i=0;
     soma=0;
     while(!achou){
         soma+=aptip[i];
         if(soma >= sort){
                 for(register int k=0; k<10; k++)
                     selecionado.individuo[k] = populacao[i][k];
                 selecionado.apt = apt[i];
                 achou=1;
         }
         i++;
     }
}

//------------------------------------------------------------------------------

void mutacao_ordem(){
     int aux, pos_al;
     for(register int k=0; k<10; k++){
       if(!(rand()%(int)(100/TX_MUT))){
          do{pos_al = rand()%10;}while(pos_al == k);
          aux = filho[k];
          filho[k] = filho[pos_al];
          filho[pos_al] = aux;
       }
     }
}

//------------------------------------------------------------------------------

void reproducao_ordem(){
     int i, indice, pai1[10], pai2[10], corte;
     switch(TIPO_SEL){
        case 1:
             roleta();
             for(register int k=0; k<10; k++)
             pai1[k] = selecionado.individuo[k];

             roleta();
             for(register int k=0; k<10; k++)
             pai2[k] = selecionado.individuo[k];

             break;
        case 2:
             torneio();
             for(register int k=0; k<10; k++)
             pai1[k] = selecionado.individuo[k];

             torneio();
             for(register int k=0; k<10; k++)
             pai2[k] = selecionado.individuo[k];

             break;
     }
     corte = rand()%9;
     for(i=0; i<=corte; i++){
        filho[i] = pai1[i];
     }
     indice=i;

     for(register int j=0; j<10; j++){
        for(register int k=indice; k<10; k++){
           if(pai2[j] == pai1[k]){
              filho[i] = pai1[k];
              i++;
              break;
           }
        }
     }
}

//------------------------------------------------------------------------------

void nova_geracao(){
    for(register int i=0; i<POP; i++){
      reproducao_ordem();
      mutacao_ordem();
      for(register int k=0; k<10; k++)
         geracao[i][k] = filho[k];
    }
}

//------------------------------------------------------------------------------

void atualizacao(){
    nova_geracao();
    for(register int i=0; i<POP; i++){
      for(register int j=0; j<10; j++){
          populacao[i][j] = geracao[i][j];
       }
    }
    printf("\nGERAÇÃO %d\n", ger++);
    aptdao();
    for(register int i=0; i<POP; i++){
       printf("indivíduo %i: ", i+1);
       for(register int j=0; j<10; j++){
         printf("%i | ", populacao[i][j]);
       }
       printf("  Distância: %iKM\n", apt[i]);
    }
}

//------------------------------------------------------------------------------

int main(){

int x=0, op;

setlocale(LC_ALL, "portuguese");
srand(time(0));
system("color 0");
printf("\tCAXEIRO VIAJANTE IA ALGORITIMO GENETICO \n");

    printf("Digite a quantidade da População: ");
	    fflush(stdin);
	    scanf("%i", &POP);
    do{
        printf("\n\nDefina tipo de seleção: \n1- Roleta Viciada\n2- Torneio\n");
        fflush(stdin);
        scanf("%i", &TIPO_SEL);
        if(TIPO_SEL == 2){
           printf("\nDigite o tamanho do torneio \n");
           fflush(stdin);
           scanf("%d", &K);
        }
    }while(TIPO_SEL!=1 && TIPO_SEL!=2);
    do{
        printf("\nDigite a quantidade de gerações (maior que 1)\n");
        fflush(stdin);
        scanf("%i", &QTD_GER);
    }while(QTD_GER < 2);
    do{
        printf("\nDigite a taxa de mutação (entre 0-5%%)\n");
        fflush(stdin);
        scanf("%f", &TX_MUT);

    }while(TX_MUT<=0|| TX_MUT>5);


primeira_geracao();
getch();
for(register int x=0; x<(QTD_GER-1); x++){
   atualizacao();



    int MenorCaminho = 10000;
    int PosicaoMenorCaminho;

    for (int i = 0; i < POP; i++) {
        if (apt[i] < MenorCaminho) {
            MenorCaminho = apt[i];
            PosicaoMenorCaminho = i;
        }
    }

    printf("\nO Melhor percurso foi o %i\n", PosicaoMenorCaminho + 1);

    for (register int j = 0; j < 10; j++) {
        printf("%i - ", populacao[PosicaoMenorCaminho][j]);
    }

    printf("  Distância percorrida: %i km\n", apt[PosicaoMenorCaminho]);

   getch();
}

system("pause");
}

