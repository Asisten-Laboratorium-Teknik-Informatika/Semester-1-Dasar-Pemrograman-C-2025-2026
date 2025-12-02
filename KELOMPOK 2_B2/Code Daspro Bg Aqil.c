/* program_kasir_final_fix.c
   Fitur: Login 3x, Tabel Rapi, Pemisah Ribuan, Simpan Struk, LOGIKA STOK DIPERBAIKI.
*/

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#define ITEMS 9
#define MAX_LINE 256

/* -------------------------
   Utility: read / write stok
   ------------------------- */
int read_stok(const char *filename, int stok[], const char *default_stok_str) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        // Buat file default jika belum ada
        f = fopen(filename, "w");
        if (!f) return 0;
        // Default stok awal
        int def[ITEMS] = {10, 10, 10, 5, 10, 8, 5, 5, 15};
        for (int i=0; i<ITEMS; i++) fprintf(f, "%d\n", def[i]);
        fclose(f);
        // Baca ulang
        f = fopen(filename, "r");
        if (!f) return 0;
    }
    for (int i=0; i<ITEMS; i++) {
        if (fscanf(f, "%d", &stok[i]) != 1) stok[i] = 0;
    }
    fclose(f);
    return 1;
}

int write_stok(const char *filename, int stok[]) {
    FILE *f = fopen(filename, "w");
    if (!f) return 0;
    for (int i=0; i<ITEMS; i++) fprintf(f, "%d\n", stok[i]);
    fclose(f);
    return 1;
}

/* -------------------------
   History helpers
   ------------------------- */
int count_history_lines(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) return 0;
    int lines = 0;
    char buf[MAX_LINE];
    while (fgets(buf, sizeof(buf), f)) {
        if (strlen(buf) > 1) lines++;
    }
    fclose(f);
    return lines;
}

void append_history(const char *filename, const char *line) {
    FILE *f = fopen(filename, "a");
    if (!f) return;
    fprintf(f, "%s\n", line);
    fclose(f);
}

/* -------------------------
   Utility: format rupiah (10000 -> "10.000")
   ------------------------- */
void formatRupiahInt(int num, char *out, size_t outsz) {
    if (outsz == 0) return;
    if (num < 0) {
        char tmp[64];
        formatRupiahInt(-num, tmp, sizeof(tmp));
        snprintf(out, outsz, "-%s", tmp);
        return;
    }
    char buf[64];
    sprintf(buf, "%d", num);
    int len = (int)strlen(buf);
    char rev[64];
    int ri = 0, cnt = 0;
    for (int i = len - 1; i >= 0; --i) {
        rev[ri++] = buf[i];
        cnt++;
        if (cnt == 3 && i != 0) {
            rev[ri++] = '.';
            cnt = 0;
        }
    }
    rev[ri] = '\0';
    int j = 0;
    for (int i = ri - 1; i >= 0 && j < (int)outsz - 1; --i) {
        out[j++] = rev[i];
    }
    out[j] = '\0';
}

void formatRupiahFloat(float numf, char *out, size_t outsz) {
    int val = (int)(numf + 0.5f);
    formatRupiahInt(val, out, outsz);
}

/* ==========================
   MAIN PROGRAM
   ========================== */
int main() {
    // Data Barang Tetap
    const char *names[ITEMS] = {
        "Buku Tulis", "Buku Cerpen", "Buku Pengetahuan Umum", "Novel",
        "Buku Fiksi Ilmiah", "Komik", "Ensiklopedia", "Atlas", "Majalah"
    };
    int prices[ITEMS] = {5000, 25000, 40000, 45000, 50000, 30000, 75000, 60000, 20000};
    const char *statusBarang[ITEMS] = {"Baru","Baru","Baru","Bekas","Baru","Bekas","Baru","Baru","Bekas"};

    // Warna ANSI
    const char *RED = "\033[1;31m";
    const char *BLUE = "\033[1;34m";
    const char *GREEN = "\033[1;32m";
    const char *YELLOW = "\033[1;33m";
    const char *RESET = "\033[0m";

    /* --- LOGIN SYSTEM --- */
    char username[50], password[50];
    int attempts = 0;
    int logged = 0;

    while (attempts < 3) {
        printf("%s--- LOGIN KASIR --- %s\n", BLUE, RESET);
        printf("Username: ");
        if (!fgets(username, sizeof(username), stdin)) return 1;
        username[strcspn(username, "\n")] = 0;
        printf("Password: ");
        if (!fgets(password, sizeof(password), stdin)) return 1;
        password[strcspn(password, "\n")] = 0;

        if (strcmp(username, "kasir") == 0 && strcmp(password, "1234") == 0) {
            printf("%sLogin sukses!%s\n\n", GREEN, RESET);
            logged = 1;
            break;
        } else {
            attempts++;
            printf("%sLogin salah! Percobaan %d/3%s\n\n", RED, attempts, RESET);
        }
    }
    if (!logged) {
        printf("%sGagal Login. Program Keluar.%s\n", RED, RESET);
        return 0;
    }

    /* --- BACA STOK DARI FILE --- */
    int stok[ITEMS];
    if (!read_stok("stok.txt", stok, NULL)) {
        printf("%sGagal memproses file stok.txt%s\n", RED, RESET);
        return 1;
    }

    /* --- INPUT PELANGGAN --- */
    char nama[80], sales[80];
    int kunjungan;

    printf("%s=========================================%s\n", BLUE, RESET);
    printf("  Selamat Datang di Toko BUKU KELOMPOK 7\n");
    printf("%s=========================================%s\n\n", BLUE, RESET);

    printf("Masukkan nama pelanggan    : ");
    if (!fgets(nama, sizeof(nama), stdin)) return 1;
    nama[strcspn(nama, "\n")] = 0;

    printf("Masukkan nama sales/penjual: ");
    if (!fgets(sales, sizeof(sales), stdin)) return 1;
    sales[strcspn(sales, "\n")] = 0;

    while (1) {
        char tmp[32];
        printf("Sudah berapa kali Anda berbelanja di sini? ");
        if (!fgets(tmp, sizeof(tmp), stdin)) return 1;
        if (sscanf(tmp, "%d", &kunjungan) != 1) {
            printf("%sInput harus angka!%s\n", RED, RESET); continue;
        }
        if (kunjungan < 0) {
            printf("%sTidak boleh negatif!%s\n", RED, RESET); continue;
        }
        break;
    }
    int pelangganLama = (kunjungan > 3) ? 1 : 0;
    if (pelangganLama) printf("%sStatus: Pelanggan Lama (Bonus Diskon)%s\n\n", GREEN, RESET);
    else printf("%sStatus: Pelanggan Baru%s\n\n", YELLOW, RESET);

    /* --- PROSES BELANJA --- */
    int bought_ids[100], bought_qty[100];
    int bought_count = 0;
    int pilihan = 0, jumlah = 0;
    char lanjut = 'y';
    float total = 0.0f;
    float total_item_discounts = 0.0f;

    while (lanjut == 'y' || lanjut == 'Y') {
        // Tampilkan Tabel
        printf("\n%s-------------------------------------------------------------------------------%s\n", BLUE, RESET);
        printf("| %-2s | %-30s | %-13s | %-6s | %-10s |\n", "No", "Nama Buku", "Harga", "Stok", "Status");
        printf("-------------------------------------------------------------------------------\n");
        for (int i = 0; i < ITEMS; i++) {
            char hargaFmt[32];
            formatRupiahInt(prices[i], hargaFmt, sizeof(hargaFmt));
            printf("| %-2d | %-30s | %sRp %-8s%s | %-6d | %-10s |\n",
                   i+1, names[i], RED, hargaFmt, RESET, stok[i], statusBarang[i]);
        }
        printf("-------------------------------------------------------------------------------\n");

        // Input Pilihan Barang
        while (1) {
            char buf[64];
            printf("\nMasukkan nomor pilihan barang (1-%d): ", ITEMS);
            if (!fgets(buf, sizeof(buf), stdin)) return 1;
            if (sscanf(buf, "%d", &pilihan) != 1) {
                printf("%sInput harus angka!%s\n", RED, RESET); continue;
            }
            if (pilihan < 1 || pilihan > ITEMS) {
                printf("%sPilihan tidak tersedia!%s\n", RED, RESET); continue;
            }
            if (stok[pilihan-1] == 0) {
                printf("%sStok habis.%s\n", RED, RESET);
                // Opsi batal atau ganti
                while (1) {
                    char ops[8];
                    printf("Ganti barang (g) atau batalkan transaksi (b)? ");
                    if (!fgets(ops, sizeof(ops), stdin)) return 1;
                    if (ops[0]=='g' || ops[0]=='G') break;
                    if (ops[0]=='b' || ops[0]=='B') {
                        // FIX: Jangan update stok ke file jika batal!
                        printf("Transaksi dibatalkan. Stok tidak berubah.\n");
                        return 0;
                    }
                    printf("Input salah (g/b)!\n");
                }
                continue;
            }
            break;
        }

        // Input Jumlah
        while (1) {
            char buf[64];
            printf("Masukkan jumlah yang dibeli : ");
            if (!fgets(buf, sizeof(buf), stdin)) return 1;
            if (sscanf(buf, "%d", &jumlah) != 1) {
                printf("%sInput harus angka!%s\n", RED, RESET); continue;
            }
            if (jumlah <= 0) {
                printf("%sMinimal beli 1!%s\n", RED, RESET); continue;
            }
            if (jumlah > stok[pilihan-1]) {
                printf("%sStok tidak cukup! Sisa: %d%s\n", RED, stok[pilihan-1], RESET); continue;
            }
            break;
        }

        // Proses Masuk Keranjang (Update stok di RAM saja dulu)
        int idx = pilihan - 1;
        float item_total = prices[idx] * (float)jumlah;
        float item_discount = 0.0f;

        // Diskon barang bekas
        if (strcmp(statusBarang[idx], "Bekas") == 0) {
            item_discount = item_total * 0.10f;
            total_item_discounts += item_discount;
        }
        total += item_total - item_discount;

        bought_ids[bought_count] = idx;
        bought_qty[bought_count] = jumlah;
        bought_count++;

        // Kurangi stok di memori (belum di file)
        stok[idx] -= jumlah;

        // Loop Tambah Barang
        while (1) {
            char buf[8];
            printf("Ingin tambah barang lain? (y/n): ");
            if (!fgets(buf, sizeof(buf), stdin)) return 1;
            if (buf[0]=='y' || buf[0]=='Y' || buf[0]=='n' || buf[0]=='N') {
                lanjut = buf[0];
                break;
            }
        }
    }

    /* --- HITUNG TOTAL AKHIR & DISKON TIER --- */
    float diskon_tier = 0.0f;
    if (total >= 500000.0f) diskon_tier = 0.15f;
    else if (total >= 200000.0f) diskon_tier = 0.10f;
    else if (total >= 100000.0f) diskon_tier = 0.05f;

    if (pelangganLama) {
        printf("\n%s[INFO] Pelanggan lama: Tambahan diskon 5%%%s\n", GREEN, RESET);
        diskon_tier += 0.05f;
    }

    float diskon_total_amt = total * diskon_tier;
    float subtotal_after_all_discounts = total - diskon_total_amt;

    /* --- TAMPILKAN RINGKASAN SEMENTARA --- */
    printf("\n-----------------------------------------\n");
    printf("Ringkasan Belanja:\n");
    char totalFmt[32], dTAmt[32], subtotalFmt[32];
    formatRupiahFloat(total, totalFmt, sizeof(totalFmt));
    formatRupiahFloat(diskon_total_amt, dTAmt, sizeof(dTAmt));
    formatRupiahFloat(subtotal_after_all_discounts, subtotalFmt, sizeof(subtotalFmt));

    printf("Total Item (stlh diskon barang): %sRp %s%s\n", RED, totalFmt, RESET);
    if (diskon_tier > 0.0f) {
        printf("Diskon Tambahan (%.0f%%)       : -%sRp %s%s\n", diskon_tier*100.0f, RED, dTAmt, RESET);
    }
    printf("Subtotal Sementara             : %sRp %s%s\n", RED, subtotalFmt, RESET);

    /* --- METODE PEMBAYARAN --- */
    char metode[32];
    float adminFee = 0.0f;
    while (1) {
        printf("\nPilih metode pembayaran (cash/qris/debit): ");
        if (!fgets(metode, sizeof(metode), stdin)) return 1;
        metode[strcspn(metode, "\n")] = 0;

        if (strcmp(metode, "cash") == 0) {
            if (subtotal_after_all_discounts > 100000.0f) {
                printf("%sBonus Cash: Potongan Rp 5.000%s\n", GREEN, RESET);
                subtotal_after_all_discounts -= 5000.0f;
            }
            break;
        } else if (strcmp(metode, "qris") == 0) {
            adminFee = subtotal_after_all_discounts * 0.01f;
            printf("%sAdmin QRIS (1%%): Rp %.0f%s\n", YELLOW, adminFee, RESET);
            subtotal_after_all_discounts += adminFee;
            break;
        } else if (strcmp(metode, "debit") == 0) {
            adminFee = 2500.0f;
            printf("%sAdmin Debit: Rp 2.500%s\n", YELLOW, RESET);
            subtotal_after_all_discounts += adminFee;
            break;
        } else {
            printf("%sMetode salah!%s\n", RED, RESET);
        }
    }

    /* --- PEMBAYARAN --- */
    int bayar_int = 0, kembalian_int = 0;
    while (1) {
        char tmp[64];
        char finalFmt[32];
        formatRupiahFloat(subtotal_after_all_discounts, finalFmt, sizeof(finalFmt));

        printf("\nTotal yang harus dibayar: %sRp %s%s\n", RED, finalFmt, RESET);
        printf("Masukkan uang Anda      : Rp ");

        if (!fgets(tmp, sizeof(tmp), stdin)) return 1;
        if (sscanf(tmp, "%d", &bayar_int) != 1) {
            printf("Input harus angka!\n"); continue;
        }

        if (bayar_int < 0) continue;

        kembalian_int = (int)(bayar_int - (int)(subtotal_after_all_discounts + 0.5f));

        if (kembalian_int < 0) {
            printf("%sUang kurang Rp %d!%s\n", RED, -kembalian_int, RESET);
            char opsi[8];
            while (1) {
                printf("Coba lagi (y) atau batalkan transaksi (b)? ");
                if (!fgets(opsi, sizeof(opsi), stdin)) return 1;
                if (opsi[0]=='b' || opsi[0]=='B') {
                    // FIX: Batal di sini juga JANGAN update file stok
                    printf("Transaksi Dibatalkan. Uang dikembalikan.\n");
                    return 0;
                } else if (opsi[0]=='y' || opsi[0]=='Y') {
                    break;
                }
            }
        } else {
            // Uang cukup
            break;
        }
    }

    /* --- UPDATE DATA (HANYA JIKA BAYAR SUKSES) --- */
    // 1. Generate ID
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char tanggal_id[32];
    strftime(tanggal_id, sizeof(tanggal_id), "%Y%m%d", t);
    int hist_lines = count_history_lines("history.txt");
    char trx_id[64];
    snprintf(trx_id, sizeof(trx_id), "TRX-%s-%04d", tanggal_id, hist_lines + 1);
    char waktu[64];
    strftime(waktu, sizeof(waktu), "%d-%m-%Y %H:%M:%S", t);

    // 2. Simpan stok baru ke file
    write_stok("stok.txt", stok);

    // 3. Simpan log simple ke history.txt
    char items_str[512] = "";
    for (int i=0; i<bought_count; i++) {
        char temp[64];
        snprintf(temp, sizeof(temp), "%dx %s, ", bought_qty[i], names[bought_ids[i]]);
        strncat(items_str, temp, sizeof(items_str)-strlen(items_str)-1);
    }
    // Hapus koma terakhir
    int L = (int)strlen(items_str);
    if (L >= 2 && items_str[L-2]==',') items_str[L-2]=0;

    char hist_line[1024];
    snprintf(hist_line, sizeof(hist_line),
             "%s | %s | %s | Rp %.0f | %s | %s",
             trx_id, nama, items_str, subtotal_after_all_discounts, metode, waktu);
    append_history("history.txt", hist_line);


    /* --- CETAK STRUK LAYAR --- */
    char totalBeforeFmt[32], itemDiscFmt[32], discTierFmt[32], adminFmt[32], subtotalAfterFmt[32];
    char bayarFmt[32], kembaliFmt[32];

    formatRupiahFloat(total + total_item_discounts, totalBeforeFmt, sizeof(totalBeforeFmt));
    formatRupiahFloat(total_item_discounts, itemDiscFmt, sizeof(itemDiscFmt));
    formatRupiahFloat(diskon_total_amt, discTierFmt, sizeof(discTierFmt));
    formatRupiahFloat(adminFee, adminFmt, sizeof(adminFmt));
    formatRupiahFloat(subtotal_after_all_discounts, subtotalAfterFmt, sizeof(subtotalAfterFmt));
    formatRupiahInt(bayar_int, bayarFmt, sizeof(bayarFmt));
    formatRupiahInt(kembalian_int, kembaliFmt, sizeof(kembaliFmt));

    printf("\n%s======================================================================%s\n", BLUE, RESET);
    printf("                        STRUK PEMBAYARAN TOKO BUKU KELOMPOK 7\n");
    printf("======================================================================\n\n");
    printf("ID Transaksi   : %s\n", trx_id);
    printf("Waktu Transaksi: %s\n", waktu);
    printf("Nama Pelanggan : %s\n", nama);
    printf("Sales/Penjual  : %s\n", sales);
    printf("Metode Bayar   : %s\n\n", metode);
    printf("----------------------------------------------------------------------\n");
    printf("RINCIAN PEMBELIAN:\n");

    for (int i=0; i<bought_count; i++) {
        int id = bought_ids[i];
        int q = bought_qty[i];
        float itotal = prices[id] * (float)q;
        float idisc = (strcmp(statusBarang[id], "Bekas") == 0) ? itotal*0.10f : 0.0f;
        char pFmt[32], tFmt[32], dFmt[32];
        formatRupiahInt(prices[id], pFmt, sizeof(pFmt));
        formatRupiahFloat(itotal - idisc, tFmt, sizeof(tFmt));

        printf("%2d x %-30s @ Rp %-8s = Rp %-12s", q, names[id], pFmt, tFmt);
        if (idisc > 0.0f) {
            formatRupiahFloat(idisc, dFmt, sizeof(dFmt));
            printf(" (Disc: Rp %s)", dFmt);
        }
        printf("\n");
    }
    printf("----------------------------------------------------------------------\n");
    printf("Total Awal          : %sRp %s%s\n", RED, totalBeforeFmt, RESET);
    if (total_item_discounts > 0.0f) printf("Diskon Barang Bekas : -%sRp %s%s\n", RED, itemDiscFmt, RESET);
    if (diskon_tier > 0.0f)          printf("Diskon Tambahan     : -%sRp %s%s\n", RED, discTierFmt, RESET);
    if (adminFee > 0.0f)             printf("Biaya Admin         : %sRp %s%s\n", RED, adminFmt, RESET);
    printf("GRAND TOTAL         : %sRp %s%s\n", RED, subtotalAfterFmt, RESET);
    printf("Tunai               : %sRp %s%s\n", RED, bayarFmt, RESET);
    printf("Kembali             : %sRp %s%s\n", RED, kembaliFmt, RESET);
    printf("\n======================================================================\n");
    printf("Terima kasih %s!\n", nama);
    printf("======================================================================%s\n", RESET);


    /* --- SIMPAN STRUK LENGKAP KE FILE (APPEND) --- */
    FILE *fstruk = fopen("riwayat_struk_lengkap.txt", "a");
    if (fstruk) {
        fprintf(fstruk, "======================================================================\n");
        fprintf(fstruk, "                        STRUK PEMBAYARAN TOKO BUKU KELOMPOK 7\n");
        fprintf(fstruk, "======================================================================\n\n");
        fprintf(fstruk, "ID Transaksi   : %s\n", trx_id);
        fprintf(fstruk, "Waktu Transaksi: %s\n", waktu);
        fprintf(fstruk, "Nama Pelanggan : %s\n", nama);
        fprintf(fstruk, "Sales/Penjual  : %s\n", sales);
        fprintf(fstruk, "Metode Bayar   : %s\n\n", metode);
        fprintf(fstruk, "----------------------------------------------------------------------\n");
        fprintf(fstruk, "RINCIAN PEMBELIAN:\n");

        for (int i=0; i<bought_count; i++) {
            int id = bought_ids[i];
            int q = bought_qty[i];
            float itotal = prices[id] * (float)q;
            float idisc = (strcmp(statusBarang[id], "Bekas") == 0) ? itotal*0.10f : 0.0f;
            char pFmt[32], tFmt[32], dFmt[32];
            formatRupiahInt(prices[id], pFmt, sizeof(pFmt));
            formatRupiahFloat(itotal - idisc, tFmt, sizeof(tFmt));

            fprintf(fstruk, "%2d x %-30s @ Rp %-8s = Rp %-12s", q, names[id], pFmt, tFmt);
            if (idisc > 0.0f) {
                formatRupiahFloat(idisc, dFmt, sizeof(dFmt));
                fprintf(fstruk, " (Disc: Rp %s)", dFmt);
            }
            fprintf(fstruk, "\n");
        }
        fprintf(fstruk, "----------------------------------------------------------------------\n");
        fprintf(fstruk, "Total Awal          : Rp %s\n", totalBeforeFmt);
        if (total_item_discounts > 0.0f) fprintf(fstruk, "Diskon Barang Bekas : -Rp %s\n", itemDiscFmt);
        if (diskon_tier > 0.0f)          fprintf(fstruk, "Diskon Tambahan     : -Rp %s\n", discTierFmt);
        if (adminFee > 0.0f)             fprintf(fstruk, "Biaya Admin         : Rp %s\n", adminFmt);
        fprintf(fstruk, "GRAND TOTAL         : Rp %s\n", subtotalAfterFmt);
        fprintf(fstruk, "Tunai               : Rp %s\n", bayarFmt);
        fprintf(fstruk, "Kembali             : Rp %s\n", kembaliFmt);
        fprintf(fstruk, "\n======================================================================\n");
        fprintf(fstruk, "Terima kasih %s!\n\n\n", nama);
        fclose(fstruk);
        printf("\n[INFO] Struk tersimpan di 'riwayat_struk_lengkap.txt'\n");
    }


    return 0;
}
