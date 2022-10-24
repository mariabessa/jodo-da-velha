#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include <time.h>
#define ANSI_RESET            "\x1b[0m" 
#define ANSI_BOLD             "\x1b[1m"  // coloca o texto em negrito
#define ANSI_COLOR_RED        "\x1b[31m" // cor para o comando 'X'
#define ANSI_COLOR_CYAN       "\x1b[36m" // cor para o comando 'Y'
#define BOLD(string)       ANSI_BOLD             string ANSI_RESET
#define RED(string)        ANSI_COLOR_RED        string ANSI_RESET
#define CYAN(string)       ANSI_COLOR_CYAN       string ANSI_RESET
#define TAB_HOR "\u2501"  // caracteres uteis para a tabela 
#define TAB_VER "\u2503"
#define TAB_TL  "\u250F"
#define TAB_ML  "\u2523" 
#define TAB_BL  "\u2517" 
#define TAB_TJ  "\u2533"
#define TAB_MJ  "\u254B" 
#define TAB_BJ  "\u253B" 
#define TAB_TR  "\u2513" 
#define TAB_MR  "\u252B"  
#define TAB_BR  "\u251B"  

typedef struct{
    char nome[30], simbolo;
    int vitorias, empates, derrotas;
}Jogador;

typedef struct{
    char nomeDoArquivo[20];
    int numJogadores;
    char nomeUm[30], nomeDois[30];
    char **tabuleiro;
    int ultimoJogador;
}Jogo;

int menuPrincipal();
char ** inicializaMatriz();
void novoJogo();
void adicionarJogador(int i,Jogador todosJogadores[10], Jogador jogadorDaVez);
void jogadorVsJogador(Jogo jogo);
void jogadorVsComputador();
int jogada(char nome[], Jogo *jogo, char simbolo);
void salvarJogo(Jogo jogo);
void abrirJogo(char nomeDoArquivo[20]);
int ninguemGanhou(char **matriz, char simbolo);
int jogadaDaMaquina(char **matriz, char simb);
void sorteioDaJogadaDaMaquina(char **matriz);
void atualizarRanking(Jogador jogadorUm, Jogador jogadorDois);
Jogador lerJogadorNoArquivo(FILE *file);
void criarArquivoRanking(Jogador jogadorUm, Jogador jogadorDois);
void inserirJogadorNoArquivo(FILE *file, Jogador jogador);
void inicializaJogador(Jogador *jogador);
void ordenarTodosJogadores(Jogador todosJogadores[10], int numeroDeJogadores);
void exibirRanking(char arquivo[12]);
void exibirTabuleiro(char **matriz);

int main(){
    char nomeDoArquivo[20];
    char opcao;
    printf("Bem vindo ao Jogo da Velha");
    do{
        opcao = menuPrincipal();
        switch (opcao){
            case '0':
                printf("Obrigada por participar do nosso jogo!\n");
                return 0;
                break;
            case '1':
                novoJogo();
                break;
            case '2':
                printf("Qual o nome do arquivo do jogo que você deseja jogar?");
                scanf("%s", nomeDoArquivo);
                getchar();
                abrirJogo(nomeDoArquivo);
                break;
            case '3':
                abrirJogo("ContinuaUltJogo.txt");
                break;
            case '4': 
                exibirRanking("ranking.txt");
                break;
            default:
                printf("Opção inválida! Digite um valor válido: ");
                scanf("%c", &opcao);
                continue;
        }
    }while(opcao != '0');
    return 0;
}

//inicializa a matriz vazia para inicio de jogo
char ** inicializaMatriz(){ 
    char ** matriz = malloc(3 * sizeof(char));
    for(int i = 0; i < 3; i++){
        matriz[i] = malloc(3 * sizeof(char));
        for (int j = 0; j < 3; j++){
            matriz[i][j] = ' ';
        }
    }
    return matriz;
}

//função que imprime o menu principal e retorna a opcão selecionada
int menuPrincipal(){ 
    char opcao;
    printf("\n \n0. Sair do Jogo\n");
    printf("1. Começar um novo jogo\n");
    printf("2. Continuar um jogo salvo\n");
    printf("3. Continuar o jogo atual\n");
    printf("4. Exibir o ranking\n");
    printf("Durante o jogo digite “voltar” para retornar ao menu\n\n");
    printf("Escolha a opção: ");
    scanf("%c", &opcao);
    return opcao;
}

//função para verificar o número de jogadores selecionado para um novo jogo
void novoJogo(){
    int numDeJogadores;
    Jogo jogo;  
    jogo.ultimoJogador = 2; //para controlar o próximo jogador da partida (jogador 1 sempre inicia)
    
    do{
        printf("Digite o número de jogadores (1 ou 2): ");
        scanf("%d", &numDeJogadores);
        getchar();
    }while(numDeJogadores != 1 && numDeJogadores != 2);
    jogo.tabuleiro = inicializaMatriz();
    if (numDeJogadores == 1){
        jogo.numJogadores = 1;
        printf("Digite o nome do jogador: ");
        fgets(jogo.nomeUm, 30, stdin);
        jogo.nomeUm[strlen(jogo.nomeUm)-1] = '\0';
        if (strcmp(jogo.nomeUm, "computador") == 0){ //o nome dos dois jogadores em uma mesma partida não pode ser igual
            printf("Nome inválido! Digite o nome do jogador: ");
            fgets(jogo.nomeDois, 30, stdin);
        }
        strcpy(jogo.nomeDois,"Computador");
        jogadorVsComputador(jogo);
    } else if (numDeJogadores == 2){
        jogo.numJogadores = 2;  
        printf("Digite o nome do jogador 1: ");
        fgets(jogo.nomeUm, 30, stdin);
        jogo.nomeUm[strlen(jogo.nomeUm)-1] = '\0';
        printf("Digite o nome do jogador 2: ");
        fgets(jogo.nomeDois, 30, stdin);
        jogo.nomeDois[strlen(jogo.nomeDois)-1] = '\0';
        if (strcmp(jogo.nomeUm, jogo.nomeDois) == 0){ //o nome dos dois jogadores em uma mesma partida não pode ser igual
            printf("Os nomes dos dois jogadores não podem ser iguais. Digite o nome do jogador 2:");
            fgets(jogo.nomeDois, 30, stdin);
            jogo.nomeDois[strlen(jogo.nomeDois)-1] = '\0';
        }
        jogadorVsJogador(jogo);
    }
}

//inicializa os valores da struct Jogador
void inicializaJogador(Jogador *jogador){ 
    jogador->derrotas = 0;
    jogador->empates = 0;
    jogador->vitorias = 0;
}

//Partida de jogo da velha com dois jogadores
void jogadorVsJogador(Jogo jogo){ 
    int rodada = 0;
    char simboloDaVez;
    int voltarParaOMenu = 0;
    Jogador jogadorUm, jogadorDois;
    do{
        rodada++;
        system("clear");
        exibirTabuleiro(jogo.tabuleiro);
        if (jogo.ultimoJogador == 2){ //se ultimo jogador for o 2, será vez do 1 jogar
            simboloDaVez = 'X';    
            voltarParaOMenu = jogada(jogo.nomeUm, &jogo, simboloDaVez); //jogada retorna 1 se o jogador escolheu voltar ou salvar o jogo, retorna 0 se o jogador continua a partida
            strcpy(jogadorUm.nome, jogo.nomeUm);
            jogo.ultimoJogador = 1;
        }else{ //se ultimo jogador for o 1, será vez do 2 jogar
            simboloDaVez = 'O';
            voltarParaOMenu = jogada(jogo.nomeDois, &jogo, simboloDaVez);  //jogada retorna 1 se o jogador escolheu voltar ou salvar o jogo, retorna 0 se o jogador continua a partida
            strcpy(jogadorDois.nome, jogo.nomeDois);
            jogo.ultimoJogador = 2;
        }
        if(voltarParaOMenu == 1){ //voltarParaMenu retorna 1 se o comando for "voltar" ou "salvar"
            return;
        }
    }while(ninguemGanhou(jogo.tabuleiro, simboloDaVez) && rodada < 9); //enquanto ninguel ganhou ou o limites de rodadas não for atingido
    system("clear");
    exibirTabuleiro(jogo.tabuleiro);
    inicializaJogador(&jogadorUm);
    inicializaJogador(&jogadorDois);
    // se o jogo acabou na ultima rodada e ninguem ganhou, então deu velha
    if(rodada == 9 && ninguemGanhou(jogo.tabuleiro, simboloDaVez)){
        printf("Deu velha!\nDigite qualquer tecla para continuar!\n");
        jogadorUm.empates = 1;
        jogadorDois.empates = 1;
    }
    //jogador 1 sempre inicia o jogo
    //rodada impar: sempre o jogador 1 joga
    //rodada par: sempre o jogador 2 joga
    else if (rodada % 2 != 0){ // se alguem ganhou em uma rodada impar, então jogador 1 ganhou
        printf("Parabéns %s, você ganhou!\nDigite qualquer tecla para continuar!\n", jogo.nomeUm);
        jogadorUm.vitorias = 1;
        jogadorDois.derrotas = 1;
    }else{ // se alguem ganhou em uma rodada par, então jogador 2 ganhou
        printf("Parabéns %s, você ganhou!\nDigite qualquer tecla para continuar!\n", jogo.nomeDois);
        jogadorDois.vitorias = 1;
        jogadorUm.derrotas = 1;
    }
    atualizarRanking(jogadorUm, jogadorDois); //ao final de toda partida, o ranking será atualizado
    getchar();
}

//Partida de jogo da velha entre jogador e computador
void jogadorVsComputador(Jogo jogo){ 
    Jogador jogadorUm, jogadorDois;
    int rodada = 0;
    char simboloDaVez;
    int voltarParaOMenu = 0;
    do{
        rodada++;
        system("clear");
        exibirTabuleiro(jogo.tabuleiro);
        if (jogo.ultimoJogador == 2){ //se ultimo jogador for o 2, será vez do 1 jogar
            simboloDaVez = 'X';
            voltarParaOMenu = jogada(jogo.nomeUm, &jogo, simboloDaVez); //jogada retorna 1 se o jogador escolheu voltar ou salvar o jogo, retorna 0 se o jogador continua a partida
            jogo.ultimoJogador = 1;
            strcpy(jogadorUm.nome, jogo.nomeUm);
        }else{ //se ultimo jogador for o 1, será vez do 2 jogar
            simboloDaVez = 'O';
            strcpy(jogadorDois.nome, jogo.nomeDois);
            if (jogadaDaMaquina(jogo.tabuleiro, 'O') == 0){ //jogadaDaMaquina verifica se há alguma posição que o computador consegue ganhar. Se há: marca jogada na posição e retorna 1, se não há: retorna 0
                if (jogadaDaMaquina(jogo.tabuleiro, 'X') == 0){ //jogadaDaMaquina verifica se há algum posição que o oponente consegue ganhar. Se há: marca jogada na posição para impedir vitória do adversário e retorna 1, se não há: retorna 0
                    sorteioDaJogadaDaMaquina(jogo.tabuleiro); // não havendo posição para computador ganhar ou para impedir vitória do adversário, computador sorteia uma posição para marcar
                }
            }
            jogo.ultimoJogador = 2;
        }
        if(voltarParaOMenu == 1){ //voltarParaMenu retorna 1 se o comando for "voltar" ou "salvar"
            return;
        }
    }while(ninguemGanhou(jogo.tabuleiro, simboloDaVez) && rodada < 9); //ninguemGanhou retorna 1 se o jogo ainda não acabou e retorna 0 se algum dos jogadores ganhou 
    system("clear");
    exibirTabuleiro(jogo.tabuleiro);
    inicializaJogador(&jogadorUm);
    inicializaJogador(&jogadorDois);
    // se o jogo acabou na ultima rodada, e ninguem ganhou então deu velha 
    if(rodada == 9 && ninguemGanhou(jogo.tabuleiro, simboloDaVez)){
        printf("Deu velha!\nDigite qualquer tecla para continuar!\n");
        jogadorUm.empates = 1;
        jogadorDois.empates = 1;
    }
     //jogador 1 sempre inicia o jogo
    //rodada impar: sempre o jogador 1 joga
    //rodada par: sempre o jogador 2 joga
    else if (rodada % 2 != 0){ // se alguem ganhou em uma rodada impar, então jogador 1 ganhou
        printf("Parabéns %s, você ganhou!\nDigite qualquer tecla para continuar!\n", jogo.nomeUm);
        jogadorUm.vitorias = 1;
        jogadorDois.derrotas = 1;
    }else{ // se alguem ganhou em uma rodada par, então computador ganhou
        printf("O computador ganhou!\nDigite qualquer tecla para continuar!\n");
        jogadorUm.derrotas = 1;
        jogadorDois.vitorias = 1;
    }
    atualizarRanking(jogadorUm, jogadorDois); //ao final de toda partida, o ranking será atualizado
    getchar();
}


//  função para realizar o comando do jogador, retona 1 se é para voltar para o menu principal (comando voltar e salvar), retorna 0 se é para continuar partida (comando marcar)
int jogada(char nome[], Jogo *jogo, char simbolo){ 
    char comando[30];
    int linhaJogada, colunaJogada, comandoValido;
    do{
        comandoValido = 1;
        printf("%s, digite o comando:", nome);
        fgets(comando, 30, stdin);
        comando[strlen(comando)-1] = '\0';
        char marcar[] = {"marcar"};
        char salvar[] = {"salvar"};
        char* ponteiroUm = strstr(comando, marcar); //se não há "marcar" no comando, strstr retorna NULL
        char* ponteiroDois = strstr(comando, salvar); //se não há "salvar" no comando, strstr retorna NULL
        if (ponteiroUm != NULL){ // se o comando for marcar, salva a linha e a coluna e verifica se é possivel fazer jogada, se for, a realiza
            int naoMarcou = 1;
            do{
                linhaJogada = (int) comando[7] - 49;
                colunaJogada = (int) comando[8] - 49;
                if(jogo->tabuleiro[linhaJogada][colunaJogada] != ' '){ //verifica se não há alguma marcação antecedente na posição selecionada
                    printf("Comando inválido %s, digite o comando novamente:", nome);          
                    fgets(comando, 30, stdin);
                    comando[strlen(comando)-1] = '\0';
                } else {
                    jogo->tabuleiro[linhaJogada][colunaJogada] = simbolo;
                    naoMarcou = 0;
                }
            } while(naoMarcou == 1); //o jogo não continua enquanto o jogador não fizer uma marcação valida
        }else if(strcmp(comando, "voltar") == 0){ // se o comando for voltar, salva os dados do jogo em um arquivo e retorna 1 
            strcpy(jogo->nomeDoArquivo, "ContinuaUltJogo.txt");
            salvarJogo(*jogo);
            return 1;
        }else if (ponteiroDois != NULL){ //se o comando for salvar, salva o nome do arquivo informado e cria o arquivo com o jogo
            int tam = strlen(comando);
            for  (int i = 7; i < tam; i++){
                jogo->nomeDoArquivo[i-7] = comando[i];
            }
            jogo->nomeDoArquivo[tam-7] = '\0';
            salvarJogo(*jogo);
            printf("Jogo salvo!");
            return 1;
        }else{ //se nenhum dos tres comandos foi digitado, informa que não é um comando valido 
            printf("\nComando inválido!!!\n\n");
            comandoValido = 0; //para repetir o loop enquanto não for digitado um comando valido
        }
    }while(comandoValido == 0);
    return 0;
}

//função para salvar o jogo quando o jogador optar por voltar ou salvar
void salvarJogo(Jogo jogo){ 
    FILE *file = fopen(jogo.nomeDoArquivo, "w");
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
    fprintf(file, "%d", jogo.ultimoJogador);
    fclose(file);
}

//funcão para abrir o jogo quando selecionado a opção 2 ou 3 do menu principal
void abrirJogo(char nomeDoArquivo[20]){ 
    FILE *file = fopen(nomeDoArquivo, "r");
    Jogo jogo;
    fscanf(file, "%d", &jogo.numJogadores); 
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

//função que retorna 0 se houve vitória na rodada e 1 se ninguem ganhou na rodada
int ninguemGanhou(char **matriz, char simbolo){ 
    int marcadoMesmaLinha = 1, marcadoMesmaColuna = 1;
    //verifica todas as celulas na diagonal principal então marcadas e se estão com o mesmo simbolo
    if(matriz[0][0] == simbolo && matriz[0][0] == matriz[1][1] && matriz[1][1] == matriz[2][2]){
        return 0;
    }
    //verifica todas as celulas na diagonal secundaria então marcadas e se estão com o mesmo simbolo
    if (matriz[2][0] == simbolo && matriz[0][2] == matriz[1][1] && matriz[1][1] == matriz[2][0]){
        return 0;
    }
    //verifica se há duas celulas vizinhas em uma mesma linha ou em uma mesma coluna marcadas com o mesmo simbolo, se houver soma 1(uma) unidade a variavel
    for(int i = 0; i < 3; i++){
        for (int j = 0; j < 2; j++){
            if (matriz[i][j] == matriz[i][j+1] && matriz[i][j] == simbolo){
                marcadoMesmaLinha++;
            }else if (matriz[j][i] == matriz [j+1][i] && matriz[j][i] == simbolo){
                marcadoMesmaColuna++;
            }
            if (marcadoMesmaLinha == 3 || marcadoMesmaColuna == 3){ //se somar 3 é porque há caso de vitória
                return 0;
            }
        }
        marcadoMesmaLinha = 1;
        marcadoMesmaColuna = 1;
    }
    return 1; //se não aconteceu nenhum dos casos, então ninguem ganhou
}

//função para o computador marcar em uma posição estrategica
int jogadaDaMaquina(char **matriz, char simb){ 
    //verifica se há posição com possibilidade de vitória na diagonal principal e na diagonal secundária
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
    //verifica se há posição com possibilidade nas linhas e nas colunas
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

//sorteia uma linha e uma coluna aleatória para o Computador jogador
void sorteioDaJogadaDaMaquina(char **matriz){ 
    int sorteioUm, sorteioDois;
    srand(time(NULL));
    do{
        sorteioUm = rand() % 3;
        sorteioDois = rand() % 3;
    }while(matriz[sorteioUm][sorteioDois] != ' ');
    matriz[sorteioUm][sorteioDois] = 'O';
}

//função para inserir ou atualizar os dados dos dois jogadores no final de uma partida
void atualizarRanking(Jogador jogadorUm, Jogador jogadorDois){ 
    int attJogUm = 0, attJogDois = 0, numeroDeJogadores;
    Jogador todosJogadores[10]; //array para armazenar os dados dos jogadores do arquivo com o ranking
    FILE *file = fopen("ranking.txt", "r");
    if (file == NULL){ //se não existir o arquivo com o ranking, cria o arquivo com os jogadores da rodada atual
            criarArquivoRanking(jogadorUm, jogadorDois);
            return;
    }
    fscanf(file, "%d", &numeroDeJogadores);
    if (numeroDeJogadores == 10){ //o jogo deve permitir armazenar os resultados de até 10 jogadores
        return;
    }else{ 
        //verificando se algum dos jogadores da partida atual já esta no arquivo, se sim soma as pontuações deste
        for (int i = 0; i < numeroDeJogadores; i++){
            todosJogadores[i] = lerJogadorNoArquivo(file); //lerJogadorNoArquivo retorna um Jogador que sera armazenado na posição i do array
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
        // se attJog = 1, então o jogador atual estava no arquivo, se attJog = 0, então o jogador atual não estava no arquivo
        if (attJogUm == 0){ //se jogador 1 não estava no ranking, então ele é adicionado a ultima posição do array
            adicionarJogador(numeroDeJogadores, todosJogadores, jogadorUm);
            numeroDeJogadores++;
        }
        if (attJogDois == 0){ //se jogador 2 não estava no ranking, então ele é adicionado a ultima posição do array
            adicionarJogador(numeroDeJogadores, todosJogadores, jogadorDois);
            numeroDeJogadores++;
        }
        fclose(file);
        ordenarTodosJogadores(todosJogadores, numeroDeJogadores); //coloca em ordem o array de acordo com os critérios
        file = fopen("ranking.txt", "w"); //após ordenar todos, reescreve o arquivo com os jogadores em ordem
        fprintf(file, "%d \n", numeroDeJogadores);
        for (int i = 0; i < numeroDeJogadores; i++){
            fprintf(file, "%s\n", todosJogadores[i].nome);
            fprintf(file, "%d %d %d\n", todosJogadores[i].vitorias, todosJogadores[i].derrotas, todosJogadores[i].empates);
        }
        fclose(file);
        }
}

//função que retorna Jogador do arquivo do ranking
Jogador lerJogadorNoArquivo(FILE *file){ 
    Jogador jogador;
    fscanf(file, "%s", jogador.nome);
    fscanf(file, "%d %d %d", &jogador.vitorias, &jogador.derrotas, &jogador.empates);
    return jogador;
}

//função que cria um arquivo para o ranking no caso da primeira partida do programa
void criarArquivoRanking(Jogador jogadorUm, Jogador jogadorDois){ 
    FILE *file = fopen("ranking.txt", "w");
    fprintf(file, "2\n");
    if (jogadorUm.vitorias > jogadorDois.vitorias){ //para inserir já em ordem
        inserirJogadorNoArquivo(file, jogadorUm);
        inserirJogadorNoArquivo(file, jogadorDois);
    }else{
        inserirJogadorNoArquivo(file, jogadorDois);
        inserirJogadorNoArquivo(file, jogadorUm);
    }
    fclose(file);
}

//função para inserir o jogador no arquivo, no caso da primeira partida do programa
void inserirJogadorNoArquivo(FILE *file, Jogador jogador){ 
    fprintf(file, "%s\n", jogador.nome);
    fprintf(file, "%d %d %d\n", jogador.vitorias, jogador.derrotas, jogador.empates);
}
//função para adicionar ao array um jogador que não esta no arquivo do ranking
void adicionarJogador(int numeroDeJogadores, Jogador todosJogadores[10], Jogador jogadorDaVez){
    strcpy(todosJogadores[numeroDeJogadores].nome, jogadorDaVez.nome);
    todosJogadores[numeroDeJogadores].vitorias = jogadorDaVez.vitorias;
    todosJogadores[numeroDeJogadores].derrotas = jogadorDaVez.derrotas;
    todosJogadores[numeroDeJogadores].empates = jogadorDaVez.empates;
}

//função para colocar os jogadores em ordem segundo os criterios
void ordenarTodosJogadores(Jogador todosJogadores[10], int numeroDeJogadores){ 
    int menorInd, aux;
    char auxChar[30];
    for (int i = 0; i < numeroDeJogadores-1; i++){
        menorInd = i;
        for (int j = i+1; j < numeroDeJogadores; j++){
            if(todosJogadores[j].vitorias > todosJogadores[menorInd].vitorias){
                menorInd = j;
            }else if(todosJogadores[j].vitorias == todosJogadores[menorInd].vitorias){
                if(todosJogadores[j].empates > todosJogadores[menorInd].empates){
                    menorInd = j;
                }else if((todosJogadores[j].empates == todosJogadores[menorInd].empates)){
                    if(todosJogadores[j].derrotas < todosJogadores[menorInd].derrotas){
                        menorInd = j;
                    }
                }
            }   
        }
            if (menorInd != i){
                strcpy(auxChar, todosJogadores[i].nome);
                strcpy(todosJogadores[i].nome, todosJogadores[menorInd].nome);
                strcpy(todosJogadores[menorInd].nome, auxChar);
                aux = todosJogadores[i].vitorias;
				todosJogadores[i].vitorias = todosJogadores[menorInd].vitorias;
				todosJogadores[menorInd].vitorias= aux;
                aux = todosJogadores[i].derrotas;
				todosJogadores[i].derrotas = todosJogadores[menorInd].derrotas;
				todosJogadores[menorInd].derrotas= aux;
                aux = todosJogadores[i].empates;
				todosJogadores[i].empates = todosJogadores[menorInd].empates;
				todosJogadores[menorInd].empates= aux;
            }
}
}

//função para imprimir o ranking
void exibirRanking(char arquivo[12]){ 
    FILE *file = fopen(arquivo, "r");
    Jogador jogador[10];
    if (file == NULL){
        printf("É necessário jogar pelo menos uma partida para exibir o ranking!");
        system("clear");
        return;
    }
    int numeroDeJogadores, tam;
    printf("   " TAB_VER BOLD("           Nome           ")TAB_VER BOLD(" Vitórias ")TAB_VER BOLD(" Empates ")TAB_VER" Derrotas\n");
    printf(TAB_HOR TAB_HOR TAB_HOR TAB_MJ TAB_HOR  TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR
    TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_MJ TAB_HOR 
    TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_MJ TAB_HOR TAB_HOR TAB_HOR
    TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_MJ TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR
    TAB_HOR TAB_HOR TAB_HOR"\n");
    fscanf(file, "%d\n", &numeroDeJogadores);
    for(int i = 0; i < numeroDeJogadores; i++){
        fgets(jogador[i].nome, 30, file);
        jogador[i].nome[strlen(jogador[i].nome) - 1] = '\0';
        fscanf(file, "%d %d %d\n", &jogador[i].vitorias, &jogador[i].empates, &jogador[i].derrotas);
        printf(BOLD(" %d ")TAB_VER, i+1);
        tam = strlen(jogador[i].nome);
        for (int i = 0; i < ((26 - tam) / 2) + (tam % 2); i++){
            printf(" ");
        }
        printf(BOLD("%s"), jogador[i].nome);
        for (int i = 0; i < (26 - tam) / 2; i++){
            printf(" ");
        }
        printf(TAB_VER);
        if (jogador[i].vitorias / 10 < 1){
            printf(" ");
        }
        printf(BOLD("    %d    ")TAB_VER, jogador[i].vitorias);
        if (jogador[i].derrotas / 10 < 1){
            printf(" ");
        }
        printf(BOLD("   %d    ")TAB_VER, jogador[i].derrotas);
        if (jogador[i].empates / 10 < 1){
            printf(" ");
        }
        printf(BOLD("   %d    \n"), jogador[i].empates);
    }
    getchar();
    printf("Pressione qualquer tecla para voltar!");
    getchar();
    system("clear");
}

void exibirTabuleiro(char **matriz){
    printf(TAB_TL TAB_HOR TAB_HOR TAB_HOR TAB_TJ TAB_HOR TAB_HOR TAB_HOR TAB_TJ TAB_HOR TAB_HOR TAB_HOR TAB_TR "\n");
    printf(TAB_VER);
    if (matriz[0][0] == 'X'){
        printf(BOLD(RED(" %c ")), matriz[0][0]);
    }else if (matriz[0][0] == 'O'){
        printf(BOLD(CYAN(" %c ")), matriz[0][0]);
    }else{
        printf(BOLD(" %c "), matriz[0][0]);
    }
    printf(TAB_VER);
    if (matriz[0][1] == 'X'){
        printf(BOLD(RED(" %c ")), matriz[0][1]);
    }else if (matriz[0][1] == 'O'){
        printf(BOLD(CYAN(" %c ")), matriz[0][1]);
    }else{
        printf(BOLD(" %c "), matriz[0][1]);
    }
    printf(TAB_VER);
    if (matriz[0][2] == 'X'){
        printf(BOLD(RED(" %c ")), matriz[0][2]);
    }else if (matriz[0][2] == 'O'){
        printf(BOLD(CYAN(" %c ")), matriz[0][2]);
    }else{
        printf(BOLD(" %c "), matriz[0][2]);
    }
    printf(TAB_VER"\n");
    printf(TAB_ML TAB_HOR TAB_HOR TAB_HOR TAB_MJ TAB_HOR  TAB_HOR TAB_HOR TAB_MJ TAB_HOR TAB_HOR TAB_HOR TAB_MR "\n");
    printf(TAB_VER);
    if (matriz[1][0] == 'X'){
        printf(BOLD(RED(" %c ")), matriz[1][0]);
    }else if (matriz[1][0] == 'O'){
        printf(BOLD(CYAN(" %c ")), matriz[1][0]);
    }else{
        printf(BOLD(" %c "), matriz[1][0]);
    }
    printf(TAB_VER);
    if (matriz[1][1] == 'X'){
        printf(BOLD(RED(" %c ")), matriz[1][1]);
    }else if (matriz[1][1] == 'O'){
        printf(BOLD(CYAN(" %c ")), matriz[1][1]);
    }else{
        printf(BOLD(" %c "), matriz[1][1]);
    } 
    printf(TAB_VER);
    if (matriz[1][2] == 'X'){
        printf(BOLD(RED(" %c ")), matriz[1][2]);
    }else if (matriz[1][2] == 'O'){
        printf(BOLD(CYAN(" %c ")), matriz[1][2]);
    }else{
        printf(BOLD(" %c "), matriz[1][2]);
    }
    printf(TAB_VER"\n");
    printf(TAB_ML TAB_HOR TAB_HOR TAB_HOR TAB_MJ TAB_HOR TAB_HOR TAB_HOR TAB_MJ TAB_HOR TAB_HOR TAB_HOR TAB_MR "\n");
    printf(TAB_VER);
    if (matriz[2][0] == 'X'){
        printf(BOLD(RED(" %c ")), matriz[2][0]);
    }else if (matriz[2][0] == 'O'){
        printf(BOLD(CYAN(" %c ")), matriz[2][0]);
    }else{
        printf(BOLD(" %c "), matriz[2][0]);
    }
    printf(TAB_VER);
    if (matriz[2][1] == 'X'){
        printf(BOLD(RED(" %c ")), matriz[2][1]);
    }else if (matriz[2][1] == 'O'){
        printf(BOLD(CYAN(" %c ")), matriz[2][1]);
    }else{
        printf(BOLD(" %c "), matriz[2][1]);
    }
    printf(TAB_VER); 
    if (matriz[2][2] == 'X'){
        printf(BOLD(RED(" %c ")), matriz[2][2]);
    }else if (matriz[2][2] == 'O'){
        printf(BOLD(CYAN(" %c ")), matriz[2][2]);
    }else{
        printf(BOLD(" %c "), matriz[2][2]);
    }
    printf(TAB_VER "\n");
    printf(TAB_BL TAB_HOR TAB_HOR TAB_HOR TAB_BJ TAB_HOR TAB_HOR TAB_HOR TAB_BJ TAB_HOR TAB_HOR TAB_HOR TAB_BR "\n");
    printf("\n");
}                                                                                                                                 