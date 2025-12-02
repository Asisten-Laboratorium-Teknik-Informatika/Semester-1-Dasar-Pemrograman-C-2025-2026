#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_ITEMS 100
#define ADMIN_USERNAME "admin"
#define ADMIN_PASSWORD "12345"

// --- STRUKTUR DATA ---
typedef struct {
    char nama[50];
    char kategori[50];
    int harga;
} JasaBengkel;

typedef struct {
    char nama[50];
    char kategori[50];
    int harga;
    int stok;
} Sparepart;

// --- FUNGSI HELPER ---

// Mengubah angka menjadi format rupiah (contoh: 50000 -> 50.000)
void formatRupiah(int angka, char *output) {
    char str[50];
    sprintf(str, "%d", angka);
    int len = strlen(str);
    int i, j = 0;

    for (i = 0; i < len; i++) {
        if (i > 0 && (len - i) % 3 == 0) {
            output[j++] = '.';
        }
        output[j++] = str[i];
    }
    output[j] = '\0';
}

// Membaca input string dengan aman
void bacaString(char *pesan, char *buffer, int maxLength) {
    printf("%s", pesan);
    fflush(stdout);

    if (fgets(buffer, maxLength, stdin) != NULL) {
        buffer[strcspn(buffer, "\n")] = 0;
    }
}

// Membaca input integer dengan validasi
int bacaInteger(char *pesan) {
    char input[100];
    int nilai;
    int valid;

    do {
        printf("%s", pesan);
        fflush(stdout);

        if (fgets(input, sizeof(input), stdin) != NULL) {
            input[strcspn(input, "\n")] = 0;
            valid = 1;
            if (strlen(input) == 0) valid = 0;

            for (int i = 0; input[i] != '\0'; i++) {
                if (!isdigit(input[i])) {
                    valid = 0;
                    break;
                }
            }

            if (valid && sscanf(input, "%d", &nilai) == 1) {
                return nilai;
            } else {
                printf("[!] Input tidak valid! Harap masukkan angka bulat.\n");
            }
        }
    } while (1);
}

// --- FUNGSI TAMPILKAN DATA ---

void tampilkanJasa(JasaBengkel jasa[], int jumlahJasa) {
    char rp[30];
    printf("\n=== DAFTAR JASA BENGKEL ===\n");
    printf("No. | Nama Jasa              | Harga\n");
    printf("----|------------------------|----------------\n");
    for (int i = 0; i < jumlahJasa; i++) {
        formatRupiah(jasa[i].harga, rp);
        printf("%2d. | %-22s | Rp %-10s\n",
               i + 1, jasa[i].nama, rp);
    }
    printf("\n");
}

void tampilkanSparepart(Sparepart sparepart[], int jumlahSparepart) {
    char rp[30];
    printf("\n=== DAFTAR SPAREPART ===\n");
    printf("No. | Nama Sparepart         | Harga          | Stok\n");
    printf("----|------------------------|----------------|------\n");
    for (int i = 0; i < jumlahSparepart; i++) {
        formatRupiah(sparepart[i].harga, rp);
        printf("%2d. | %-22s | Rp %-10s | %d\n",
               i + 1, sparepart[i].nama, rp, sparepart[i].stok);
    }
    printf("\n");
}

// --- FUNGSI ADMIN ---

int loginAdmin() {
    char username[50];
    char password[50];

    printf("\n=== LOGIN ADMIN ===\n");
    bacaString("Username: ", username, sizeof(username));
    bacaString("Password: ", password, sizeof(password));

    if (strcmp(username, ADMIN_USERNAME) == 0 && strcmp(password, ADMIN_PASSWORD) == 0) {
        printf("Login admin berhasil!\n");
        return 1;
    } else {
        printf("Login gagal! Username atau password salah.\n");
        return 0;
    }
}

void tambahStokSparepart(Sparepart sparepart[], int jumlahSparepart) {
    printf("\n=== TAMBAH STOK SPAREPART ===\n");
    tampilkanSparepart(sparepart, jumlahSparepart);

    int pilihan = bacaInteger("Pilih nomor sparepart yang ingin ditambah stok: ");

    if (pilihan < 1 || pilihan > jumlahSparepart) {
        printf("[!] Nomor sparepart tidak valid!\n");
        return;
    }

    int index = pilihan - 1;
    printf("Sparepart: %s\n", sparepart[index].nama);
    printf("Stok saat ini: %d\n", sparepart[index].stok);

    int tambahan = bacaInteger("Masukkan jumlah stok yang ingin ditambah: ");

    if (tambahan <= 0) {
        printf("[!] Jumlah harus lebih dari 0!\n");
        return;
    }

    sparepart[index].stok += tambahan;
    printf("✅ Stok berhasil ditambah! Stok sekarang: %d\n", sparepart[index].stok);
}

void menuAdmin(Sparepart sparepart[], int jumlahSparepart) {
    int pilihan;
    do {
        printf("\n=== MENU ADMIN ===\n");
        printf("1. Lihat Stok Sparepart\n");
        printf("2. Tambah Stok Sparepart\n");
        printf("3. Kembali ke Menu Utama\n");

        pilihan = bacaInteger("Pilih menu (1-3): ");

        switch (pilihan) {
            case 1:
                tampilkanSparepart(sparepart, jumlahSparepart);
                break;
            case 2:
                tambahStokSparepart(sparepart, jumlahSparepart);
                break;
            case 3:
                printf("Kembali ke menu utama...\n");
                break;
            default:
                printf("[!] Pilihan tidak valid!\n");
        }
    } while (pilihan != 3);
}

// --- FUNGSI CETAK STRUK (DENGAN WAKTU) ---

void cetakStrukBengkel(JasaBengkel jasa[], Sparepart sparepart[],
                       int pilihanJasa[], int jumlahJasa[], int totalJasa,
                       int pilihanSparepart[], int jumlahSparepart[], int totalSparepartItem,
                       double totalHarga, double diskon, double totalSetelahDiskon,
                       double pembayaran, double kembalian,
                       char namaPelanggan[], char noKendaraan[]) {

    char rpSatuan[30], rpTotal[30], rpFinal[30], rpBayar[30], rpKembali[30], rpDiskon[30];

    // -- LOGIKA WAKTU --
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char waktuStr[100];
    // Format: Tanggal-Bulan-Tahun Jam:Menit:Detik
    sprintf(waktuStr, "%02d-%02d-%04d %02d:%02d:%02d",
            tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900,
            tm.tm_hour, tm.tm_min, tm.tm_sec);

    // -- OUTPUT KE LAYAR --
    printf("\n========================================\n");
    printf("        STRUK BENGKEL MOTOR SEJAHTERA     \n");
    printf("========================================\n");
    printf("Waktu          : %s\n", waktuStr); // Menampilkan Waktu
    printf("Nama Pelanggan : %s\n", namaPelanggan);
    printf("No. Kendaraan  : %s\n", noKendaraan);
    printf("----------------------------------------\n");

    if (totalJasa > 0) {
        printf("JASA:\n");
        for (int i = 0; i < totalJasa; i++) {
            int idx = pilihanJasa[i] - 1;
            double subtotal = (double)jumlahJasa[i] * jasa[idx].harga;

            formatRupiah(jasa[idx].harga, rpSatuan);
            formatRupiah((int)subtotal, rpTotal);

            printf("- %-20s \n  %d x Rp %-10s = Rp %s\n",
                   jasa[idx].nama, jumlahJasa[i], rpSatuan, rpTotal);
        }
    }

    if (totalSparepartItem > 0) {
        printf("\nSPAREPART:\n");
        for (int i = 0; i < totalSparepartItem; i++) {
            int idx = pilihanSparepart[i] - 1;
            double subtotal = (double)jumlahSparepart[i] * sparepart[idx].harga;

            formatRupiah(sparepart[idx].harga, rpSatuan);
            formatRupiah((int)subtotal, rpTotal);

            printf("- %-20s \n  %d x Rp %-10s = Rp %s\n",
                   sparepart[idx].nama, jumlahSparepart[i], rpSatuan, rpTotal);
        }
    }

    formatRupiah((int)totalHarga, rpTotal);
    formatRupiah((int)totalSetelahDiskon, rpFinal);
    formatRupiah((int)pembayaran, rpBayar);
    formatRupiah((int)kembalian, rpKembali);
    formatRupiah((int)diskon, rpDiskon);

    printf("========================================\n");
    printf("Total Harga          : Rp %s\n", rpTotal);
    if (diskon > 0) {
        printf("Diskon               : Rp %s\n", rpDiskon);
    }
    printf("Total Bayar          : Rp %s\n", rpFinal);
    printf("Pembayaran           : Rp %s\n", rpBayar);
    printf("Kembalian            : Rp %s\n", rpKembali);
    printf("========================================\n");
    printf("    Terima kasih atas kunjungan Anda!    \n");
    printf("========================================\n");

    // -- SIMPAN KE FILE --
    char namaFile[100];
    sprintf(namaFile, "Struk_%s.txt", namaPelanggan);
    FILE *fp = fopen(namaFile, "w");

    if (fp != NULL) {
        fprintf(fp, "========================================\n");
        fprintf(fp, "        STRUK BENGKEL MOTOR SEJAHTERA     \n");
        fprintf(fp, "========================================\n");
        fprintf(fp, "Waktu          : %s\n", waktuStr); // Simpan Waktu ke File
        fprintf(fp, "Nama Pelanggan : %s\n", namaPelanggan);
        fprintf(fp, "No. Kendaraan  : %s\n", noKendaraan);
        fprintf(fp, "----------------------------------------\n");

        if (totalJasa > 0) {
            fprintf(fp, "JASA:\n");
            for (int i = 0; i < totalJasa; i++) {
                int idx = pilihanJasa[i] - 1;
                double subtotal = (double)jumlahJasa[i] * jasa[idx].harga;

                formatRupiah(jasa[idx].harga, rpSatuan);
                formatRupiah((int)subtotal, rpTotal);

                fprintf(fp, "- %-20s \n  %d x Rp %-10s = Rp %s\n",
                        jasa[idx].nama, jumlahJasa[i], rpSatuan, rpTotal);
            }
        }

        if (totalSparepartItem > 0) {
            fprintf(fp, "\nSPAREPART:\n");
            for (int i = 0; i < totalSparepartItem; i++) {
                int idx = pilihanSparepart[i] - 1;
                double subtotal = (double)jumlahSparepart[i] * sparepart[idx].harga;

                formatRupiah(sparepart[idx].harga, rpSatuan);
                formatRupiah((int)subtotal, rpTotal);

                fprintf(fp, "- %-20s \n  %d x Rp %-10s = Rp %s\n",
                        sparepart[idx].nama, jumlahSparepart[i], rpSatuan, rpTotal);
            }
        }

        fprintf(fp, "========================================\n");
        formatRupiah((int)totalHarga, rpTotal);
        fprintf(fp, "Total Harga          : Rp %s\n", rpTotal);
        if (diskon > 0) {
            fprintf(fp, "Diskon               : Rp %s\n", rpDiskon);
        }
        fprintf(fp, "Total Bayar          : Rp %s\n", rpFinal);
        fprintf(fp, "Pembayaran           : Rp %s\n", rpBayar);
        fprintf(fp, "Kembalian            : Rp %s\n", rpKembali);
        fprintf(fp, "========================================\n");
        fprintf(fp, "    Terima kasih atas kunjungan Anda!    \n");
        fprintf(fp, "========================================\n");

        fclose(fp);
        printf("\n[INFO] Struk berhasil disimpan ke file: %s\n", namaFile);
    }
}

// --- FUNGSI UTAMA (MAIN) ---

int main() {
    JasaBengkel jasa[] = {
        {"Ganti Oli Mesin", "Servis Rutin", 50000},
        {"Cleaning CVT", "Servis Transmisi", 30000},
        {"Bubut Rumah Roller", "Modifikasi", 50000},
        {"Tune Up Karburator", "Servis Mesin", 75000},
        {"Service Rem", "Servis Keselamatan", 60000},
        {"Ganti Kampas Rem", "Servis Keselamatan", 120000},
        {"Service Rantai", "Servis Transmisi", 30000},
        {"Ganti Busi", "Servis Mesin", 25000},
        {"Service Lampu", "Servis Kelistrikan", 40000},
        {"Ganti Ban Dalam", "Servis Roda", 35000}
    };
    int jumlahJasaTersedia = sizeof(jasa) / sizeof(jasa[0]);

    Sparepart sparepart[] = {
        {"Oli Mesin 1L", "Pelumas", 45000, 20},
        {"V-Belt Matic", "Transmisi", 50000, 15},
        {"Rumah Roller Assy", "Transmisi", 85000, 10},
        {"Roller Set (6pcs)", "Transmisi", 45000, 20},
        {"Kampas Rem Depan", "Rem", 55000, 15},
        {"Kampas Rem Belakang", "Rem", 45000, 15},
        {"Busi Standard", "Pengapian", 15000, 25},
        {"Ban Dalam", "Ban", 25000, 30},
        {"Rantai Motor", "Transmisi", 85000, 10},
        {"Filter Udara", "Mesin", 35000, 15}
    };
    int jumlahSparepartTersedia = sizeof(sparepart) / sizeof(sparepart[0]);

    char aplikasiJalan = 'y';
    char buffer[100];

    while (aplikasiJalan == 'y' || aplikasiJalan == 'Y') {
        printf("\n\n#########################################\n");
        printf("   SELAMAT DATANG DI BENGKEL KELOMPOK 4   \n");
        printf("#########################################\n");
        printf("1. Menu Pelanggan\n");
        printf("2. Menu Admin (Tambah Stok)\n");
        printf("3. Keluar\n");

        int menuUtama = bacaInteger("Pilih menu (1-3): ");

        if (menuUtama == 1) {
            // --- MENU PELANGGAN ---
            int pilihanJasa[MAX_ITEMS], jumlahJasaPilih[MAX_ITEMS], totalJasa = 0;
            int pilihanSparepart[MAX_ITEMS], jumlahSparepartPilih[MAX_ITEMS], totalSparepartItem = 0;
            char namaPelanggan[50];
            char noKendaraan[20];
            char lanjutBelanja = 'y';

            bacaString("Masukkan nama pelanggan: ", namaPelanggan, sizeof(namaPelanggan));
            bacaString("Masukkan nomor kendaraan: ", noKendaraan, sizeof(noKendaraan));

            while (lanjutBelanja == 'y' || lanjutBelanja == 'Y') {
                printf("\n=== PILIH JENIS PESANAN ===\n");
                printf("1. Pesan Jasa Servis\n");
                printf("2. Pesan Sparepart\n");
                printf("3. Selesai & Bayar\n");

                int menuPilihan = bacaInteger("Pilih menu (1-3): ");

                switch (menuPilihan) {
                    case 1: {
                        char tambahLagi = 'y';
                        do {
                            if (totalJasa >= MAX_ITEMS) {
                                printf("Keranjang jasa penuh!\n");
                                break;
                            }
                            tampilkanJasa(jasa, jumlahJasaTersedia);
                            int pilihJasa = bacaInteger("Pilih nomor jasa: ");

                            if (pilihJasa < 1 || pilihJasa > jumlahJasaTersedia) {
                                printf("[!] Nomor jasa tidak valid!\n");
                            } else {
                                int sudahAda = 0;
                                for(int k=0; k<totalJasa; k++) {
                                    if(pilihanJasa[k] == pilihJasa) {
                                        sudahAda = 1;
                                        break;
                                    }
                                }

                                if(sudahAda) {
                                    printf("[!] Jasa '%s' sudah dipilih!\n", jasa[pilihJasa-1].nama);
                                } else {
                                    pilihanJasa[totalJasa] = pilihJasa;
                                    jumlahJasaPilih[totalJasa] = 1;
                                    totalJasa++;
                                    printf("-> Jasa '%s' ditambahkan.\n", jasa[pilihJasa-1].nama);
                                }
                            }

                            printf("\nTambah jasa lain? (y/n): ");
                            if(fgets(buffer, sizeof(buffer), stdin) != NULL) {
                                tambahLagi = tolower(buffer[0]);
                            }
                        } while (tambahLagi == 'y');
                        break;
                    }

                    case 2: {
                        char tambahLagi = 'y';
                        do {
                            if (totalSparepartItem >= MAX_ITEMS) {
                                printf("Keranjang sparepart penuh!\n");
                                break;
                            }
                            tampilkanSparepart(sparepart, jumlahSparepartTersedia);
                            int pilihSparepart = bacaInteger("Pilih nomor sparepart: ");

                            if (pilihSparepart < 1 || pilihSparepart > jumlahSparepartTersedia) {
                                printf("[!] Nomor sparepart tidak valid!\n");
                            } else {
                                int stokSudahDiKeranjang = 0;
                                for(int i=0; i<totalSparepartItem; i++) {
                                    if(pilihanSparepart[i] == pilihSparepart) {
                                        stokSudahDiKeranjang += jumlahSparepartPilih[i];
                                    }
                                }

                                int sisaStokReal = sparepart[pilihSparepart-1].stok - stokSudahDiKeranjang;

                                if (sisaStokReal <= 0) {
                                    printf("[!] Stok habis!\n");
                                } else {
                                    printf("Stok tersedia: %d\n", sisaStokReal);
                                    int qtySparepart = bacaInteger("Masukkan jumlah: ");

                                    if (qtySparepart <= 0) {
                                        printf("[!] Jumlah harus lebih dari 0.\n");
                                    } else if (qtySparepart > sisaStokReal) {
                                        printf("[!] Stok tidak cukup!\n");
                                    } else {
                                        pilihanSparepart[totalSparepartItem] = pilihSparepart;
                                        jumlahSparepartPilih[totalSparepartItem] = qtySparepart;
                                        totalSparepartItem++;
                                        printf("-> Sparepart ditambahkan.\n");
                                    }
                                }
                            }

                            printf("\nTambah sparepart lain? (y/n): ");
                             if(fgets(buffer, sizeof(buffer), stdin) != NULL) {
                                tambahLagi = tolower(buffer[0]);
                            }
                        } while (tambahLagi == 'y');
                        break;
                    }

                    case 3:
                        lanjutBelanja = 'n';
                        break;

                    default:
                        printf("[!] Pilihan tidak valid!\n");
                }
            }

            // Hitung Total
            double totalHarga = 0;
            for (int i = 0; i < totalJasa; i++) {
                int idx = pilihanJasa[i] - 1;
                totalHarga += (double)jumlahJasaPilih[i] * jasa[idx].harga;
            }
            for (int i = 0; i < totalSparepartItem; i++) {
                int idx = pilihanSparepart[i] - 1;
                totalHarga += (double)jumlahSparepartPilih[i] * sparepart[idx].harga;
            }

            double diskon = 0.0;
            if (totalHarga > 1000000)
                diskon = totalHarga * 0.05;

            double totalSetelahDiskon = totalHarga - diskon;

            if (totalHarga > 0) {
                char rpTampil[30];
                formatRupiah((int)totalSetelahDiskon, rpTampil);
                printf("\n=== PEMBAYARAN ===\n");
                printf("Total Tagihan: Rp %s\n", rpTampil);

                double pembayaran;
                do {
                    pembayaran = (double)bacaInteger("Masukkan jumlah pembayaran (Rp): ");
                    if (pembayaran < totalSetelahDiskon) {
                        double kurang = totalSetelahDiskon - pembayaran;
                        formatRupiah((int)kurang, rpTampil);
                        printf("[!] Pembayaran kurang! Kurang Rp %s\n", rpTampil);
                    }
                } while (pembayaran < totalSetelahDiskon);

                double kembalian = pembayaran - totalSetelahDiskon;

                cetakStrukBengkel(jasa, sparepart, pilihanJasa, jumlahJasaPilih, totalJasa,
                                  pilihanSparepart, jumlahSparepartPilih, totalSparepartItem,
                                  totalHarga, diskon, totalSetelahDiskon, pembayaran,
                                  kembalian, namaPelanggan, noKendaraan);

                // Update stok sparepart secara permanen
                for(int i=0; i<totalSparepartItem; i++) {
                    int idx = pilihanSparepart[i] - 1;
                    sparepart[idx].stok -= jumlahSparepartPilih[i];
                }

            } else {
                printf("\nTidak ada transaksi yang dilakukan.\n");
            }

        } else if (menuUtama == 2) {
            // --- MENU ADMIN ---
            if (loginAdmin()) {
                menuAdmin(sparepart, jumlahSparepartTersedia);
            }
        } else if (menuUtama == 3) {
            aplikasiJalan = 'n';
            printf("Terima kasih, aplikasi ditutup.\n");
        } else {
            printf("[!] Pilihan tidak valid!\n");
        }
    }

    return 0;
}
