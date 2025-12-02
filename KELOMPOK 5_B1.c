#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define PLAYER 'X'
#define AI 'O'

void displayBoard(char board[3][3]);
char checkWin(char board[3][3]);
int isDraw(char board[3][3]);
void humanMove(char board[3][3], char symbol);
void aiMove(char board[3][3]);
void saveScoreSingle(int playerWins, int aiWins);
void loadScoreSingle(int *playerWins, int *aiWins);
void saveScoreMulti(int xWins, int oWins);
void loadScoreMulti(int *xWins, int *oWins);
void clearScreen();
void playGameSingle();
void playGameMulti();

void displayBoard(char board[3][3]) {
    printf("\n");
    for (int i = 0; i < 3; i++) {
        printf(" %c | %c | %c ", board[i][0], board[i][1], board[i][2]);
        if (i < 2) printf("\n---|---|---\n");
    }
    printf("\n");
}

char checkWin(char board[3][3]) {
    for (int i = 0; i < 3; i++) {
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2])
            return board[i][0];
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i])
            return board[0][i];
    }
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2])
        return board[0][0];
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0])
        return board[0][2];
    return 0;
}

int isDraw(char board[3][3]) {
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (board[i][j] != 'X' && board[i][j] != 'O')
                return 0;
    return 1;
}

void humanMove(char board[3][3], char symbol) {
    int choice;
    int row, col;

    while (1) {
        printf("Pemain %c, pilih posisi (1-9): ", symbol);
        if (scanf("%d", &choice) != 1) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {}
            printf("Input tidak valid! Masukkan angka 1-9.\n");
            continue;
        }

        if (choice < 1 || choice > 9) {
            printf("Pilihan tidak valid! Coba lagi.\n");
            continue;
        }

        row = (choice - 1) / 3;
        col = (choice - 1) % 3;

        if (board[row][col] != 'X' && board[row][col] != 'O') {
            board[row][col] = symbol;
            break;
        } else {
            printf("Posisi sudah terisi! Coba lagi.\n");
        }
    }
}

void aiMove(char board[3][3]) {
    int row, col;
    printf("Giliran AI...\n");

    do {
        int move = rand() % 9;
        row = move / 3;
        col = move % 3;
    } while (board[row][col] == 'X' || board[row][col] == 'O');

    board[row][col] = AI;
}

void saveScoreSingle(int playerWins, int aiWins) {
    FILE *f = fopen("score_single.txt", "w");
    if (!f) {
        printf("Gagal menyimpan skor single!\n");
        return;
    }
    fprintf(f, "%d %d", playerWins, aiWins);
    fclose(f);
}

void loadScoreSingle(int *playerWins, int *aiWins) {
    FILE *f = fopen("score_single.txt", "r");
    if (!f) {
        *playerWins = 0;
        *aiWins = 0;
        return;
    }
    fscanf(f, "%d %d", playerWins, aiWins);
    fclose(f);
}

void saveScoreMulti(int xWins, int oWins) {
    FILE *f = fopen("score_multi.txt", "w");
    if (!f) {
        printf("Gagal menyimpan skor multi!\n");
        return;
    }
    fprintf(f, "%d %d", xWins, oWins);
    fclose(f);
}

void loadScoreMulti(int *xWins, int *oWins) {
    FILE *f = fopen("score_multi.txt", "r");
    if (!f) {
        *xWins = 0;
        *oWins = 0;
        return;
    }
    fscanf(f, "%d %d", xWins, oWins);
    fclose(f);
}

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void playGameSingle() {
    char board[3][3] = {
        {'1','2','3'},
        {'4','5','6'},
        {'7','8','9'}
    };

    int playerWins, aiWins;
    loadScoreSingle(&playerWins, &aiWins);

    while (1) {
        clearScreen();
        printf("=== TIC TAC TOE (Single vs AI) ===\n");
        printf("Skor: Player(X) = %d | AI(O) = %d\n", playerWins, aiWins);
        displayBoard(board);

        humanMove(board, PLAYER);

        char winner = checkWin(board);
        if (winner == PLAYER) {
            clearScreen();
            displayBoard(board);
            printf("Kamu menang!\n");
            playerWins++;
            saveScoreSingle(playerWins, aiWins);
            break;
        }

        if (isDraw(board)) {
            clearScreen();
            displayBoard(board);
            printf("Seri!\n");
            break;
        }

        aiMove(board);

        winner = checkWin(board);
        if (winner == AI) {
            clearScreen();
            displayBoard(board);
            printf("AI menang!\n");
            aiWins++;
            saveScoreSingle(playerWins, aiWins);
            break;
        }

        if (isDraw(board)) {
            clearScreen();
            displayBoard(board);
            printf("Seri!\n");
            break;
        }
    }
}

void playGameMulti() {
    char board[3][3] = {
        {'1','2','3'},
        {'4','5','6'},
        {'7','8','9'}
    };

    int xWins, oWins;
    loadScoreMulti(&xWins, &oWins);

    char current = 'X';

    while (1) {
        clearScreen();
        printf("=== TIC TAC TOE (Multiplayer) ===\n");
        printf("Skor: X = %d | O = %d\n", xWins, oWins);
        displayBoard(board);

        humanMove(board, current);

        char winner = checkWin(board);
        if (winner == 'X' || winner == 'O') {
            clearScreen();
            displayBoard(board);
            printf("Pemain %c menang!\n", winner);
            if (winner == 'X') xWins++; else oWins++;
            saveScoreMulti(xWins, oWins);
            break;
        }

        if (isDraw(board)) {
            clearScreen();
            displayBoard(board);
            printf("Seri!\n");
            break;
        }

        current = (current == 'X') ? 'O' : 'X';
    }
}

int main() {
    srand((unsigned)time(0));
    int choice;
    int playerWins, aiWins;
    int xWins, oWins;
    loadScoreSingle(&playerWins, &aiWins);
    loadScoreMulti(&xWins, &oWins);

    while (1) {
        printf("\n===== MENU UTAMA =====\n");
        printf("1. Mainkan (Single vs AI)\n");
        printf("2. Mainkan (Multiplayer)\n");
        printf("3. Lihat Skor (Single)\n");
        printf("4. Lihat Skor (Multiplayer)\n");
        printf("5. Reset Skor (Single)\n");
        printf("6. Reset Skor (Multiplayer)\n");
        printf("7. Keluar\n");
        printf("Pilih: ");

        if (scanf("%d", &choice) != 1) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {}
            printf("Input tidak valid! Masukkan angka.\n");
            continue;
        }

        if (choice == 1) {
            playGameSingle();
        } else if (choice == 2) {
            playGameMulti();
        } else if (choice == 3) {
            loadScoreSingle(&playerWins, &aiWins);
            printf("\nSkor Single Saat Ini:\n");
            printf("Player (X): %d\n", playerWins);
            printf("AI (O): %d\n", aiWins);
        } else if (choice == 4) {
            loadScoreMulti(&xWins, &oWins);
            printf("\nSkor Multiplayer Saat Ini:\n");
            printf("X: %d\n", xWins);
            printf("O: %d\n", oWins);
        } else if (choice == 5) {
            playerWins = 0;
            aiWins = 0;
            saveScoreSingle(playerWins, aiWins);
            printf("Skor Single berhasil direset.\n");
        } else if (choice == 6) {
            xWins = 0;
            oWins = 0;
            saveScoreMulti(xWins, oWins);
            printf("Skor Multiplayer berhasil direset.\n");
        } else if (choice == 7) {
            printf("Terima kasih telah bermain!\n");
            break;
        } else {
            printf("Pilihan tidak valid!\n");
        }
    }

    return 0;
}
