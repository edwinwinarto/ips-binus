#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 256

typedef struct {
    double min;
    double max;
    char grade[3];
    double bobot;
} Grade;


Grade grades[] = {
    {90.00, 100.00, "A", 4.00},
    {85.00, 89.99, "A-", 3.67},
    {80.00, 84.99, "B+", 3.33},
    {75.00, 79.99, "B", 3.00},
    {70.00, 74.99, "B-", 2.50},
    {65.00, 69.99, "C", 2.00},
    {50.00, 64.99, "D", 1.00},
    {0.00, 49.99, "E", 0.00}
};

char* tentukan_grade(double skor, double *bobot) {
    for (int i = 0; i < sizeof(grades) / sizeof(grades[0]); i++) {
        if (skor >= grades[i].min && skor <= grades[i].max) {
            *bobot = grades[i].bobot;
            return grades[i].grade;
        }
    }
    *bobot = 0.00;
    return "E"; 
}


double hitung_nilai(char *data) {
    char *token;
    token = strtok(data, ","); 
    token = strtok(NULL, ","); 
    int sks = atoi(token);
    token = strtok(NULL, ","); 
    int jumlah_nilai = atoi(token);

    double total = 0.0;
    double total_persentase = 0.0;

    for (int i = 0; i < jumlah_nilai; i++) {
        token = strtok(NULL, ",");
        double nilai = atof(token);
        token = strtok(NULL, ",");
        double persentase = atof(token) / 100.0;

        total += nilai * persentase;
        total_persentase += persentase * 100;
    }

    if (total_persentase != 100.0) {
        printf("Peringatan: Total persentase tidak 100%%!\n");
    }

    return total;
}

int main() {
    FILE *input = fopen("input.txt", "r");
    FILE *output = fopen("output.txt", "w");
    if (!input || !output) {
        printf("Gagal membuka file!\n");
        return 1;
    }

    char line[MAX_LINE];
    double total_bobot_sks = 0.0;
    double total_sks = 0.0;

    fprintf(output, "Rangkuman nilai :\n");
    while (fgets(line, sizeof(line), input)) {
        char data_copy[MAX_LINE];
        strcpy(data_copy, line);

        char *token;
        token = strtok(line, ",");
        char nama_matkul[50];
        strcpy(nama_matkul, token);

        token = strtok(NULL, ",");
        int sks = atoi(token);

        double nilai_akhir = hitung_nilai(data_copy);
        double bobot;
        char *grade = tentukan_grade(nilai_akhir, &bobot);
        total_bobot_sks += bobot * sks;
        total_sks += sks;

        fprintf(output, "%s = %.2f = %s\n", nama_matkul, nilai_akhir, grade);
    }

    if (total_sks > 0) {
        double ipk = total_bobot_sks / total_sks;
        fprintf(output, "\nIPK = %.2f\n", ipk);
    }

    fclose(input);
    fclose(output);
    printf("Proses selesai! Cek output.txt\n");
    return 0;
}
