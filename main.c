#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include <time.h>

typedef struct{
    char nome[30], simbolo;
    int vitorias, empates, derrotas;
}Jogador;

typedef struct{
    char nomeDoArquivo[20];
    int numJogadores;
    char nomeUm[30], nomeDois[30];
    char tabuleiro[3][3];
    int ultimoJogador;
}Jogo;

int menuPrincipal();
void exibirOpcoes();
void novoJogo();
Jogador inicializaJogador(char nome[30], char simbolo);
void inicializaMatriz(char matriz[3][3]);
int ninguemGanhou(char matriz[3][3], char simbolo);
void exibirTabuleiro(char matriz[3][3]);
void jogadorVsJogador();
void jogadorVsComputador();

int main(){
    char nomeDoArquivo[20];
    int opcao = menuPrincipal();
    switch (opcao){
        case 0:
            printf("Obrigada por participar do nosso jogo!");
            return 0;
            break;
        case 1:
            novoJogo();
            break;
        case 2:
            printf("Qual o nome do arquivo do jogo que você deseja jogar?");
            scanf("%s", &nomeDoArquivo);
            abrirJogo(nomeDoArquivo);
            break;
        case 3:
            abrirJogo("jogoAtual.txt");
            break;
        default:
            break;
    }
    return 0;
}

int menuPrincipal(){
    int opcao;
    printf("Bem vindo ao Jogo da Velha\n\n");
    exibirOpcoes();
    printf("Escolha a opção: ");
    scanf("%d", &opcao);
    while(opcao < 0  || opcao > 4){
        system("clear");
        printf("Opção inválida!\n");
        exibirOpcoes();
        printf("Escolha uma opção válida: ");
        scanf("%d", &opcao);
    }
    return opcao;
}

void exibirOpcoes(){
    printf("0. Sair do Jogo\n");
    printf("1. Começar um novo jogo\n");
    printf("2. Continuar um jogo salvo\n");
    printf("3. Continuar o jogo atual\n");
    printf("4. Exibir o ranking\n");
    printf("Durante o jogo digite “voltar” para retornar ao menu\n\n");
}

void novoJogo(){
    int numDeJogadores;
    printf("Digite o número de jogadores (1 ou 2): ");
    scanf("%d", &numDeJogadores);
    if (numDeJogadores == 1){
        Jogo jogo;
        jogo.numJogadores = 1;
        jogo.ultimoJogador = 2;
        getchar();
        printf("Digite o nome do jogador: ");
        fgets(jogo.nomeUm, 30, stdin);
        jogo.nomeUm[strlen(jogo.nomeUm)-1] = '\0';
        strcpy(jogo.nomeDois,"Computador");
        inicializaMatriz(jogo.tabuleiro);
        jogadorVsComputador(jogo);
    } else if (numDeJogadores == 2){
        Jogo jogo;
        jogo.numJogadores = 2;
        getchar();    
        jogo.ultimoJogador = 2;
        printf("Digite o nome do jogador 1: ");
        fgets(jogo.nomeUm, 30, stdin);
        jogo.nomeUm[strlen(jogo.nomeUm)-1] = '\0';
        printf("Digite o nome do jogador 2: ");
        fgets(jogo.nomeDois, 30, stdin);
        inicializaMatriz(jogo.tabuleiro);
        jogadorVsJogador(jogo);
    }
}

Jogador inicializaJogador(char nome[30], char simbolo){
    Jogador jogador;

    jogador.simbolo = simbolo;
    strcpy(jogador.nome, nome);
    // verificar quantas derrotas e empates e vitórias tem o jogador
    jogador.derrotas = 0;
    jogador.empates = 0;
    jogador.vitorias = 0;    
    return jogador;
}

void inicializaMatriz(char matriz[3][3]){
    for(int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            matriz[i][j] = ' ';
        }
    }
}

int ninguemGanhou(char matriz[3][3], char simbolo){
    int marcadoMesmaLinha = 1, marcadoMesmaColuna = 1;
    if(matriz[0][0] == simbolo && matriz[0][0] == matriz[1][1] && matriz[1][1] == matriz[2][2]){
        return 0;
    }
    if (matriz[2][0] == simbolo && matriz[0][2] == matriz[1][1] && matriz[1][1] == matriz[2][0]){
        return 0;
    }
    for(int i = 0; i < 3; i++){
        for (int j = 0; j < 2; j++){
            if (matriz[i][j] == matriz[i][j+1] && matriz[i][j] == simbolo){
                marcadoMesmaLinha++;
            }else if (matriz[j][i] == matriz [j+1][i] && matriz[j][i] == simbolo){
                marcadoMesmaColuna++;
            }
            if (marcadoMesmaLinha == 3 || marcadoMesmaColuna == 3){
                return 0;
            }
        }
        marcadoMesmaLinha = 1;
        marcadoMesmaColuna = 1;
    }
    return 1;
}

void exibirTabuleiro(char matriz[3][3]){
    printf(" %c | %c | %c \n", matriz[0][0], matriz[0][1], matriz[0][2]);
    printf("---|---|---\n");
    printf(" %c | %c | %c \n", matriz[1][0], matriz[1][1], matriz[1][2]);
    printf("---|---|---\n");
    printf(" %c | %c | %c \n", matriz[2][0], matriz[2][1], matriz[2][2]);
}

void jogada(char nome[], Jogo *jogo, char simbolo){
    char comando[7], nomeDoArqDoUsuario;
    int jogada, linhaJogada, colunaJogada;
    //do{
    printf("%s, digite o comando:", nome);
    scanf("%s", comando);
    //} while(jogo->tabuleiro[linhaJogada][colunaJogada] != ' ');
    if (strcmp(comando, "marcar") == 0){
        scanf("%d", &jogada);
        linhaJogada = (jogada / 10) - 1;
        colunaJogada = (jogada % 10) - 1;
        jogo->tabuleiro[linhaJogada][colunaJogada] = simbolo;
    }else if(strcmp(comando, "voltar") == 0){
        strcpy(jogo->nomeDoArquivo, "jogoAtual.txt");
        salvarJogo(*jogo);
    }else if(strcmp(comando, "salvar") == 0){
        scanf("%s", jogo->nomeDoArquivo);
        salvarJogo(*jogo); 
    }else {
        printf("Comando inválido");
    }
}

//Limpa o buffer do teclado
void flush_in(){ 
    int ch;
    while( (ch = fgetc(stdin)) != EOF && ch != '\n' ){} 
}

void jogadorVsJogador(Jogo jogo){
    int rodada = 0;
    char simboloDaVez;
    do{
        rodada++;
        jogo.ultimoJogador = rodada % 2 + 1;
        system("clear");
        exibirTabuleiro(jogo.tabuleiro);
        if (rodada % 2 == 0){
            simboloDaVez = 'X';
            jogada(jogo.nomeUm, &jogo, simboloDaVez);
        }else{
            simboloDaVez = 'Y';
            jogada(jogo.nomeDois, &jogo, simboloDaVez);
        }
    }while(ninguemGanhou(jogo.tabuleiro, simboloDaVez) && rodada < 9);
    exibirTabuleiro(jogo.tabuleiro);
    // se o jogo acabou na ultima rodada e ninguem ganhou então deu velha
    if(rodada == 9 && ninguemGanhou(jogo.tabuleiro, simboloDaVez)){
        printf("Deu velha!\n");
    } else if (rodada % 2 != 0){
        printf("Parabéns %s, você ganhou!\n Digite qualquer tecla para continuar!\n", jogo.nomeUm);
    }else{
        printf("Parabéns %s, você ganhou!\n Digite qualquer tecla para continuar!\n", jogo.nomeDois);
    }
    getchar();
    exibirOpcoes();
}

void jogadorVsComputador(Jogo jogo){
    int rodada = 0;
    char simboloDaVez;
    do{
        rodada++;
        jogo.ultimoJogador = rodada % 2 + 1;
        system("clear");
        exibirTabuleiro(jogo.tabuleiro);
        if (rodada % 2 != 0){
            simboloDaVez = 'X';
            jogada(jogo.nomeUm, &jogo, simboloDaVez);
        }else{
            simboloDaVez = 'O';
            jogadaDaMaquina(jogo.tabuleiro);
        }
    }while(ninguemGanhou(jogo.tabuleiro, simboloDaVez) && rodada < 9);
    system("clear");
    exibirTabuleiro(jogo.tabuleiro);
    // se o jogo acabou na ultima rodada, e ninguem ganhou então deu velha 
    if(rodada == 9 && ninguemGanhou(jogo.tabuleiro, simboloDaVez)){
        printf("Deu velha!\n");
    } else if (rodada % 2 != 0){
        printf("Parabéns %s, você ganhou!\n Digite qualquer tecla para continuar!\n", jogo.nomeUm);
    }else{
        printf("O computador ganhou!\n Digite qualquer tecla para continuar!\n");
    }
    getchar();
    exibirOpcoes();
}

void sorteioDaJogadaDaMaquina(char matriz[3][3]){
    int sorteioUm, sorteioDois;
    srand(time(NULL));
    do{
        sorteioUm = rand() % 3;
        sorteioDois = rand() % 3;
    }while(matriz[sorteioUm][sorteioDois] != ' ');
    matriz[sorteioUm][sorteioDois] = 'O';
}

void jogadaDaMaquina(char matriz[3][3]){
    int i,j;
    if (matriz[0][0] == matriz [1][1] && matriz[0][0] == 'X' && matriz[2][2] == ' '){
        matriz[2][2] = 'O';
        return;
    }else if (matriz[0][0] == matriz [2][2] && matriz[0][0] == 'X' && matriz[1][1] == ' '){
        matriz[1][1] = 'O';
        return;
    }else if (matriz[1][1] == matriz [2][2] && matriz[1][1] == 'X' && matriz[0][0] == ' '){
        matriz[0][0] = 'O';
        return;
    }else if (matriz[0][2] == matriz [1][1] && matriz[1][1] == 'X' && matriz[2][0] == ' '){
        matriz[2][0] = 'O';
        return;
    }else if (matriz[0][2] == matriz [2][0] && matriz[0][2] == 'X' && matriz[1][1] == ' '){
        matriz[1][1] = 'O';
        return;
    }else if (matriz[1][1] == matriz [2][0] && matriz[1][1] == 'X' && matriz[0][2] == ' '){
        matriz[0][2] = 'O';
        return;
    }
    for(int i = 0; i < 3; i++){
        if (matriz[i][0] == matriz[i][1] && matriz[i][0] == 'X' && matriz[i][2] == ' '){
            matriz[i][2] = 'O';
            return;
        }else if (matriz[i][1] == matriz[i][2] && matriz[i][1] == 'X' && matriz[i][0] == ' '){
            matriz[i][0] = 'O';
            return;
        }else if (matriz[i][0] == matriz[i][2] && matriz[i][0] == 'X' && matriz[i][1] == ' '){
            matriz[i][1] = 'O';
            return;
        }else if (matriz[0][i] == matriz [1][i] && matriz[0][i] == 'X' && matriz[2][i] == ' '){
            matriz[2][i] = 'O';
            return;
        }else if (matriz[1][i] == matriz [2][i] && matriz[1][i] == 'X' && matriz[0][i] == ' '){
            matriz[0][i] = 'O';
            return;
        }else if (matriz[0][i] == matriz [2][i] && matriz[0][i] == 'X' && matriz[1][i] == ' '){
            matriz[1][i] = 'O';
            return;
        }
    }
    sorteioDaJogadaDaMaquina(matriz);
}

void salvarJogo(Jogo jogo){
    FILE *file = fopen(jogo.nomeDoArquivo, "w");
    if(file == NULL){
        printf("Erro ao abrir o arquivo!");
    }
    fprintf(file, "%d\n", jogo.numJogadores);
    fprintf(file, "%s\n", jogo.nomeUm);
    fprintf(file, "%s\n", jogo.nomeDois);
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            if (jogo.tabuleiro[i][j] == ' '){
                fprintf(file, " - ");
            } else {
                fprintf(file, " %c ", jogo.tabuleiro[i][j]);
            }
        }
        fprintf(file, "\n");
    }
    printf("entrou aqui\n");
    fprintf(file, "%d", jogo.ultimoJogador);
    fclose(file);
}

void abrirJogo(char nomeDoArquivo[20]){
    FILE *file = fopen(nomeDoArquivo, "r");
    Jogo jogo;
    fscanf(file, "%d", &jogo.numJogadores);
    fgetc(file);
    fgets(jogo.nomeUm, 30, file);
    jogo.nomeUm[strlen(jogo.nomeUm)-1] = '\0';
    fgets(jogo.nomeDois, 30, file);
    jogo.nomeDois[strlen(jogo.nomeDois)-1] = '\0';
    for (int i = 0; i < 3; i++){
        fscanf(file, " %c  %c  %c ", &jogo.tabuleiro[i][0], &jogo.tabuleiro[i][1], &jogo.tabuleiro[i][2]);
    }
    printf("\nNum de jogadores: %d\n Nome do 1: %s \n Nome do 2:%s\n", jogo.numJogadores, jogo.nomeUm, jogo.nomeDois);
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            if (jogo.tabuleiro[i][j] == '-'){
                jogo.tabuleiro[i][j] = ' ';
            }
        }
    }
    fscanf(file, "%d", &jogo.ultimoJogador);
    if (jogo.numJogadores == 2){
        jogadorVsJogador(jogo);
    }else if (jogo.numJogadores == 1){
        jogadorVsComputador(jogo);
    }
}