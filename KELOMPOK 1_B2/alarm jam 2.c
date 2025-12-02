#include <stdio.h>
#include <time.h>
#include <unistd.h>  // sleep(), usleep()
#include <stdlib.h>  // system()

#define MAX_ALARM 10
struct Alarm {
    int jam;
    int menit;
    int detik;
};

struct Alarm alarmList[MAX_ALARM];
int totalAlarm = 0;


void displayTime() {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    printf("\r%02d:%02d:%02d ", t->tm_hour, t->tm_min, t->tm_sec);
    fflush(stdout);
}


int checkAlarm() {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    for(int i = 0; i < totalAlarm; i++) {
        if (t->tm_hour == alarmList[i].jam &&
            t->tm_min  == alarmList[i].menit &&
            t->tm_sec  == alarmList[i].detik) {

            printf("\n\nAlarm Berbunyi! Waktu %02d:%02d:%02d\n",
                    alarmList[i].jam, alarmList[i].menit, alarmList[i].detik);

            // Bunyi alarm 30 detik (0.2 detik per beep)
            for(int b = 0; b < 20; b++) {
                printf("\a");              // bunyi beep
                fflush(stdout);
                usleep(3000);            // 0.003 detik
            }

            printf("\nAlarm selesai!\n");
            return 1;
        }
    }
    return 0;
}


void tambahAlarm() {
    if(totalAlarm >= MAX_ALARM) {
        printf("Maksimal jumlah alarm tercapai!\n");
        return;
    }
    printf("Masukkan alarm (jam menit detik): ");
    scanf("%d %d %d", &alarmList[totalAlarm].jam,
                      &alarmList[totalAlarm].menit,
                      &alarmList[totalAlarm].detik);
    totalAlarm++;
    printf("Alarm ditambahkan!\n");
}


void lihatAlarm() {
    if(totalAlarm == 0) {
        printf("Tidak ada alarm yang disimpan.\n");
        return;
    }
    printf("\n===== DAFTAR ALARM =====\n");
    for(int i = 0; i < totalAlarm; i++) {
        printf("%d) %02d:%02d:%02d\n", i+1,
                alarmList[i].jam, alarmList[i].menit, alarmList[i].detik);
    }
}


void editAlarm() {
    lihatAlarm();
    if(totalAlarm == 0) return;

    int pilih;
    printf("\nPilih nomor alarm yang ingin diedit: ");
    scanf("%d", &pilih);

    if(pilih < 1 || pilih > totalAlarm) {
        printf("Nomor tidak valid!\n");
        return;
    }

    printf("Masukkan waktu alarm baru (jam menit detik): ");
    scanf("%d %d %d", &alarmList[pilih-1].jam,
                      &alarmList[pilih-1].menit,
                      &alarmList[pilih-1].detik);
    printf("Alarm telah diperbarui!\n");
}


void hapusAlarm() {
    lihatAlarm();
    if(totalAlarm == 0) return;

    int pilih;
    printf("\nPilih nomor alarm yang ingin dihapus: ");
    scanf("%d", &pilih);

    if(pilih < 1 || pilih > totalAlarm) {
        printf("Nomor tidak valid!\n");
        return;
    }

    for(int i = pilih - 1; i < totalAlarm - 1; i++) {
        alarmList[i] = alarmList[i+1];
    }
    totalAlarm--;
    printf("Alarm telah dihapus!\n");
}


void mulaiAlarm() {
    char pilih;

    printf("\n=============== Alarm Aktif  ===============\n");
    printf("\n\n");

    while(1) {
        displayTime();

        if(checkAlarm()) {
            printf("\nKembali ke menu? (y/n): ");
            scanf(" %c", &pilih);

            if(pilih == 'y' || pilih == 'Y') {
                return; // kembali ke menu
            }
        }
        sleep(1);
    }
}


int main() {
    int pilih;
    do {
        printf("\n===== MENU ALARM JAM =====\n");
        printf("1. Tambah Alarm\n");
        printf("2. Edit Alarm\n");
        printf("3. Hapus Alarm\n");
        printf("4. Lihat Daftar Alarm\n");
        printf("5. Mulai Jalankan Alarm\n");
        printf("0. Keluar\n");
        printf("Pilih menu: ");
        scanf("%d", &pilih);

        switch(pilih) {
            case 1: tambahAlarm(); break;
            case 2: editAlarm(); break;
            case 3: hapusAlarm(); break;
            case 4: lihatAlarm(); break;
            case 5: mulaiAlarm(); break;
            case 0: printf("Keluar program...\n"); break;
            default: printf("Menu tidak valid!\n");
        }

    } while(pilih != 0);

    return 0;
}


