//TA_StrukturData_Array
//Rizky Alif Ichwanto
//222212857
//2KS2
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CONTACTS 100
#define FILENAME "kontak.dat"  // Nama berkas untuk menyimpan data kontak

// ANSI escape codes untuk warna teks
#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_RED     "\x1b[31m"

int wildcardMatch(const char *pattern, const char *string) {
    while (*pattern) {
        if (*pattern == '*') {
            while (*pattern == '*') {
                pattern++;
            }

            if (*pattern == '\0') {
                return 1;
            }

            while (*string && strncmp(string, pattern, 1) != 0) {
                string++;
            }
        } else {
            if (*string != *pattern) {
                return 0;
            }

            string++;
        }

        pattern++;
    }

    return *string == '\0';
}

struct Contact {
    char nama[50];
    char jenisKelamin[10];
    char tanggalLahir[15];
    int umur;
    char nomorTelepon[15];
    char email[50];
};

void tampilkanMenu() {
    printf("\n");
    printf(ANSI_COLOR_CYAN "||============================================================||\n" ANSI_COLOR_RESET);
    printf(ANSI_COLOR_CYAN "||" ANSI_COLOR_RESET "                    " ANSI_COLOR_CYAN "Menu Kontak" ANSI_COLOR_RESET "                             ||\n");
    printf(ANSI_COLOR_CYAN "||============================================================||\n" ANSI_COLOR_RESET);
    printf(ANSI_COLOR_GREEN"|| (1) Tampilkan Daftar Kontak                                ||\n");
    printf("|| (2) Pencarian Kontak                                       ||\n");
    printf("|| (3) Tambah Kontak Baru                                     ||\n");
    printf("|| (4) Hapus Kontak                                           ||\n");
    printf("|| (5) Edit Kontak                                            ||\n");
    printf("|| (6) Urutkan Kontak                                         ||\n");
    printf(ANSI_COLOR_RED "|| (0) Keluar                                                 ||\n" ANSI_COLOR_RESET);
    printf(ANSI_COLOR_CYAN "================================================================\n" ANSI_COLOR_RESET);
}

void inputKontak(struct Contact *kontak) {
    char buffer[100];

    printf("Masukkan Nama Lengkap: ");
    fgets(kontak->nama, sizeof(kontak->nama), stdin);
    strtok(kontak->nama, "\n");

    printf("Masukkan Jenis Kelamin: ");
    fgets(kontak->jenisKelamin, sizeof(kontak->jenisKelamin), stdin);
    strtok(kontak->jenisKelamin, "\n");

    printf("Masukkan Tanggal Lahir (format: DD/MM/YYYY): ");
    fgets(buffer, sizeof(buffer), stdin);
    strtok(buffer, "\n");
    strcpy(kontak->tanggalLahir, buffer);

    printf("Masukkan Umur: ");
    fgets(buffer, sizeof(buffer), stdin);
    sscanf(buffer, "%d", &kontak->umur);

    printf("Masukkan Nomor Telepon: ");
    fgets(kontak->nomorTelepon, sizeof(kontak->nomorTelepon), stdin);
    strtok(kontak->nomorTelepon, "\n");

    printf("Masukkan Alamat Email: ");
    fgets(kontak->email, sizeof(kontak->email), stdin);
    strtok(kontak->email, "\n");
}

void tampilkanKontak(const struct Contact *kontak) {
    printf("Nama: %s\n", kontak->nama);
    printf("Jenis Kelamin: %s\n", kontak->jenisKelamin);
    printf("Tanggal Lahir: %s\n", kontak->tanggalLahir);
    printf("Umur: %d\n", kontak->umur);
    printf("Nomor Telepon: %s\n", kontak->nomorTelepon);
    printf("Email: %s\n", kontak->email);
}

void tampilkanDaftarKontak(const struct Contact *daftarKontak, int jumlahKontak) {
    // Menampilkan header tabel
    printf("| %-30s | %-15s | %-15s | %-5s | %-15s | %-30s |\n",
           "Nama", "Jenis Kelamin", "Tanggal Lahir", "Umur", "Nomor Telepon", "Email");
    printf("| %s | %s | %s | %s | %s | %s |\n",
           "----------------------------", "---------------", "---------------",
           "------", "---------------", "----------------------------");

    // Menampilkan setiap kontak dalam bentuk tabel
    for (int i = 0; i < jumlahKontak; i++) {
        printf("| %-30s | %-15s | %-15s | %-5d | %-15s | %-30s |\n",
               daftarKontak[i].nama, daftarKontak[i].jenisKelamin,
               daftarKontak[i].tanggalLahir, daftarKontak[i].umur,
               daftarKontak[i].nomorTelepon, daftarKontak[i].email);
    }
    printf("| %s | %s | %s | %s | %s | %s |\n",
           "----------------------------", "---------------", "---------------",
           "------", "---------------", "----------------------------");
}

void tambahkanKontak(struct Contact *daftarKontak, int *jumlahKontak, const struct Contact *kontakBaru) {
    // Memeriksa apakah kontak dengan nama atau nomor telepon yang sama sudah ada
    for (int i = 0; i < *jumlahKontak; i++) {
        if (strcmp(kontakBaru->nama, daftarKontak[i].nama) == 0 ||
            strcmp(kontakBaru->nomorTelepon, daftarKontak[i].nomorTelepon) == 0) {
            printf("Kontak dengan nama atau nomor telepon yang sama sudah ada. Tidak dapat menambahkan kontak.\n");
            return;
        }
    }
    // Menambahkan kontak jika tidak ada masalah
    if (*jumlahKontak < MAX_CONTACTS) {
        daftarKontak[*jumlahKontak] = *kontakBaru;
        (*jumlahKontak)++;
        printf("Kontak berhasil ditambahkan!\n");
    } else {
        printf("Buku kontak penuh. Tidak dapat menambahkan lebih banyak kontak.\n");
    }
}


void pencarianKontakWildcard(const struct Contact *daftarKontak, int jumlahKontak, const char *atribut) {
    int ditemukan = 0;

    for (int i = 0; i < jumlahKontak; i++) {
        if (wildcardMatch(atribut, daftarKontak[i].nama) ||
            wildcardMatch(atribut, daftarKontak[i].nomorTelepon)) {
            tampilkanKontak(&daftarKontak[i]);
            ditemukan = 1;
        }
    }

    if (!ditemukan) {
        printf("Kontak dengan atribut '%s' tidak ditemukan.\n", atribut);
    }
}

void hapusKontak(struct Contact *daftarKontak, int *jumlahKontak, const char *atribut) {
    int ditemukan = 0;

    for (int i = 0; i < *jumlahKontak; i++) {
        // Mencari kontak berdasarkan atribut
        if (strcmp(atribut, daftarKontak[i].nama) == 0 ||
            strcmp(atribut, daftarKontak[i].nomorTelepon) == 0) {
            // Menggeser elemen-elemen array untuk menghapus kontak
            for (int j = i; j < *jumlahKontak - 1; j++) {
                daftarKontak[j] = daftarKontak[j + 1];
            }

            (*jumlahKontak)--;
            ditemukan = 1;
            printf("Kontak berhasil dihapus!\n");
            break;  // Keluar dari loop setelah menghapus kontak
        }
    }

    if (!ditemukan) {
        printf("Kontak dengan atribut '%s' tidak ditemukan.\n", atribut);
    }
}

void editKontak(struct Contact *daftarKontak, int jumlahKontak, const char *atribut) {
    int ditemukan = 0;

    for (int i = 0; i < jumlahKontak; i++) {
        // Mencari kontak berdasarkan atribut "nama" atau "nomorTelepon"
        if ((strcmp(atribut, daftarKontak[i].nama) == 0) ||
            (strcmp(atribut, daftarKontak[i].nomorTelepon) == 0)) {
            // Menampilkan informasi kontak sebelum diedit
            printf("Informasi Kontak Sebelum Diedit:\n");
            tampilkanKontak(&daftarKontak[i]);

            // Mengedit kontak
            printf("\nMasukkan informasi baru untuk kontak:\n");
            inputKontak(&daftarKontak[i]);
            ditemukan = 1;
            printf("Kontak berhasil diedit!\n");
            break;  // Keluar dari loop setelah mengedit kontak
        }
    }

    if (!ditemukan) {
        printf("Kontak dengan atribut '%s' tidak ditemukan.\n", atribut);
    }
}

int compareNama(const void *a, const void *b) {
    return strcmp(((struct Contact *)a)->nama, ((struct Contact *)b)->nama);
}

int compareUmur(const void *a, const void *b) {
    return ((struct Contact *)a)->umur - ((struct Contact *)b)->umur;
}

void urutkanKontak(struct Contact *daftarKontak, int jumlahKontak, const char *atribut) {
    if (strcmp(atribut, "umur") == 0) {
        qsort(daftarKontak, jumlahKontak, sizeof(struct Contact), compareUmur);
    } else if (strcmp(atribut, "nama") == 0) {
        qsort(daftarKontak, jumlahKontak, sizeof(struct Contact), compareNama);
    }
    // Tambahkan kondisi pengurutan berdasarkan atribut lainnya sesuai kebutuhan
}

void simpanKontakKeBerkas(const struct Contact *daftarKontak, int jumlahKontak) {
    FILE *berkas = fopen(FILENAME, "wb");
    
    if (berkas != NULL) {
        fwrite(daftarKontak, sizeof(struct Contact), jumlahKontak, berkas);
        fclose(berkas);
    } else {
        printf("Gagal membuka berkas untuk penyimpanan.\n");
    }
}

void bacaKontakDariBerkas(struct Contact *daftarKontak, int *jumlahKontak) {
    FILE *berkas = fopen(FILENAME, "rb");

    if (berkas != NULL) {
        *jumlahKontak = fread(daftarKontak, sizeof(struct Contact), MAX_CONTACTS, berkas);
        fclose(berkas);
    } else {
        printf("Gagal membuka berkas untuk pembacaan.\n");
    }
}

int main() {
    struct Contact daftarKontak[MAX_CONTACTS];
    int jumlahKontak = 0;
    int pilihan;
    char atribut[50];

    // Membaca data kontak dari berkas saat program dimulai
    bacaKontakDariBerkas(daftarKontak, &jumlahKontak);

    do {
        system("cls");

        tampilkanMenu();
        printf("Masukkan pilihan Anda: ");
        scanf("%d", &pilihan);
        getchar();  // Membersihkan newline di buffer setelah scanf

        switch (pilihan) {
            case 1:
                system("cls");
                // Menampilkan daftar kontak dalam bentuk tabel
                tampilkanDaftarKontak(daftarKontak, jumlahKontak);
                break;

            case 2:
                // Pencarian kontak dengan wildcard
                printf("Masukkan atribut untuk pencarian (gunakan * sebagai wildcard): ");
                scanf("%s", atribut);
                getchar();  // Membersihkan newline di buffer
                pencarianKontakWildcard(daftarKontak, jumlahKontak, atribut);
                break;

            case 3:
                // Menambah kontak baru
                if (jumlahKontak < MAX_CONTACTS) {
                    struct Contact kontakBaru;
                    inputKontak(&kontakBaru);
                    tambahkanKontak(daftarKontak, &jumlahKontak, &kontakBaru);
                    // Menyimpan kontak ke berkas setelah penambahan kontak baru
                    simpanKontakKeBerkas(daftarKontak, jumlahKontak);
                } else {
                    printf("Buku kontak penuh. Tidak dapat menambahkan lebih banyak kontak.\n");
                }
                break;
            case 4:
                // Menghapus kontak
                printf("Masukkan atribut untuk penghapusan: ");
                scanf("%s", atribut);
                hapusKontak(daftarKontak, &jumlahKontak, atribut);
                break;

            case 5:
                // Mengedit kontak
                printf("Masukkan atribut untuk pengeditan: ");
                scanf("%s", atribut);
                editKontak(daftarKontak, jumlahKontak, atribut);
                break;

            case 6:
                // Mengurutkan kontak
                printf("Masukkan atribut untuk pengurutan: ");
                scanf("%s", atribut);
                urutkanKontak(daftarKontak, jumlahKontak, atribut);
                break;

            case 0:
                // Keluar dari program
                printf("Terima kasih! Program selesai.\n");
                simpanKontakKeBerkas(daftarKontak,jumlahKontak);
                break;

            default:
                printf(ANSI_COLOR_RED "Pilihan tidak valid. Silakan pilih lagi.\n" ANSI_COLOR_RESET);
        }
        // Menunda tampilan hasil untuk beberapa detik sebelum kembali ke menu utama
        printf("\nTunggu sebentar...");
        fflush(stdout);  // Memastikan output ditampilkan segera
        sleep(2);  // Ganti dengan fungsi sleep yang sesuai dengan sistem operasi Anda
    }while (pilihan != 0);
    return 0;
}