#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#ifdef _WIN32
    #define CLEAR "cls"
#else
    #define CLEAR "clear"
#endif

#define MAX 100

typedef struct {
    char nama[50];
    int level;
    int skor;
} Score;

Score papanSkor[MAX];
int jumlahSkor = 0;

void clearInput() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void simpanSkor(char *nama, int level, int skor) {
    FILE *file = fopen("papan_score.txt", "a");
    if (!file) return;
    fprintf(file, "%s %d %d\n", nama, level, skor);
    fclose(file);
}

void loadSkor() {
    FILE *file = fopen("papan_score.txt", "r");
    if (!file) return;

    jumlahSkor = 0;
    while (fscanf(file, "%s %d %d",
                  papanSkor[jumlahSkor].nama,
                  &papanSkor[jumlahSkor].level,
                  &papanSkor[jumlahSkor].skor) != EOF)
    {
        jumlahSkor++;
        if (jumlahSkor >= MAX) break;
    }
    fclose(file);
}

void tampilkanSkor() {
    loadSkor();

    for (int i = 0; i < jumlahSkor - 1; i++) {
        for (int j = i + 1; j < jumlahSkor; j++) {
            if (papanSkor[i].level > papanSkor[j].level ||
               (papanSkor[i].level == papanSkor[j].level &&
                papanSkor[i].skor > papanSkor[j].skor))
            {
                Score tmp = papanSkor[i];
                papanSkor[i] = papanSkor[j];
                papanSkor[j] = tmp;
            }
        }
    }

    system(CLEAR);
    printf("=========== Papan Skor ===========\n");
    printf("%-15s | %-5s | %-5s\n", "Nama", "Lvl", "Skor");
    printf("----------------------------------\n");

    for (int i = 0; i < jumlahSkor; i++) {
        printf("%-15s | %-5d | %-5d\n",
               papanSkor[i].nama,
               papanSkor[i].level,
               papanSkor[i].skor);
    }

    printf("\nTekan Enter untuk kembali...");
    clearInput();
    getchar();
}

void tampilkanBoard(char *cards, int *open, int size) {
    system(CLEAR);
    printf("=========== MEMORY GAME ===========\n\n");

    int side = sqrt(size);

    for (int i = 0; i < size; i++) {
        if (open[i])
            printf(" [ %c] ", cards[i]);
        else
            printf(" [%2d] ", i + 1);

        if ((i + 1) % side == 0)
            printf("\n");
    }
    printf("\n");
}

void shuffle(char *cards, int size) {
    for (int i = 0; i < size; i++) {
        int r = rand() % size;
        char t = cards[i];
        cards[i] = cards[r];
        cards[r] = t;
    }
}

void resetSkor() {
    FILE *file = fopen("papan_score.txt", "w");
    if (!file) return;
    fclose(file);

    printf("\n===== Papan skor berhasil dibersihkan! =====\n");
    printf("Tekan Enter untuk kembali...");
    clearInput();
    getchar();
}

void playGame() {
    char nama[50];
    printf("Masukkan nama anda: ");
    scanf("%49s", nama);
    clearInput();

    int level, size;
    printf("\n====== PILIH LEVEL ======\n");
    printf("1. Very Easy (2x2)\n");
    printf("2. Easy      (4x4)\n");
    printf("3. Medium    (6x6)\n");
    printf("4. Hard      (8x8)\n");
    printf("Pilih level: ");
    scanf("%d", &level);
    clearInput();

    if (level == 1) size = 4;
    else if (level == 2) size = 16;
    else if (level == 3) size = 36;
    else if (level == 4) size = 64;
    else {
        printf("Level tidak valid!\n");
        return;
    }

    char *cards = malloc(size);
    int *open = calloc(size, sizeof(int));
    int pairs = size / 2;

    for (int i = 0; i < pairs; i++) {
        char sym = (i < 26) ? 'A' + i : 'a' + (i - 26);
        cards[i * 2] = sym;
        cards[i * 2 + 1] = sym;
    }

    srand(time(NULL));
    shuffle(cards, size);

    int langkah = 0;

    while (1) {
        tampilkanBoard(cards, open, size);

        char in1[10], in2[10];
        int p1, p2;

        // ============================
        // INPUT PERTAMA
        // ============================
        printf("Pilih posisi pertama (1-%d) atau: ", size);
        scanf("%9s", in1);
        clearInput();

        if (in1[0] == 'q' && in1[1] == '\0') {
            free(cards);
            free(open);
            system(CLEAR);
            printf("===== GAME DIBATALKAN =====\n");
            return;
        }

        p1 = atoi(in1) - 1;
        if (p1 < 0 || p1 >= size || open[p1]) {
            printf("Posisi pertama tidak valid!\n\n");
            continue;
        }

        // ============================
        // INPUT KEDUA
        // ============================
        printf("Pilih posisi kedua (1-%d): ", size);
        scanf("%9s", in2);
        clearInput();

        if (in2[0] == 'q' && in2[1] == '\0') {
            free(cards);
            free(open);
            system(CLEAR);
            printf("===== GAME DIBATALKAN =====\n");
            return;
        }

        p2 = atoi(in2) - 1;
        if (p2 < 0 || p2 >= size || open[p2] || p1 == p2) {
            printf("Posisi kedua tidak valid!\n\n");
            continue;
        }

        // ============================
        // PROSES
        // ============================
        langkah++;
        open[p1] = open[p2] = 1;

        tampilkanBoard(cards, open, size);

        if (cards[p1] == cards[p2]) {
            printf("Cocok!\n(Enter untuk lanjut)");
        } else {
            printf("Tidak cocok!\n(Enter untuk lanjut)");
            open[p1] = open[p2] = 0;
        }

        getchar();

        // cek selesai
        int done = 1;
        for (int i = 0; i < size; i++) {
            if (!open[i]) {
                done = 0;
                break;
            }
        }
        if (done) break;
    }

    // ============================
    // GAME SELESAI
    // ============================
    system(CLEAR);
    int skor = (size * 1000) - langkah * 10;

    printf("===== GAME SELESAI =====\n");
    printf("Total langkah : %d\n", langkah);
    printf("Skor akhir    : %d\n", skor);
    printf("=========================\n");

    simpanSkor(nama, level, skor);
    tampilkanSkor();

    free(cards);
    free(open);
}

void penjelasanGame() {
    system(CLEAR);
    printf("=================================== PENJELASAN GAME ===================================\n\n");
    printf("Memory Game adalah permainan mencocokkan pasangan kartu.\n");
    printf("Tujuan: Temukan semua pasangan kartu dengan langkah sesedikit mungkin.\n\n");

    printf("Aturan:\n");
    printf("1. Semua kartu tertutup di awal.\n");
    printf("2. Pilih dua posisi.\n");
    printf("3. Jika cocok, tetap terbuka.\n");
    printf("4. Jika tidak, menutup kembali.\n\n");

    printf("Kontrol:\n");
    printf("- Pilih kartu dengan memasukkan nomor.\n");
    printf("- Masukkan 'q' jika ingin keluar.\n\n");

    printf("Skor berdasar jumlah langkah.\n\n");

    printf("Tekan Enter untuk kembali...");
    clearInput();
    getchar();
}

int main() {
    penjelasanGame();
    srand(time(NULL));

    int pilihan;

    while (1) {
        printf("\n=== MEMORY GAME ===\n");
        printf("1. Petunjuk game\n");
        printf("2. Mulai game\n");
        printf("3. Lihat Papan Skor\n");
        printf("4. Reset papan skor\n");
        printf("5. Keluar\n");
        printf("Pilih: ");
        scanf("%d", &pilihan);
        clearInput();

        switch (pilihan) {
            case 1: penjelasanGame(); break;
            case 2: playGame(); break;
            case 3: tampilkanSkor(); break;
            case 4: resetSkor(); break;
            case 5: printf("Terima kasih!\n"); return 0;
            default: printf("Pilihan tidak valid!\n");
        }
    }
}
