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
void inicializaJogador(Jogador *jogador);
void inicializaMatriz(char matriz[3][3]);
int ninguemGanhou(char matriz[3][3], char simbolo);
void exibirTabuleiro(char matriz[3][3]);
void jogadorVsJogador(Jogo jogo);
void jogadorVsComputador();
void abrirJogo(char nomeDoArquivo[20]);
void salvarJogo(Jogo jogo);
void sorteioDaJogadaDaMaquina(char matriz[3][3]);
int jogadaDaMaquina(char matriz[3][3], char simb);
void atualizarRanking(Jogador jogadorUm, Jogador jogadorDois);
Jogador lerJogadorNoArquivo(FILE *file);
void adicionarJogador(int i,Jogador todosJogadores[10], Jogador jogadorDaVez);
void criarArquivoRanking(Jogador jogadorUm, Jogador jogadorDois);

int main(){
    char nomeDoArquivo[20];
    int opcao;
    do{
        opcao = menuPrincipal();
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
                scanf("%s", nomeDoArquivo);
                getchar();
                abrirJogo(nomeDoArquivo);
                break;
            case 3:
                abrirJogo("ContinuaUltJogo.txt");
                break;
            case 4: 
            default:
                break;
        }
    }while(opcao != 0);
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
        jogo.nomeDois[strlen(jogo.nomeDois)-1] = '\0';
        inicializaMatriz(jogo.tabuleiro);
        jogadorVsJogador(jogo);
    }
}

void atualizarRanking(Jogador jogadorUm, Jogador jogadorDois){
    int attJogUm = 0, attJogDois = 0, i, numeroDeJogadores;
    Jogador todosJogadores[10];
    FILE *file = fopen("statusDosJogadores.txt", "r");
    if (file == NULL){
            criarArquivoRanking(jogadorUm, jogadorDois);
            return;
    }

    fscanf(file, "%d", &numeroDeJogadores);
    for (i = 0; i < numeroDeJogadores; i++){
        todosJogadores[i] = lerJogadorNoArquivo(file);
        if (strcmp(todosJogadores[i].nome, jogadorUm.nome) == 0){
            todosJogadores[i].vitorias += jogadorUm.vitorias;
            todosJogadores[i].derrotas += jogadorUm.derrotas;
            todosJogadores[i].empates += jogadorUm.empates;
            attJogUm = 1;
        }else if (strcmp(todosJogadores[i].nome, jogadorDois.nome) == 0){
            todosJogadores[i].vitorias += jogadorDois.vitorias;
            todosJogadores[i].derrotas += jogadorDois.derrotas;
            todosJogadores[i].empates += jogadorDois.empates;
            attJogDois = 1;
        }
    }
    // Se não estiver na lista
    if (attJogUm == 0){
        adicionarJogador(i, todosJogadores, jogadorUm);
        i++;
        numeroDeJogadores++;
    }
    if (attJogDois == 0){
        adicionarJogador(i, todosJogadores, jogadorDois);
        i++;
        numeroDeJogadores++;
    }
    for (int j = 0; j < numeroDeJogadores; j++){
        printf("%d %s\n", j, todosJogadores[j].nome);
    }
    fclose(file);
}

void adicionarJogador(int i, Jogador todosJogadores[10], Jogador jogadorDaVez){
    strcpy(todosJogadores[i].nome, jogadorDaVez.nome);
    todosJogadores[i].vitorias = jogadorDaVez.vitorias;
    todosJogadores[i].derrotas = jogadorDaVez.derrotas;
    todosJogadores[i].empates = jogadorDaVez.empates;
}

void inserirJogadorNoArquivo(FILE *file, Jogador jogador){
    fprintf(file, "%s\n", jogador.nome);
    fprintf(file, "%d %d %d\n", jogador.vitorias, jogador.derrotas, jogador.empates);
}

Jogador lerJogadorNoArquivo(FILE *file){
    Jogador jogador;
    fscanf(file, "%s", jogador.nome);
    fscanf(file, "%d %d %d", &jogador.vitorias, &jogador.derrotas, &jogador.empates);
    printf("lerjog: %s\n", jogador.nome);
    return jogador;
}

void criarArquivoRanking(Jogador jogadorUm, Jogador jogadorDois){
    FILE *file = fopen("statusDosJogadores.txt", "w");
    fprintf(file, "2\n");
    if (jogadorUm.vitorias > jogadorDois.vitorias){
        inserirJogadorNoArquivo(file, jogadorUm);
        inserirJogadorNoArquivo(file, jogadorDois);
    }else{
        inserirJogadorNoArquivo(file, jogadorDois);
        inserirJogadorNoArquivo(file, jogadorUm);
    }
    fclose(file);
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

// vai retonar 1 se é para voltar para o menu principal
int jogada(char nome[], Jogo *jogo, char simbolo){
    char comando[30];
    int linhaJogada, colunaJogada, comandoValido;
    do{
    printf("%s, digite o comando:", nome);
    fgets(comando, 30, stdin);
    comando[strlen(comando)-1] = '\0';
    char marcar[] = {"marcar"};
    char* ponteiro = strstr(comando, marcar);
    if (ponteiro != NULL){
        comandoValido = 1;
        int naoMarcou = 1;
        do{
            // 49 = 48 + 1
            // 48 - é onde começa os números na tabela ASC
            // 1  - porque o índice começa em 0(Zero).
            linhaJogada = (int) comando[7] - 49;
            colunaJogada = (int) comando[8] - 49;
            if(jogo->tabuleiro[linhaJogada][colunaJogada] != ' '){
                printf("Comando inválido %s, digite o comando novamente:", nome);          
                fgets(comando, 30, stdin);
                comando[strlen(comando)-1] = '\0';
            } else {
                jogo->tabuleiro[linhaJogada][colunaJogada] = simbolo;
                naoMarcou = 0;
            }
        } while(naoMarcou == 1);
    }else if(strcmp(comando, "voltar") == 0){
        comandoValido = 1;
        printf("1=%d", jogo->numJogadores);
        strcpy(jogo->nomeDoArquivo, "ContinuaUltJogo.txt");
        printf("2=%d", jogo->numJogadores);
        salvarJogo(*jogo);
        // tem que voltar para o menu nesse ponto, e não continuar o jogo como está no momento
        // DICA: return 1;
        return 1;
    }else if(strcmp(comando, "salvar") == 0){
        comandoValido = 1;
        printf("Digite o nome do jogo: ");
        scanf("%s", jogo->nomeDoArquivo);
        salvarJogo(*jogo); 
        // tem que voltar para o menu nesse ponto, e não continuar o jogo como está no momento
        // DICA: return 1; 
        return 1;
    }else{
        printf("\nComando inválido!!!\n\n");
        comandoValido = 0;
    }
    }while(comandoValido == 0);
    return 0;
}

//Limpa o buffer do teclado
void flush_in(){ 
    int ch;
    while( (ch = fgetc(stdin)) != EOF && ch != '\n' ){} 
}

void jogadorVsJogador(Jogo jogo){ 
    int rodada = 0;
    char simboloDaVez;
    int voltarParaOMenu = 0;
    Jogador jogadorUm, jogadorDois;
    do{
        rodada++;
        system("clear");
        exibirTabuleiro(jogo.tabuleiro);
        if (jogo.ultimoJogador == 2){
            simboloDaVez = 'X';    
            voltarParaOMenu = jogada(jogo.nomeUm, &jogo, simboloDaVez);
            strcpy(jogadorUm.nome, jogo.nomeUm);
            jogo.ultimoJogador = 1;
        }else{
            simboloDaVez = 'O';
            voltarParaOMenu = jogada(jogo.nomeDois, &jogo, simboloDaVez);
            strcpy(jogadorDois.nome, jogo.nomeDois);
            jogo.ultimoJogador = 2;
        }
        if(voltarParaOMenu == 1){
            return;
        }
    }while(ninguemGanhou(jogo.tabuleiro, simboloDaVez) && rodada < 9);
    exibirTabuleiro(jogo.tabuleiro);
    inicializaJogador(&jogadorUm);
    inicializaJogador(&jogadorDois);
    // se o jogo acabou na ultima rodada e ninguem ganhou então deu velha
    if(rodada == 9 && ninguemGanhou(jogo.tabuleiro, simboloDaVez)){
        printf("Deu velha!\n");
        jogadorUm.empates = 1;
        jogadorDois.empates = 1;
    }else if (rodada % 2 != 0){
        printf("Parabéns %s, você ganhou!\n Digite qualquer tecla para continuar!\n", jogo.nomeUm);
        jogadorUm.vitorias = 1;
        jogadorDois.derrotas = 1;
    }else{
        printf("Parabéns %s, você ganhou!\n Digite qualquer tecla para continuar!\n", jogo.nomeDois);
        jogadorDois.vitorias = 1;
        jogadorUm.derrotas = 1;
    }
    atualizarRanking(jogadorUm, jogadorDois);
    getchar();
    exibirOpcoes();
}

void inicializaJogador(Jogador *jogador){
    jogador->derrotas = 0;
    jogador->empates = 0;
    jogador->vitorias = 0;
}

// fazer a mesma coisa que foi feita pro jogadorVsJogador                                                                                                                                                   
void jogadorVsComputador(Jogo jogo){
    int rodada = 0;
    char simboloDaVez;
    int voltarParaOMenu = 0;
    do{
        rodada++;
        system("clear");
        exibirTabuleiro(jogo.tabuleiro);
        if (jogo.ultimoJogador == 2){
            simboloDaVez = 'X';
            voltarParaOMenu = jogada(jogo.nomeUm, &jogo, simboloDaVez);
            jogo.ultimoJogador = 1;
        }else{
            simboloDaVez = 'O';
            if (jogadaDaMaquina(jogo.tabuleiro, 'O') == 0){
                if (jogadaDaMaquina(jogo.tabuleiro, 'X') == 0){
                    sorteioDaJogadaDaMaquina(jogo.tabuleiro);
                }
            }
            jogo.ultimoJogador = 2;
        }
        if(voltarParaOMenu == 1){
            return;
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

int jogadaDaMaquina(char matriz[3][3], char simb){
    if (matriz[0][0] == matriz [1][1] && matriz[0][0] == simb && matriz[2][2] == ' '){
        matriz[2][2] = 'O';
        return 1;
    }else if (matriz[0][0] == matriz [2][2] && matriz[0][0] == simb && matriz[1][1] == ' '){
        matriz[1][1] = 'O';
        return 1;
    }else if (matriz[1][1] == matriz [2][2] && matriz[1][1] == simb && matriz[0][0] == ' '){
        matriz[0][0] = 'O';
        return 1;
    }else if (matriz[0][2] == matriz [1][1] && matriz[1][1] == simb && matriz[2][0] == ' '){
        matriz[2][0] = 'O';
        return 1;
    }else if (matriz[0][2] == matriz [2][0] && matriz[0][2] == simb && matriz[1][1] == ' '){
        matriz[1][1] = 'O';
        return 1;
    }else if (matriz[1][1] == matriz [2][0] && matriz[1][1] == simb && matriz[0][2] == ' '){
        matriz[0][2] = 'O';
        return 1;
    }
    for(int i = 0; i < 3; i++){
        if (matriz[i][0] == matriz[i][1] && matriz[i][0] == simb && matriz[i][2] == ' '){
            matriz[i][2] = 'O';
            return 1;
        }else if (matriz[i][1] == matriz[i][2] && matriz[i][1] == simb && matriz[i][0] == ' '){
            matriz[i][0] = 'O';
            return 1;
        }else if (matriz[i][0] == matriz[i][2] && matriz[i][0] == simb && matriz[i][1] == ' '){
            matriz[i][1] = 'O';
            return 1;
        }else if (matriz[0][i] == matriz [1][i] && matriz[0][i] == simb && matriz[2][i] == ' '){
            matriz[2][i] = 'O';
            return 1;
        }else if (matriz[1][i] == matriz [2][i] && matriz[1][i] == simb && matriz[0][i] == ' '){
            matriz[0][i] = 'O';
            return 1;
        }else if (matriz[0][i] == matriz [2][i] && matriz[0][i] == simb && matriz[1][i] == ' '){
            matriz[1][i] = 'O';
            return 1;
        }
    }
    return 0;
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

