#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

// Struktur data barang
typedef struct {
    char nama[50];
    char kategori[50];
    int harga;
    int stok;
} Barang;

// Struktur data transaksi
typedef struct {
    char namaPelanggan[50];
    time_t waktuTransaksi;
    Barang barang[100];
    int jumlah[100];
    int totalItem;
    float totalHarga;
    float diskon;
    float totalSetelahDiskon;
    float pembayaran;
    float kembalian;
} Transaksi;

// Deklarasi fungsi
int muatDataBarang(Barang barang[], int *jumlahBarang);
void simpanDataBarang(Barang barang[], int jumlahBarang);
void simpanTransaksi(Transaksi transaksi);
void tampilkanBarang(Barang barang[], int jumlahBarang);
void tambahStok(Barang barang[], int jumlahBarang);
void tambahBarang(Barang barang[], int *jumlahBarang);
void cetakStruk(Transaksi transaksi, int bulanSekarang);
int bacaInteger(char *pesan);
void clearBuffer();
void tampilkanMenu();
void tampilkanMenuAdmin();
void printLine(int length);
void printHeader(char *title);

// Fungsi untuk membuat garis pemisah
void printLine(int length) {
    for(int i = 0; i < length; i++) {
        printf("=");
    }
    printf("\n");
}

// Fungsi untuk mencetak header dengan format rapi
void printHeader(char *title) {
    printf("\n");
    printLine(60);
    printf("%*s\n", (60 + strlen(title)) / 2, title);
    printLine(60);
}

// Fungsi untuk memuat data barang dari file
int muatDataBarang(Barang barang[], int *jumlahBarang) {
    FILE *file = fopen("barang.txt", "r");
    if (file == NULL) {
        return 0; // File tidak ada, akan dibuat nanti
    }

    *jumlahBarang = 0;
    while (fscanf(file, "%49[^|]|%49[^|]|%d|%d\n",
                  barang[*jumlahBarang].nama,
                  barang[*jumlahBarang].kategori,
                  &barang[*jumlahBarang].harga,
                  &barang[*jumlahBarang].stok) == 4) {
        (*jumlahBarang)++;
        if (*jumlahBarang >= 100) break;
    }

    fclose(file);
    return 1;
}

// Fungsi untuk menyimpan data barang ke file
void simpanDataBarang(Barang barang[], int jumlahBarang) {
    FILE *file = fopen("barang.txt", "w");
    if (file == NULL) {
        printf("Error: Tidak dapat menyimpan data barang!\n");
        return;
    }

    for (int i = 0; i < jumlahBarang; i++) {
        fprintf(file, "%s|%s|%d|%d\n",
                barang[i].nama,
                barang[i].kategori,
                barang[i].harga,
                barang[i].stok);
    }

    fclose(file);
}

// Fungsi untuk menyimpan transaksi ke file
void simpanTransaksi(Transaksi transaksi) {
    FILE *file = fopen("transaksi.txt", "a");
    if (file == NULL) {
        printf("Error: Tidak dapat menyimpan transaksi!\n");
        return;
    }

    fprintf(file, "=== TRANSAKSI ===\n");
    fprintf(file, "Nama Pelanggan: %s\n", transaksi.namaPelanggan);
    fprintf(file, "Waktu: %s", ctime(&transaksi.waktuTransaksi));
    fprintf(file, "Barang yang dibeli:\n");
    fprintf(file, "No. | Nama Barang           | Jumlah | Harga Satuan | Subtotal\n");
    fprintf(file, "----|-----------------------|--------|--------------|----------\n");

    for (int i = 0; i < transaksi.totalItem; i++) {
        fprintf(file, "%2d. | %-21s | %6d | Rp %9d | Rp %8.0f\n",
                i + 1,
                transaksi.barang[i].nama,
                transaksi.jumlah[i],
                transaksi.barang[i].harga,
                (float)transaksi.jumlah[i] * transaksi.barang[i].harga);
    }

    fprintf(file, "----|-----------------------|--------|--------------|----------\n");
    fprintf(file, "Total Harga: Rp %.0f\n", transaksi.totalHarga);
    fprintf(file, "Diskon: Rp %.0f\n", transaksi.diskon);
    fprintf(file, "Total Setelah Diskon: Rp %.0f\n", transaksi.totalSetelahDiskon);
    fprintf(file, "Pembayaran: Rp %.0f\n", transaksi.pembayaran);
    fprintf(file, "Kembalian: Rp %.0f\n", transaksi.kembalian);
    fprintf(file, "=================================\n\n");

    fclose(file);
}

// Fungsi tampilkan daftar barang
void tampilkanBarang(Barang barang[], int jumlahBarang) {
    printHeader("DAFTAR BARANG TOKO SEMBAKO");
    printf("%-4s %-22s %-20s %-15s %s\n", "No.", "Nama Barang", "Kategori", "Harga", "Stok");
    printLine(80);

    for (int i = 0; i < jumlahBarang; i++) {
        printf("%-4d %-22s %-20s Rp%-12d %3d\n",
               i + 1,
               barang[i].nama,
               barang[i].kategori,
               barang[i].harga,
               barang[i].stok);
    }
    printf("\n");
}

// Fungsi untuk menambah stok barang
void tambahStok(Barang barang[], int jumlahBarang) {
    tampilkanBarang(barang, jumlahBarang);

    int pilih = bacaInteger("Pilih nomor barang yang ingin ditambah stok: ");

    if (pilih < 1 || pilih > jumlahBarang) {
        printf("Nomor barang tidak valid!\n");
        return;
    }

    int tambahan = bacaInteger("Masukkan jumlah stok yang ingin ditambah: ");

    if (tambahan <= 0) {
        printf("Jumlah stok harus lebih dari 0.\n");
        return;
    }

    barang[pilih - 1].stok += tambahan;
    printf("\nStok %s berhasil ditambah menjadi %d\n",
           barang[pilih - 1].nama, barang[pilih - 1].stok);

    simpanDataBarang(barang, jumlahBarang);
}

// Fungsi untuk menambah barang baru
void tambahBarang(Barang barang[], int *jumlahBarang) {
    if (*jumlahBarang >= 100) {
        printf("Maaf, kapasitas barang sudah penuh!\n");
        return;
    }

    printHeader("TAMBAH BARANG BARU");

    printf("Masukkan nama barang: ");
    fgets(barang[*jumlahBarang].nama, sizeof(barang[*jumlahBarang].nama), stdin);
    barang[*jumlahBarang].nama[strcspn(barang[*jumlahBarang].nama, "\n")] = 0;

    printf("Masukkan kategori barang: ");
    fgets(barang[*jumlahBarang].kategori, sizeof(barang[*jumlahBarang].kategori), stdin);
    barang[*jumlahBarang].kategori[strcspn(barang[*jumlahBarang].kategori, "\n")] = 0;

    barang[*jumlahBarang].harga = bacaInteger("Masukkan harga barang: ");
    barang[*jumlahBarang].stok = bacaInteger("Masukkan stok awal: ");

    (*jumlahBarang)++;
    printf("\nBarang berhasil ditambahkan!\n");

    simpanDataBarang(barang, *jumlahBarang);
}

// Fungsi untuk mengedit barang
void editBarang(Barang barang[], int jumlahBarang) {
    tampilkanBarang(barang, jumlahBarang);

    int pilih = bacaInteger("Pilih nomor barang yang ingin diedit: ");

    if (pilih < 1 || pilih > jumlahBarang) {
        printf("Nomor barang tidak valid!\n");
        return;
    }

    printHeader("EDIT BARANG");
    printf("Barang yang akan diedit: %s\n", barang[pilih - 1].nama);

    printf("Nama barang baru [%s]: ", barang[pilih - 1].nama);
    char inputNama[50];
    fgets(inputNama, sizeof(inputNama), stdin);
    inputNama[strcspn(inputNama, "\n")] = 0;
    if (strlen(inputNama) > 0) {
        strcpy(barang[pilih - 1].nama, inputNama);
    }

    printf("Kategori baru [%s]: ", barang[pilih - 1].kategori);
    char inputKategori[50];
    fgets(inputKategori, sizeof(inputKategori), stdin);
    inputKategori[strcspn(inputKategori, "\n")] = 0;
    if (strlen(inputKategori) > 0) {
        strcpy(barang[pilih - 1].kategori, inputKategori);
    }

    printf("Harga baru [%d]: ", barang[pilih - 1].harga);
    char inputHarga[20];
    fgets(inputHarga, sizeof(inputHarga), stdin);
    inputHarga[strcspn(inputHarga, "\n")] = 0;
    if (strlen(inputHarga) > 0) {
        barang[pilih - 1].harga = atoi(inputHarga);
    }

    printf("Stok baru [%d]: ", barang[pilih - 1].stok);
    char inputStok[20];
    fgets(inputStok, sizeof(inputStok), stdin);
    inputStok[strcspn(inputStok, "\n")] = 0;
    if (strlen(inputStok) > 0) {
        barang[pilih - 1].stok = atoi(inputStok);
    }

    printf("\nBarang berhasil diedit!\n");
    simpanDataBarang(barang, jumlahBarang);
}

// Fungsi untuk menghapus barang
void hapusBarang(Barang barang[], int *jumlahBarang) {
    tampilkanBarang(barang, *jumlahBarang);

    int pilih = bacaInteger("Pilih nomor barang yang ingin dihapus: ");

    if (pilih < 1 || pilih > *jumlahBarang) {
        printf("Nomor barang tidak valid!\n");
        return;
    }

    printf("Apakah Anda yakin ingin menghapus barang '%s'? (y/n): ", barang[pilih - 1].nama);
    char konfirmasi;
    scanf(" %c", &konfirmasi);
    clearBuffer();

    if (konfirmasi == 'y' || konfirmasi == 'Y') {
        // Geser semua barang setelah yang dihapus
        for (int i = pilih - 1; i < *jumlahBarang - 1; i++) {
            barang[i] = barang[i + 1];
        }
        (*jumlahBarang)--;
        printf("Barang berhasil dihapus!\n");
        simpanDataBarang(barang, *jumlahBarang);
    } else {
        printf("Penghapusan dibatalkan.\n");
    }
}

// Fungsi cetak struk
void cetakStruk(Transaksi transaksi, int bulanSekarang) {
    printHeader("STRUK PEMBELIAN");
    printf("Toko Sembako Kelompok 9\n");
    printf("Nama Pelanggan : %s\n", transaksi.namaPelanggan);
    printf("Bulan Pembelian: %d\n", bulanSekarang);

    char waktuStr[100];
    strftime(waktuStr, sizeof(waktuStr), "%d-%m-%Y %H:%M:%S", localtime(&transaksi.waktuTransaksi));
    printf("Waktu Transaksi: %s\n", waktuStr);

    printf("\n%-4s %-22s %-8s %-13s %s\n", "No.", "Nama Barang", "Jumlah", "Harga", "Subtotal");
    printLine(70);

    for (int i = 0; i < transaksi.totalItem; i++) {
        float subtotal = (float)transaksi.jumlah[i] * transaksi.barang[i].harga;
        printf("%-4d %-22s %-8d Rp%-10d Rp%-10.0f\n",
               i + 1,
               transaksi.barang[i].nama,
               transaksi.jumlah[i],
               transaksi.barang[i].harga,
               subtotal);
    }

    printLine(70);
    printf("%-42s: Rp%10.0f\n", "Total Harga", transaksi.totalHarga);

    if (transaksi.diskon > 0) {
        printf("%-42s: Rp%10.0f\n", "Diskon", transaksi.diskon);
        printf("%-42s: Rp%10.0f\n", "Total Setelah Diskon", transaksi.totalSetelahDiskon);
    }

    printf("%-42s: Rp%10.0f\n", "Pembayaran", transaksi.pembayaran);
    printf("%-42s: Rp%10.0f\n", "Kembalian", transaksi.kembalian);

    printLine(70);
    printf("Terima kasih telah berbelanja di Toko Sembako Kelompok 9!\n");
}

// Fungsi untuk validasi input angka
int bacaInteger(char *pesan) {
    char input[100];
    int nilai;

    while (1) {
        printf("%s", pesan);
        if (fgets(input, sizeof(input), stdin) != NULL) {
            // Hapus newline character
            input[strcspn(input, "\n")] = 0;

            // Validasi apakah input hanya berisi angka
            int valid = 1;
            for (int i = 0; input[i] != '\0'; i++) {
                if (!isdigit(input[i])) {
                    valid = 0;
                    break;
                }
            }

            if (valid && sscanf(input, "%d", &nilai) == 1) {
                return nilai;
            }
        }
        printf("Input tidak valid! Harap masukkan angka bulat.\n");
    }
}

// Fungsi untuk membersihkan buffer input
void clearBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Menu utama
void tampilkanMenu() {
    printHeader("TOKO SEMBAKO KELOMPOK 9");
    printf("1. Belanja\n");
    printf("2. Mode Admin\n");
    printf("3. Keluar\n");
    printf("Pilih menu (1-3): ");
}

// Menu admin
void tampilkanMenuAdmin() {
    printHeader("MODE ADMIN - TOKO SEMBAKO");
    printf("1. Lihat Daftar Barang\n");
    printf("2. Tambah Stok Barang\n");
    printf("3. Tambah Barang Baru\n");
    printf("4. Edit Barang\n");
    printf("5. Hapus Barang\n");
    printf("6. Kembali ke Menu Utama\n");
    printf("Pilih menu (1-6): ");
}

// Fungsi untuk mode admin
void modeAdmin(Barang barang[], int *jumlahBarang) {
    int menuAdmin;

    do {
        tampilkanMenuAdmin();
        menuAdmin = bacaInteger("");

        switch (menuAdmin) {
            case 1:
                tampilkanBarang(barang, *jumlahBarang);
                break;

            case 2:
                tambahStok(barang, *jumlahBarang);
                break;

            case 3:
                tambahBarang(barang, jumlahBarang);
                break;

            case 4:
                editBarang(barang, *jumlahBarang);
                break;

            case 5:
                hapusBarang(barang, jumlahBarang);
                break;

            case 6:
                printf("Kembali ke menu utama...\n");
                break;

            default:
                printf("Menu tidak valid! Pilih antara 1-6.\n");
        }
    } while (menuAdmin != 6);
}

int main() {
    // Inisialisasi data barang
    Barang barang[100];
    int jumlahBarang = 0;

    // Coba muat data dari file
    if (!muatDataBarang(barang, &jumlahBarang)) {
        // Jika file tidak ada, buat data default
        Barang barangDefault[] = {
            {"Beras", "Makanan Pokok", 15000, 50},
            {"Gula", "Makanan Pokok", 5000, 30},
            {"Minyak Goreng", "Makanan Pokok", 10000, 40},
            {"Teh", "Minuman", 5000, 25},
            {"Kopi", "Minuman", 2000, 20},
            {"Sabun", "Kebutuhan Rumah Tangga", 3000, 60},
            {"Shampoo", "Kebutuhan Rumah Tangga", 1000, 15}
        };

        jumlahBarang = sizeof(barangDefault) / sizeof(barangDefault[0]);
        for (int i = 0; i < jumlahBarang; i++) {
            barang[i] = barangDefault[i];
        }

        simpanDataBarang(barang, jumlahBarang);
        printf("Data barang default berhasil dibuat!\n");
    }

    int menu;

    do {
        tampilkanMenu();
        menu = bacaInteger("");

        switch (menu) {
            case 1: {
                // Proses belanja
                Transaksi transaksi;
                transaksi.totalItem = 0;
                transaksi.waktuTransaksi = time(NULL);

                printf("\nMasukkan nama pelanggan: ");
                fgets(transaksi.namaPelanggan, sizeof(transaksi.namaPelanggan), stdin);
                transaksi.namaPelanggan[strcspn(transaksi.namaPelanggan, "\n")] = 0;

                printf("\nSelamat datang %s!\n", transaksi.namaPelanggan);

                printHeader("PROMO DISKON");
                printf("1. Diskon 3%% untuk pembelian > Rp 50.000\n");
                printf("2. Diskon 5%% untuk pembelian > Rp 100.000\n");
                printf("3. Tambahan diskon 5%% otomatis selama bulan November!\n\n");

                tampilkanBarang(barang, jumlahBarang);

                // Dapatkan bulan saat ini
                time_t t = time(NULL);
                struct tm tm = *localtime(&t);
                int bulanSekarang = tm.tm_mon + 1;

                char lanjut = 'y';
                while ((lanjut == 'y' || lanjut == 'Y') && transaksi.totalItem < 100) {
                    int pilih = bacaInteger("Pilih nomor barang yang ingin dibeli: ");

                    if (pilih < 1 || pilih > jumlahBarang) {
                        printf("Nomor barang tidak valid! Pilih antara 1-%d\n", jumlahBarang);
                        continue;
                    }

                    int qty = bacaInteger("Masukkan jumlah barang: ");

                    if (qty <= 0) {
                        printf("Jumlah harus lebih dari 0.\n");
                        continue;
                    }

                    // Cek stok
                    if (qty > barang[pilih - 1].stok) {
                        printf("Stok tidak cukup! Stok tersedia: %d\n", barang[pilih - 1].stok);
                        continue;
                    }

                    // Kurangi stok
                    barang[pilih - 1].stok -= qty;

                    // Simpan ke transaksi
                    transaksi.barang[transaksi.totalItem] = barang[pilih - 1];
                    transaksi.jumlah[transaksi.totalItem] = qty;
                    transaksi.totalItem++;

                    printf("Apakah ingin beli barang lain? (y/n): ");
                    scanf(" %c", &lanjut);
                    clearBuffer();
                }

                if (transaksi.totalItem > 0) {
                    // Hitung total harga
                    transaksi.totalHarga = 0;
                    for (int i = 0; i < transaksi.totalItem; i++) {
                        transaksi.totalHarga += (float)transaksi.jumlah[i] * transaksi.barang[i].harga;
                    }

                    // Hitung diskon sesuai permintaan baru
                    transaksi.diskon = 0.0;
                    if (transaksi.totalHarga > 100000) {
                        transaksi.diskon = transaksi.totalHarga * 0.05; // 5% untuk > 100.000
                        printf("\nSelamat! Anda mendapatkan diskon 5%%\n");
                    } else if (transaksi.totalHarga > 50000) {
                        transaksi.diskon = transaksi.totalHarga * 0.03; // 3% untuk > 50.000
                        printf("\nSelamat! Anda mendapatkan diskon 3%%\n");
                    }

                    // Tambahan 5% jika bulan sekarang adalah November
                    if (bulanSekarang == 11) {
                        float diskonNovember = transaksi.totalHarga * 0.05;
                        transaksi.diskon += diskonNovember;
                        printf("PROMO NOVEMBER! Anda mendapat tambahan diskon 5%%\n");
                    }

                    transaksi.totalSetelahDiskon = transaksi.totalHarga - transaksi.diskon;

                    // Input pembayaran
                    do {
                        printf("\n" );
                        printLine(40);
                        printf("%-20s: Rp%10.0f\n", "Total Belanja", transaksi.totalHarga);
                        printf("%-20s: Rp%10.0f\n", "Diskon", transaksi.diskon);
                        printf("%-20s: Rp%10.0f\n", "Total Bayar", transaksi.totalSetelahDiskon);
                        printLine(40);

                        transaksi.pembayaran = (float)bacaInteger("\nMasukkan jumlah pembayaran (Rp): ");

                        if (transaksi.pembayaran < transaksi.totalSetelahDiskon) {
                            float kekurangan = transaksi.totalSetelahDiskon - transaksi.pembayaran;
                            printf("Pembayaran kurang Rp %.0f! Harus bayar minimal Rp %.0f\n",
                                   kekurangan, transaksi.totalSetelahDiskon);
                        }
                    } while (transaksi.pembayaran < transaksi.totalSetelahDiskon);

                    transaksi.kembalian = transaksi.pembayaran - transaksi.totalSetelahDiskon;

                    cetakStruk(transaksi, bulanSekarang);
                    simpanTransaksi(transaksi);
                    simpanDataBarang(barang, jumlahBarang);
                } else {
                    printf("Tidak ada barang yang dibeli.\n");
                }
                break;
            }

            case 2:
                modeAdmin(barang, &jumlahBarang);
                break;

            case 3:
                printf("\nTerima kasih! Data barang disimpan.\n");
                simpanDataBarang(barang, jumlahBarang);
                break;

            default:
                printf("Menu tidak valid! Pilih antara 1-3.\n");
        }
    } while (menu != 3);

    return 0;
}
