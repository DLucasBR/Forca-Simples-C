#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "forca_simples.h"

/*Na versao 2.0 deixa esses tamanhos todos dinamicos e veja a utilizaçao do fflush*/

char chute, chute_convertido;

short acertos[TAMANHO_MAXIMO_DA_PALAVRA_SECRETA];
short qt_chutes = 0;

char palavra [TAMANHO_MAXIMO_DA_PALAVRA_SECRETA];
int tamanho_da_palavra_sorteada = 0;

char  letra_repetida[255]; // registra quais letras repetidas de palavras
short acertos_repetidos = 0;
short erros = 0;

void pegachute(){

   short chutevalido = 0;

   do{
       printf("\nDigite um chute, apenas uma letra sera considerado\n");
       scanf(" %c",&chute);

       fflush(stdin);

       if(chute >= 97 && chute <= 122){
          chute_convertido = chute - 32;
          qt_chutes++;
          chutevalido = 1;}
       else if (chute >= 65 && chute <= 90){
          chute_convertido = chute + 32;
          qt_chutes++;
          chutevalido = 1;}
       else{
          printf("Seu chute deve conter apenas alguma das 26 letras do alfabeto, tente novamente\n");
       }
    }while(!chutevalido);
}
void verificachute(){
    short i;

    verifica_letra_repetida();
    imprimecabecalho();

    for(i = 0; i < strlen(palavra); i++ ){
       if(acertos[i]){
         printf("%c ",palavra[i]);
         continue;}
       else if (chute == palavra[i] || chute_convertido == palavra[i]){
         printf("%c ", palavra[i]);
         acertos[i] = 1;
       }
       if(!acertos[i])
         printf("_ ");
       }
   erros = calcularerros();
   printf("\n\n\nNumero de tentativas: %d\t",qt_chutes);
   printf("Numero de erros: %d\n", erros); // pode otimizar essa chamada já que ela ja é feita na codicao do do while
   desenhaforca(erros);
}
short verificavitoria(){
    short i;
    for( i = 0 ; i < tamanho_da_palavra_sorteada ; i++ ){
        if(!acertos[i])
            return 0;
    }
    desenhavitoria();
    return 1;
}
void imprimecabecalho(){
    system("cls");
    printf("\t\t\t\t***********************************************************\n"    );
    printf("\t\t\t\t*               JOGO DA FORCA - LIKE A BOSS               *\n"    );
    printf("\t\t\t\t*  UMA PALAVRA ALEATORIA SERA SORTEADA DO ARQUIVO BD.txt  *\n"    );
    printf("\t\t\t\t*    ACERTE A PALAVRA ERRANDO ATE 5 VEZES OU MORRA X-P    *\n"    );
    printf("\t\t\t\t***********************************************************\n\n\n");
}
void inicializajogo(){

    escolhepalavra();

    tamanho_da_palavra_sorteada = strlen(palavra);
    imprimecabecalho();
    desenhaforca(erros);

    short i;
    /*imprime o espaços pela primeira vez para o usuario saber o tamanho da palavra*/
    for(i = 0; i < tamanho_da_palavra_sorteada;i++){
        printf("_ ");
    }
    /*verifica se a palavra tem letras repetidas para considerar isso no momento de contabilizar os acertos e erros*/
    calcula_letras_repetidas();
    /*inicializa o vetor que verifica quais letras o usuario ja acertou*/
    for(i = 0 ; i < TAMANHO_MAXIMO_DA_PALAVRA_SECRETA; i++)
        acertos[i] = 0;
}
void calcula_letras_repetidas(){
    /*Letras repetidas interferem no calculo do numero de erros*/

    short i,j;

    for(i = 0 ; i < 255; i++)
        letra_repetida[i] = 0;

    for( i = 0; i < tamanho_da_palavra_sorteada; i++){
        for(j = 64 ;j < 123;j++){
            if(j > 90  && j < 97)
                continue;
            if(palavra[i] == j){
                letra_repetida[j]++;
                continue;}
    }}
}
void verifica_letra_repetida(){
   /*Se a palavra contiver a letra chutada mais de uma vez registra para subtrair da contagem de acertos final*/
    if(letra_repetida[chute] > 1)
        acertos_repetidos += letra_repetida[chute] - 1;
    if(letra_repetida[chute_convertido] > 1)
        acertos_repetidos += letra_repetida[chute_convertido] - 1;
    /* *** */
}
short calcularerros(){
    short i, acertou = 0;
    for(i = 0; i < tamanho_da_palavra_sorteada; i++){
        if(acertos[i])
            acertou++;
    }
   return qt_chutes - (acertou - acertos_repetidos) ;
}
short verificaenforcado(){
    if( calcularerros() > 4){
        desenhaderrota();
        return 1;}
    return 0;
}
void escolhepalavra(){
    FILE* f;
    int i;

    f = fopen("palavras.txt", "r");

    if(f == 0) {
        printf("Arquivo de palavras não disponível\n\n");
        exit(1);
    }

    int qtddepalavras;
    fscanf(f, "%d", &qtddepalavras);

    srand(time(0));
    int randomico = rand() % qtddepalavras;

    for(i = 0; i <= randomico; i++) {
        fscanf(f, "%s", palavra);
    }

    fclose(f);
}
void desenhaforca(short erros){
    printf("  _______       \n");
    printf(" |/      |      \n");
    printf(" |      %c%c%c  \n", (erros>=1?'(':' '),
        (erros>=1?'_':' '), (erros>=1?')':' '));
    printf(" |      %c%c%c  \n", (erros>=3?'\\':' '),
        (erros>=2?'|':' '), (erros>=3?'/': ' '));
    printf(" |       %c     \n", (erros>=2?'|':' '));
    printf(" |      %c %c   \n", (erros>=4?'/':' '),
        (erros>=4?'\\':' '));
    printf(" |              \n");
    printf("_|___           \n");
    printf("%s", erros>=4? "Mais um erro e voce ja era! huehueh":"");
    printf("\n\n");
}
void desenhavitoria(){
        imprimecabecalho();
        printf("%s\n\n", palavra);
        printf("\nAcerto Miseravei!\n\n");

        printf("       ___________      \n");
        printf("      '._==_==_=_.'     \n");
        printf("      .-\\:      /-.    \n");
        printf("     | (|:.     |) |    \n");
        printf("      '-|:.     |-'     \n");
        printf("        \\::.    /      \n");
        printf("         '::. .'        \n");
        printf("           ) (          \n");
        printf("         _.' '._        \n");
        printf("        '-------'       \n\n");
}
void desenhaderrota(){
    imprimecabecalho();
    printf("Perdeu playboy!\t");
    printf("A palavra era **%s**\n\n", palavra);

    printf("    _______________         \n");
    printf("   /               \\       \n");
    printf("  /                 \\      \n");
    printf("//                   \\/\\  \n");
    printf("\\|   XXXX     XXXX   | /   \n");
    printf(" |   XXXX     XXXX   |/     \n");
    printf(" |   XXX       XXX   |      \n");
    printf(" |                   |      \n");
    printf(" \\__      XXX      __/     \n");
    printf("   |\\     XXX     /|       \n");
    printf("   | |           | |        \n");
    printf("   | I I I I I I I |        \n");
    printf("   |  I I I I I I  |        \n");
    printf("   \\_             _/       \n");
    printf("     \\_         _/         \n");
    printf("       \\_______/           \n");
}

int main(){

    inicializajogo();

    do{
        pegachute();
        verificachute();
      }while(!verificavitoria() && !verificaenforcado());

      getchar();
return 0;

}
