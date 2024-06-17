#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h> 

// Função para exibir o menu e retornar a escolha do usuário
int exibirMenu() {
    int escolha;
    printf("1. Iniciar Jogo\n"); // Opção para iniciar o jogo.
    printf("2. Reiniciar Jogo\n"); // Opção para reiniciar o jogo.
    printf("3. Sair\n"); // Opção para sair do jogo.
    printf("Escolha uma opcao: ");
    scanf("%d", &escolha);
    return escolha;
}

// Função para imprimir o tabuleiro do jogo
void imprimirTabuleiro(char* tabuleiro) {
    printf(" %c | %c | %c\n", tabuleiro[0], tabuleiro[1], tabuleiro[2]); // Imprime a primeira linha do tabuleiro.
    printf("---|---|---\n");
    printf(" %c | %c | %c\n", tabuleiro[3], tabuleiro[4], tabuleiro[5]); // Imprime a segunda linha do tabuleiro.
    printf("---|---|---\n");
    printf(" %c | %c | %c\n", tabuleiro[6], tabuleiro[7], tabuleiro[8]); // Imprime a terceira linha do tabuleiro.
}

// Função para verificar se um jogador venceu
int verificarVitoria(char* tabuleiro, char jogador) {
    const int padroesVitoria[8][3] = {
        {0, 1, 2}, {3, 4, 5}, {6, 7, 8}, // Linhas
        {0, 3, 6}, {1, 4, 7}, {2, 5, 8}, // Colunas
        {0, 4, 8}, {2, 4, 6}             // Diagonais
    };

    for (int i = 0; i < 8; i++) { // Percorre todos os padrões de vitória.
        if (tabuleiro[padroesVitoria[i][0]] == jogador &&
            tabuleiro[padroesVitoria[i][1]] == jogador &&
            tabuleiro[padroesVitoria[i][2]] == jogador) {
            return 1; // Retorna 1 se algum jogador vencer.
        }
    }
    return 0; // Retorna 0 se nenhum jogador vencer.
}

// Função para verificar se o jogo terminou em empate
int verificarEmpate(char* tabuleiro) {
    for (int i = 0; i < 9; i++) { // Percorre todas as posições do tabuleiro.
        if (tabuleiro[i] == ' ') {
            return 0; // Retorna 0 se ainda houver espaços vazios no tabuleiro.
        }
    }
    return 1; // Retorna 1 se o tabuleiro estiver cheio, indicando um empate.
}

// Função para obter uma jogada aleatória do computador
int obterJogadaAleatoria(char* tabuleiro) {
    int jogada;
    do {
        jogada = rand() % 9; // Gera uma jogada aleatória de 0 a 8.
    } while (tabuleiro[jogada] != ' '); // Repete enquanto a posição gerada já estiver ocupada.
    return jogada;
}

// Função para obter a melhor jogada usando o algoritmo Minimax
int minimax(char* tabuleiro, char jogador, int profundidade, int maximizando) {
    char oponente = (jogador == 'X') ? 'O' : 'X'; // Determina o oponente com base no jogador atual.

    if (verificarVitoria(tabuleiro, jogador)) return 10 - profundidade; // Retorna uma pontuação positiva se o jogador atual ganhar.
    if (verificarVitoria(tabuleiro, oponente)) return profundidade - 10; // Retorna uma pontuação negativa se o oponente ganhar.
    if (verificarEmpate(tabuleiro)) return 0; // Retorna 0 se houver empate.

    if (maximizando) {
        int melhorPontuacao = -1000;
        for (int i = 0; i < 9; i++) {
            if (tabuleiro[i] == ' ') {
                tabuleiro[i] = jogador;
                int pontuacao = minimax(tabuleiro, jogador, profundidade + 1, 0);
                tabuleiro[i] = ' ';
                if (pontuacao > melhorPontuacao) melhorPontuacao = pontuacao; // Maximiza a pontuação.
            }
        }
        return melhorPontuacao;
    } else {
        int melhorPontuacao = 1000;
        for (int i = 0; i < 9; i++) {
            if (tabuleiro[i] == ' ') {
                tabuleiro[i] = oponente;
                int pontuacao = minimax(tabuleiro, jogador, profundidade + 1, 1);
                tabuleiro[i] = ' ';
                if (pontuacao < melhorPontuacao) melhorPontuacao = pontuacao; // Minimiza a pontuação.
            }
        }
        return melhorPontuacao;
    }
}

// Função para obter a melhor jogada do computador (níveis difícil e impossível)
int obterMelhorJogada(char* tabuleiro, char jogador) {
    int melhorJogada = -1;
    int melhorPontuacao = -1000;
    for (int i = 0; i < 9; i++) {
        if (tabuleiro[i] == ' ') {
            tabuleiro[i] = jogador;
            int pontuacao = minimax(tabuleiro, jogador, 0, 0);
            tabuleiro[i] = ' ';
            if (pontuacao > melhorPontuacao) {
                melhorPontuacao = pontuacao;
                melhorJogada = i;
            }
        }
    }
    return melhorJogada;
}

// Função para iniciar e gerenciar o jogo
void iniciarJogo(int dificuldade) {
    char* tabuleiro = (char*)malloc(9 * sizeof(char)); // Aloca dinamicamente memória para o tabuleiro.
    for (int i = 0; i < 9; i++) {
        tabuleiro[i] = ' '; // Inicializa o tabuleiro com espaços vazios.
    }
    char jogadorAtual = 'X'; // Começa com o jogador X.
        int emExecucao = 1;

    while (emExecucao) {
        system("cls"); // Limpa a tela do console.
        imprimirTabuleiro(tabuleiro); // Imprime o tabuleiro atual.
        int jogada;
        if (jogadorAtual == 'X') {
            printf("Jogador %c, insira sua jogada (1-9): ", jogadorAtual);
            scanf("%d", &jogada);
            jogada--; // Ajuste para o índice zero.
        } else {
            if (dificuldade == 1) {
                jogada = obterJogadaAleatoria(tabuleiro); // Para dificuldade fácil, obtém uma jogada aleatória.
            } else {
                jogada = obterMelhorJogada(tabuleiro, jogadorAtual); // Para dificuldade média ou difícil, usa o algoritmo Minimax.
            }
        }

        if (jogada >= 0 && jogada < 9 && tabuleiro[jogada] == ' ') { // Verifica se a jogada é válida.
            tabuleiro[jogada] = jogadorAtual; // Registra a jogada no tabuleiro.
            if (verificarVitoria(tabuleiro, jogadorAtual)) { // Verifica se o jogador atual venceu.
                system("cls");
                imprimirTabuleiro(tabuleiro);
                printf("Jogador %c venceu!\n", jogadorAtual);
                emExecucao = 0; // Termina o jogo.
            } else if (verificarEmpate(tabuleiro)) { // Verifica se houve empate.
                system("cls");
                imprimirTabuleiro(tabuleiro);
                printf("O jogo terminou em empate!\n");
                emExecucao = 0; // Termina o jogo.
            } else {
                jogadorAtual = (jogadorAtual == 'X') ? 'O' : 'X'; // Alterna para o próximo jogador.
            }
        } else {
            if (jogadorAtual == 'X') {
                printf("Jogada invalida. Tente novamente.\n");
                _getch(); // Aguarda uma tecla ser pressionada para continuar.
            }
        }
    }

    free(tabuleiro); // Libera a memória alocada para o tabuleiro.
}

// Função para escolher o nível de dificuldade
int escolherDificuldade() {
    int dificuldade;
    printf("Escolha o nivel de dificuldade:\n");
    printf("1. Facil\n"); // Nível fácil: jogadas aleatórias do computador.
    printf("2. Dificil\n"); // Nível médio: usa o algoritmo Minimax para o computador jogar.
    printf("3. Impossivel\n"); // Nível difícil: usa o algoritmo Minimax para o computador e prioriza sempre a melhor jogada.
    printf("Escolha uma opcao: ");
    scanf("%d", &dificuldade);
    return dificuldade;
}

// Função principal que gerencia o menu e a execução do jogo
int main() {
    srand(time(0)); // Inicializa a semente para geração de números aleatórios.
    int emExecucao = 1;
    int dificuldade = 1;
    while (emExecucao) {
        switch (exibirMenu()) { // Exibe o menu e obtém a escolha do usuário.
            case 1:
                dificuldade = escolherDificuldade(); // Escolhe o nível de dificuldade.
                iniciarJogo(dificuldade); // Inicia o jogo com a dificuldade selecionada.
                break;
            case 2:
                printf("Reiniciando Jogo...\n");
                iniciarJogo(dificuldade); // Reinicia o jogo com a mesma dificuldade.
                break;
            case 3:
                emExecucao = 0; // Sai do jogo.
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n"); // Trata opções inválidas.
        }
    }
    return 0;
}

