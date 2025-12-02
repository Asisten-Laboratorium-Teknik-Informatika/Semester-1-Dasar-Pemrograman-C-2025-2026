#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#ifdef _WIN32
#include <windows.h>
#define CLEAR "cls"
#else
#include <unistd.h>
#define CLEAR "clear"
#endif

// Fungsi delay animasi sederhana
void delay(int sec) {
#ifdef _WIN32
    Sleep(sec * 1000);
#else
    sleep(sec);
#endif
}

void tampilJudul() {
    printf("=============================================\n");
    printf("    GAME SIMULASI SERVICE HP + KASIR    \n");
    printf("=============================================\n");
}

void loading() {
    printf("\nLoading");
    for (int i = 0; i < 5; i++) {
        printf(".");
        fflush(stdout);
        delay(1);
    }
    printf("\n\n");
}

// Fungsi menu utama
int menuUtama() {
    int pilih;
    tampilJudul();
    printf("1. Mulai Game\n");
    printf("2. Cara Bermain\n");
    printf("3. Keluar\n");
    printf("---------------------------------------------\n");
    printf("Pilih menu: ");
    scanf("%d", &pilih);
    return pilih;
}

void caraBermain() {
    system(CLEAR);
    tampilJudul();
    printf(" CARA BERMAIN:\n");
    printf("- Kamu berperan sebagai teknisi sekaligus kasir servis HP.\n");
    printf("- Perbaiki HP pelanggan sesuai jenis kerusakan.\n");
    printf("- Setelah diperbaiki, hitung tarif servis dengan benar.\n");
    printf("- Jika salah hitung, uang pelanggan akan berkurang!\n");
    printf("- Semakin cepat & akurat, makin tinggi pendapatanmu!\n\n");
    printf("Tekan ENTER untuk kembali ke menu...");
    getchar(); getchar();
}

void mainGame(int level) {
    int waktu, uang = 0, totalServis = 0;
    int hpRusak, pilihan, tarif, bayar, kembalian;
    char nama[50];
    srand(time(0));

    system(CLEAR);
    tampilJudul();
    printf("Masukkan nama teknisi kamu: ");
    scanf("%s", nama);

    // Set waktu berdasarkan level
    if (level == 1) waktu = 60;
    else if (level == 2) waktu = 45;
    else waktu = 30;

    system(CLEAR);
    tampilJudul();
    printf("Selamat datang, %s!\n", nama);
    printf("Level kesulitan: %s\n", level == 1 ? "Easy" : (level == 2 ? "Medium" : "Hard"));
    loading();

    while (waktu > 0) {
        system(CLEAR);
        tampilJudul();

        printf(" Teknisi: %s\n", nama);
        printf("  Waktu tersisa: %d detik\n", waktu);
        printf(" Total uang: Rp%d\n", uang);
        printf("---------------------------------------------\n");

        hpRusak = rand() % 4 + 1;

        // Menentukan masalah HP
        if (hpRusak == 1) printf("Masalah: Layar HP retak dan mati total.\n");
        else if (hpRusak == 2) printf("Masalah: Baterai cepat habis.\n");
        else if (hpRusak == 3) printf("Masalah: Speaker tidak berbunyi.\n");
        else printf("Masalah: HP sering restart sendiri.\n");

        // Menentukan tarif berdasarkan jenis kerusakan
        if (hpRusak == 1) tarif = 250000;
        else if (hpRusak == 2) tarif = 150000;
        else if (hpRusak == 3) tarif = 120000;
        else tarif = 180000;

        printf("\nPilih tindakan perbaikan:\n");
        printf("1. Ganti layar\n");
        printf("2. Ganti baterai\n");
        printf("3. Ganti speaker\n");
        printf("4. Cek software & reset HP\n");
        printf("Pilihan kamu: ");
        scanf("%d", &pilihan);

        printf("\nMemperbaiki HP");
        for (int i = 0; i < 3; i++) { printf("."); fflush(stdout); delay(1); }
        printf("\n");

        if (pilihan == hpRusak) {
            printf(" HP berhasil diperbaiki!\n");
            waktu -= 5;
            totalServis++;

            printf("\n Tarif servis untuk pelanggan ini: Rp%d\n", tarif);
            printf("Masukkan jumlah uang yang dibayar pelanggan: Rp");
            scanf("%d", &bayar);

            if (bayar < tarif) {
                printf(" Uang pelanggan kurang! Kamu rugi Rp%d.\n", tarif - bayar);
                uang -= (tarif - bayar);
            } else {
                kembalian = bayar - tarif;
                printf(" Pembayaran berhasil! Kembalian: Rp%d\n", kembalian);
                uang += tarif;
                // Bonus kecil untuk kecepatan kerja
                if (rand() % 3 == 0) {
                    int bonus = 20000;
                    uang += bonus;
                    printf(" Pelanggan memberi tip Rp%d karena servis cepat!\n", bonus);
                }
            }
        } else {
            printf(" Salah perbaikan! Pelanggan kecewa dan tidak mau bayar!\n");
            uang -= 50000;
            waktu -= 10;
        }

        delay(2);
        if (uang < 0) uang = 0;
        if (waktu <= 0) break;
    }

    // Akhir permainan
    system(CLEAR);
    tampilJudul();
    printf("Waktu habis!\n\n");
    printf(" Nama teknisi: %s\n", nama);
    printf(" Total servis selesai: %d pelanggan\n", totalServis);
    printf(" Total pendapatan: Rp%d\n", uang);
    printf("---------------------------------------------\n");

    if (uang >= 1000000)
        printf(" Hebat! Kamu jadi teknisi terbaik minggu ini!\n");
    else if (uang >= 500000)
        printf(" Kamu teknisi handal, pelanggan puas!\n");
    else
        printf(" Kamu perlu latihan lagi agar servis lebih cepat dan akurat.\n");

    printf("\nTekan ENTER untuk kembali ke menu...");
    getchar(); getchar();
}

int main() {
    int pilihan, level;

    do {
        system(CLEAR);
        pilihan = menuUtama();

        switch (pilihan) {
            case 1:
                system(CLEAR);
                tampilJudul();
                printf("Pilih level kesulitan:\n");
                printf("1. Easy\n");
                printf("2. Medium\n");
                printf("3. Hard\n");
                printf("Pilihan kamu: ");
                scanf("%d", &level);
                mainGame(level);
                break;
            case 2:
                caraBermain();
                break;
            case 3:
                system(CLEAR);
                printf("Terima kasih sudah bermain! 👋\n");
                break;
            default:
                printf("Pilihan tidak valid!\n");
        }
    } while (pilihan != 3);

    return 0;
}
