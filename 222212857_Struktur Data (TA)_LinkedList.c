//TA_StrukturData_Linkedlist
//Rizky Alif Ichwanto
//222212857
//2KS2
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>


#define MAX_CONTACTS 100
#define FILENAME "kontak.dat"  // Nama berkas untuk menyimpan data kontak

// ANSI escape codes untuk warna teks
#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_WHITE   "\x1b[37m"

struct Contact {
    char nama[50];
    char jenisKelamin[10];
    char tanggalLahir[15];
    int umur;
    char nomorTelepon[15];
    char email[50];
};

struct Node {
    struct Contact data;
    struct Node* next;
};

struct Node* head = NULL;

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

void splitList(struct Node* source, struct Node** frontRef, struct Node** backRef) {
    struct Node* fast;
    struct Node* slow;

    if (source == NULL || source->next == NULL) {
        // Kurang dari dua elemen dalam list, tidak dapat di-split
        *frontRef = source;
        *backRef = NULL;
    } else {
        slow = source;
        fast = source->next;

        // Fast melaju dua kali lebih cepat daripada slow
        while (fast != NULL) {
            fast = fast->next;
            if (fast != NULL) {
                slow = slow->next;
                fast = fast->next;
            }
        }

        // Slow berada di tengah list, split menjadi dua bagian
        *frontRef = source;
        *backRef = slow->next;
        slow->next = NULL;
    }
}

struct Node* mergeLists(struct Node* a, struct Node* b, const char* atribut) {
    struct Node* result = NULL;

    // Base cases
    if (a == NULL) {
        return b;
    } else if (b == NULL) {
        return a;
    }

    // Pilih elemen yang lebih kecil dan rekursif untuk sisa list
    if (strcmp(atribut, "nama") == 0) {
        if (strcmp(a->data.nama, b->data.nama) <= 0) {
            result = a;
            result->next = mergeLists(a->next, b, atribut);
        } else {
            result = b;
            result->next = mergeLists(a, b->next, atribut);
        }
    } else if (strcmp(atribut, "umur") == 0) {
        if (a->data.umur <= b->data.umur) {
            result = a;
            result->next = mergeLists(a->next, b, atribut);
        } else {
            result = b;
            result->next = mergeLists(a, b->next, atribut);
        }
    }
    // Tambahkan kondisi pembanding lainnya sesuai kebutuhan

    return result;
}

// Fungsi merge sort untuk linked list
void mergeSort(struct Node** headRef, const char* atribut) {
    struct Node* head = *headRef;
    struct Node* a;
    struct Node* b;

    // Base case: Jika list kosong atau hanya satu elemen, sudah terurut
    if (head == NULL || head->next == NULL) {
        return;
    }

    // Split list menjadi dua bagian
    splitList(head, &a, &b);

    // Rekursif untuk kedua bagian
    mergeSort(&a, atribut);
    mergeSort(&b, atribut);

    // Gabungkan kedua bagian yang sudah diurutkan
    *headRef = mergeLists(a, b, atribut);
}

int wildcardMatch(const char *pattern, const char *string) {
    while (*pattern) {
        if (*pattern == '*') {
            // Jika bertemu dengan wildcard '*'
            while (*pattern == '*') {
                pattern++;  // Melompati semua wildcard yang berurutan
            }

            // Jika wildcard berada di akhir, itu cocok dengan sisa string
            if (*pattern == '\0') {
                return 1;
            }

            // Mencari substring yang cocok dengan wildcard
            while (*string && strncmp(string, pattern, 1) != 0) {
                string++;
            }
        } else {
            // Karakter tidak cocok, kembali ke karakter berikutnya
            if (*string != *pattern) {
                return 0;
            }

            string++;
        }

        pattern++;
    }

    // Cocok jika kedua string berakhir bersamaan
    return *string == '\0';
}

void inputKontak(struct Contact* kontak) {
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

void tampilkanKontak(const struct Contact* kontak) {
    printf("Nama: %s\n", kontak->nama);
    printf("Jenis Kelamin: %s\n", kontak->jenisKelamin);
    printf("Tanggal Lahir: %s\n", kontak->tanggalLahir);
    printf("Umur: %d\n", kontak->umur);
    printf("Nomor Telepon: %s\n", kontak->nomorTelepon);
    printf("Email: %s\n", kontak->email);
}

void tampilkanDaftarKontakLinkedList(const struct Node* head) {
    const struct Node* current = head;

    // Menampilkan header tabel
    printf("| %-30s | %-15s | %-15s | %-5s | %-15s | %-30s |\n",
           "Nama", "Jenis Kelamin", "Tanggal Lahir", "Umur", "Nomor Telepon", "Email");
    printf("| %s | %s | %s | %s | %s | %s |\n",
           "----------------------------", "---------------", "---------------",
           "------", "---------------", "----------------------------");

    // Menampilkan setiap kontak dalam bentuk tabel
    while (current != NULL) {
        printf("| %-30s | %-15s | %-15s | %-5d | %-15s | %-30s |\n",
               current->data.nama, current->data.jenisKelamin,
               current->data.tanggalLahir, current->data.umur,
               current->data.nomorTelepon, current->data.email);
        current = current->next;
    }

    // Menampilkan footer tabel
    printf("| %s | %s | %s | %s | %s | %s |\n",
           "----------------------------", "---------------", "---------------",
           "------", "---------------", "----------------------------");
}

void tambahkanKontak(struct Node** head, const struct Contact* kontakBaru) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = *kontakBaru;
    newNode->next = NULL;  // Node baru ditambahkan di awal, sehingga tidak memiliki node berikutnya

    // Jika linked list kosong, node baru menjadi head
    if (*head == NULL) {
        *head = newNode;
    } else {
        // Menemukan node terakhir dan menambahkan node baru
        struct Node* last = *head;
        while (last->next != NULL) {
            last = last->next;
        }
        last->next = newNode;
    }

    printf("Kontak berhasil ditambahkan!\n");
}

void pencarianKontakWildcard(const struct Node* head, const char* atribut) {
    const struct Node* current = head;
    int ditemukan = 0;

    while (current != NULL) {
        // Menggunakan wildcardMatch untuk mencocokkan pola dengan atribut
        if (wildcardMatch(atribut, current->data.nama) ||
            wildcardMatch(atribut, current->data.nomorTelepon)) {
            tampilkanKontak(&current->data);
            ditemukan = 1;
        }

        current = current->next;
    }

    if (!ditemukan) {
        printf("Kontak dengan atribut '%s' tidak ditemukan.\n", atribut);
    }
}

void hapusKontak(struct Node** head, const char* atribut) {
    struct Node* current = *head;
    struct Node* prev = NULL;

    while (current != NULL) {
        // Mencari node berdasarkan atribut
        if (strcmp(atribut, current->data.nama) == 0 ||
            strcmp(atribut, current->data.nomorTelepon) == 0) {
            // Menghapus node
            if (prev == NULL) {
                *head = current->next;
            } else {
                prev->next = current->next;
            }

            free(current);
            printf("Kontak berhasil dihapus!\n");
            return;
        }

        prev = current;
        current = current->next;
    }

    printf("Kontak dengan atribut '%s' tidak ditemukan.\n", atribut);
}

void editKontak(struct Node* head, const char* atribut) {
    struct Node* current = head;

    while (current != NULL) {
        // Mencari node berdasarkan atribut
        if (strcmp(atribut, current->data.nama) == 0 ||
            strcmp(atribut, current->data.nomorTelepon) == 0) {
            // Mengedit node
            printf("Masukkan informasi baru untuk kontak:\n");
            inputKontak(&current->data);
            printf("Kontak berhasil diedit!\n");
            return;
        }

        current = current->next;
    }

    printf("Kontak dengan atribut '%s' tidak ditemukan.\n", atribut);
}

void urutkanKontakLinkedList(struct Node** head, const char* atribut) {
    if (strcmp(atribut, "umur") == 0) {
        // Implementasi merge sort untuk linked list berdasarkan umur
        mergeSort(head, "umur");
    } else if (strcmp(atribut, "nama") == 0) {
        // Implementasi merge sort untuk linked list berdasarkan nama
        mergeSort(head, "nama");
    }
    // Tambahkan kondisi pengurutan berdasarkan atribut lainnya sesuai kebutuhan
}

void simpanKontakKeFile(const char* filename) {
    FILE* file = fopen(filename, "wb");

    if (file == NULL) {
        perror("Gagal membuka file");
        return;
    }

    struct Node* current = head;

    while (current != NULL) {
        fwrite(&current->data, sizeof(struct Contact), 1, file);
        current = current->next;
    }

    fclose(file);
}

void bacaKontakDariFile(const char* filename) {
    FILE* file = fopen(filename, "rb");

    if (file == NULL) {
        perror("Gagal membuka file");
        return;
    }

    struct Contact kontak;

    while (fread(&kontak, sizeof(struct Contact), 1, file) == 1) {
        struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
        newNode->data = kontak;
        newNode->next = head;
        head = newNode;
    }

    fclose(file);
}

int kontakSudahAda(const struct Node* head, const struct Contact* kontakBaru) {
    const struct Node* current = head;

    while (current != NULL) {
        // Membandingkan nama dan nomor telepon untuk memeriksa apakah kontak sudah ada
        if (strcmp(kontakBaru->nama, current->data.nama) == 0 ||
            strcmp(kontakBaru->nomorTelepon, current->data.nomorTelepon) == 0) {
            return 1;  // Kontak sudah ada
        }

        current = current->next;
    }

    return 0;  // Kontak belum ada
}

int main() {
    // Menambahkan penangan sinyal untuk SIGINT
    int pilihan;
    char atribut[50];

    // Pada saat program dimulai, baca kontak dari file (jika ada)
    bacaKontakDariFile("kontak.dat");

    do {
        system("cls");

        tampilkanMenu();
        printf(ANSI_COLOR_YELLOW "Masukkan pilihan Anda: " ANSI_COLOR_RESET);
        scanf("%d", &pilihan);
        getchar();  // Membersihkan newline di buffer setelah scanf

        switch (pilihan) {
            case 1:
                system("cls");
                // Menampilkan daftar kontak dalam bentuk tabel
                tampilkanDaftarKontakLinkedList(head);
                break;

            case 2:
                // Pencarian kontak dengan wildcard
                printf("Masukkan atribut untuk pencarian (gunakan * sebagai wildcard): ");
                scanf("%s", atribut);
                getchar();  // Membersihkan newline di buffer
                pencarianKontakWildcard(head, atribut);
                break;

            case 3:
                // Menambah kontak baru
                {
                    struct Contact kontakBaru;
                    inputKontak(&kontakBaru);

                    // Memeriksa apakah kontak sudah ada sebelum menambahkannya
                    if (!kontakSudahAda(head, &kontakBaru)) {
                        tambahkanKontak(&head, &kontakBaru);

                        // Setelah menambahkan kontak, simpan ke file
                        simpanKontakKeFile("kontak.dat");
                    } else {
                        printf("Kontak dengan nama atau nomor telepon yang sama sudah ada.\n");
                    }
                }
                break;

            case 4:
                // Mengedit kontak
                printf("Masukkan atribut untuk pengeditan: ");
                scanf("%s", atribut);
                getchar();  // Membersihkan newline di buffer
                editKontak(head, atribut);
                break;

            case 5:
                // Mengedit kontak
                printf("Masukkan atribut untuk pengeditan: ");
                scanf("%s", atribut);
                editKontak(head, atribut);
                break;

            case 6:
                // Mengurutkan kontak
                printf("Masukkan atribut untuk pengurutan: ");
                scanf("%s", atribut);
                urutkanKontakLinkedList(&head, atribut);
                break;

            case 0:
                // Keluar dari program
                printf("Terima kasih! Program selesai.\n");
                break;

            default:
                printf(ANSI_COLOR_RED "Pilihan tidak valid. Silakan pilih lagi.\n" ANSI_COLOR_RESET);
        }

        // Menunda tampilan hasil untuk beberapa detik sebelum kembali ke menu utama
        printf("\nTunggu sebentar...");
        fflush(stdout);  // Memastikan output ditampilkan segera
        sleep(2);  // Ganti dengan fungsi sleep yang sesuai dengan sistem operasi Anda
    } while (pilihan != 0);

    // Membebaskan memori linked list sebelum keluar
    struct Node* current = head;
    while (current != NULL) {
        struct Node* next = current->next;
        free(current);
        current = next;
    }

    return 0;
}