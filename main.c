#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#define CLEAR "cls"
#else
#include <unistd.h>
#define CLEAR "clear"
#endif

#define MAX 100

typedef struct {
    int id;
    char tip[20];
    float pret;
    char facilitati[100];
} Camera;

typedef struct {
    int idCamera;
    int persoane;
    char startDate[11];
    char endDate[11];
} Rezervare;

Camera camere[MAX];
int nrCamere = 0;

// colorare
void color(const char* cod) {
    printf("%s", cod);
}
void reset_color() {
    printf("\033[0m");
}

// afiseaza header
void printSubmenuHeader(const char* title) {
    printf("========== %s ==========\n\n", title);
}

// incarcarea camerelor din fișier
void incarcaCamere() {
    FILE* f = fopen("camere.txt", "r");
    if (!f) {
        printf("Eroare la deschiderea fisierului camere.txt\n");
        exit(1);
    }
    while (fscanf(f, "%d %s %f %[^\n]",
                  &camere[nrCamere].id,
                  camere[nrCamere].tip,
                  &camere[nrCamere].pret,
                  camere[nrCamere].facilitati) == 4) {
        nrCamere++;
    }
    fclose(f);
}

// numarul de zile dintr-o luna (verificare pentru an bisect)
int getDaysInMonth(int month, int year) {
    if (month == 2) {
        if ((year % 400 == 0) || (year % 4 == 0 && year % 100 != 0))
            return 29;
        else
            return 28;
    } else if (month == 4 || month == 6 || month == 9 || month == 11) {
        return 30;
    }
    return 31;
}

// validarea unei date în format "dd/mm/yyyy"
int isValidDate(const char* dataStr) {
    int zi, luna, an;
    if (sscanf(dataStr, "%d/%d/%d", &zi, &luna, &an) != 3) {
        return 0;
    }
    if (zi < 1 || luna < 1 || luna > 12 || an < 1)
        return 0;
    int zileMax = getDaysInMonth(luna, an);
    if (zi > zileMax)
        return 0;
    return 1;
}

// calculeaza numarul de zile dintre doua date (inclusiv ziua de început si final)
int computeDaysBetween(const char* startDate, const char* endDate) {
    struct tm tm_start = {0}, tm_end = {0};
    sscanf(startDate, "%d/%d/%d", &tm_start.tm_mday, &tm_start.tm_mon, &tm_start.tm_year);
    sscanf(endDate, "%d/%d/%d", &tm_end.tm_mday, &tm_end.tm_mon, &tm_end.tm_year);
    tm_start.tm_mon -= 1;
    tm_end.tm_mon -= 1;
    tm_start.tm_year -= 1900;
    tm_end.tm_year -= 1900;
    tm_start.tm_hour = tm_start.tm_min = tm_start.tm_sec = 0;
    tm_end.tm_hour = tm_end.tm_min = tm_end.tm_sec = 0;

    time_t t_start = mktime(&tm_start);
    time_t t_end   = mktime(&tm_end);

    double diff_sec = difftime(t_end, t_start);
    int days = diff_sec / (60 * 60 * 24) + 1;
    return days;
}

// verifica daca o camera este rezervata
int isCameraReserved(int cameraId) {
    FILE* f = fopen("rezervari.txt", "r");
    if (!f) return 0;
    int id, persoane;
    char startDate[11], endDate[11];
    int reserved = 0;
    while (fscanf(f, "%d %d %s %s", &id, &persoane, startDate, endDate) == 4) {
        if (id == cameraId) {
            reserved = 1;
            break;
        }
    }
    fclose(f);
    return reserved;
}

// afisarea camerelor disponibile
void afisareCamere() {
    system(CLEAR);
    printSubmenuHeader("Afisare Camere Disponibile");
    int existaDisponibile = 0;
    for (int i = 0; i < nrCamere; i++) {
        if (!isCameraReserved(camere[i].id)) {
            existaDisponibile = 1;
            // ID
            printf("ID: ");
            color("\033[1;34m");
            printf("%d", camere[i].id);
            reset_color();
            printf("\n");

            //tipul camerei
            printf("Tipul camerei: ");
            color("\033[1;34m");
            printf("%s", camere[i].tip);
            reset_color();
            printf("\n");

            //pretul pe noapte
            printf("Pret pe noapte: ");
            color("\033[1;32m");
            printf("%.2f RON", camere[i].pret);
            reset_color();
            printf("\n");

            // facilitati
            printf("Facilitati: ");
            color("\033[1;35m");
            printf("%s", camere[i].facilitati);
            reset_color();
            printf("\n--------------------------\n");
        }
    }
    if (!existaDisponibile) {
        printf("Nu exista camere disponibile momentan.\n");
    }
    printf("Apasa ENTER pentru a reveni la meniu...");
    getchar(); getchar();
}

// cautarea dupa tip
void cautaCameraTip() {
    system(CLEAR);
    printSubmenuHeader("Cautare Camera După Tip");

    char tipCautat[20];
    printf("Introdu tipul camerei (ex: Single, Double, Suite): ");
    scanf("%s", tipCautat);

    system(CLEAR);
    printSubmenuHeader("Rezultate Cautare (Tip)");
    int gasit = 0;
    for (int i = 0; i < nrCamere; i++) {
        if (strcmp(camere[i].tip, tipCautat) == 0) {
            // ID
            printf("ID: ");
            color("\033[1;34m");
            printf("%d", camere[i].id);
            reset_color();
            printf("\n");

            // tipul camerei
            printf("Tipul camerei: ");
            color("\033[1;34m");
            printf("%s", camere[i].tip);
            reset_color();
            printf("\n");

            // pretul pe noapte
            printf("Pret pe noapte: ");
            color("\033[1;32m");
            printf("%.2f RON", camere[i].pret);
            reset_color();
            printf("\n");

            // facilitati
            printf("Facilitati: ");
            color("\033[1;35m");
            printf("%s", camere[i].facilitati);
            reset_color();
            printf("\n--------------------------\n");
            gasit = 1;
        }
    }
    if (!gasit) {
        printf("Nu s-a gasit nicio camera de tipul '%s'\n", tipCautat);
    }
    printf("Apasa ENTER pentru a reveni la meniu...");
    getchar(); getchar();
}

// cautare dupa o facilitate
void cautaCameraFacilitati() {
    system(CLEAR);
    printSubmenuHeader("Cautare Camera După Facilitati");

    char facilitateCautata[30];
    printf("Introdu o facilitate dorita (ex: WiFi, Jacuzzi): ");
    scanf("%s", facilitateCautata);

    system(CLEAR);
    printSubmenuHeader("Rezultate Cautare (Facilitati)");
    int gasit = 0;
    for (int i = 0; i < nrCamere; i++) {
        if (strstr(camere[i].facilitati, facilitateCautata) != NULL) {
            // ID
            printf("ID: ");
            color("\033[1;34m");
            printf("%d", camere[i].id);
            reset_color();
            printf("\n");

            // tipul camerei
            printf("Tipul camerei: ");
            color("\033[1;34m");
            printf("%s", camere[i].tip);
            reset_color();
            printf("\n");

            // pretul pe noapte
            printf("Pret pe noapte: ");
            color("\033[1;32m");
            printf("%.2f RON", camere[i].pret);
            reset_color();
            printf("\n");

            // facilitatile
            printf("Facilitati: ");
            color("\033[1;35m");
            printf("%s", camere[i].facilitati);
            reset_color();
            printf("\n--------------------------\n");
            gasit = 1;
        }
    }
    if (!gasit) {
        printf("Nu s-a gasit nicio camera cu facilitatea '%s'\n", facilitateCautata);
    }
    printf("Apasa ENTER pentru a reveni la meniu...");
    getchar(); getchar();
}

// rezervare
void rezervareCamera() {
    system(CLEAR);
    printSubmenuHeader("Rezervare Camera");

    int id;
    printf("Introdu ID-ul camerei pe care doresti sa o rezervi: ");
    scanf("%d", &id);
    int validId = 0;
    int indexCamera = -1;
    for (int i = 0; i < nrCamere; i++) {
        if (camere[i].id == id) {
            validId = 1;
            indexCamera = i;
            break;
        }
    }
    if (!validId) {
        printf("ID invalid!\n");
        printf("Apasa ENTER pentru a reveni la meniu...");
        getchar(); getchar();
        return;
    }
    if (isCameraReserved(id)) {
        printf("Camera cu ID ");
        color("\033[1;34m");
        printf("%d", id);
        reset_color();
        printf(" este deja rezervata!\n");
        printf("Apasa ENTER pentru a reveni la meniu...");
        getchar(); getchar();
        return;
    }

    Rezervare r;
    r.idCamera = id;

    printf("Numar persoane: ");
    scanf("%d", &r.persoane);

    int valid = 0;
    do {
        printf("Introdu data de INCEPUT a rezervarii (dd/mm/yyyy): ");
        getchar();
        fgets(r.startDate, sizeof(r.startDate), stdin);
        r.startDate[strcspn(r.startDate, "\n")] = 0;
        if (!isValidDate(r.startDate)) {
            printf("Data introdusa nu este valida! Reincearca.\n");
        } else {
            valid = 1;
        }
    } while (!valid);

    valid = 0;
    do {
        printf("Introdu data de SFARSIT a rezervarii (dd/mm/yyyy): ");
        fgets(r.endDate, sizeof(r.endDate), stdin);
        r.endDate[strcspn(r.endDate, "\n")] = 0;
        if (!isValidDate(r.endDate)) {
            printf("Data introdusa nu este valida! Reincearca.\n");
            continue;
        }
        int zile = computeDaysBetween(r.startDate, r.endDate);
        if (zile <= 0) {
            printf("Perioada nu este valida (data de final trebuie sa fie dupa data de inceput)! Reincearca.\n");
        } else {
            valid = 1;
        }
    } while (!valid);

    int nrZile = computeDaysBetween(r.startDate, r.endDate);
    float totalPret = camere[indexCamera].pret * nrZile;

    FILE* f = fopen("rezervari.txt", "a");
    if (!f) {
        printf("Eroare la deschiderea fisierului rezervari.txt\n");
        return;
    }
    fprintf(f, "%d %d %s %s\n", r.idCamera, r.persoane, r.startDate, r.endDate);
    fclose(f);

    printf("Rezervare efectuata cu succes!\n");
    printf("Perioada: ");
    color("\033[1;33m");
    printf("%s", r.startDate);
    reset_color();
    printf(" pana la ");
    color("\033[1;33m");
    printf("%s", r.endDate);
    reset_color();
    printf(" (%d zile)\n", nrZile);
    printf("Total de plata: ");
    color("\033[1;32m");
    printf("%.2f RON", totalPret);
    reset_color();
    printf("\nApasa ENTER pentru a reveni la meniu...");
    getchar(); getchar();
}

// anulare rezervare
void anuleazaRezervare() {
    system(CLEAR);
    printSubmenuHeader("Anulare Rezervare");

    int id;
    printf("Introdu ID-ul camerei pentru care vrei sa anulezi rezervarea: ");
    scanf("%d", &id);

    FILE* f = fopen("rezervari.txt", "r");
    if (!f) {
        printf("Eroare la deschiderea fisierului rezervari.txt\n");
        getchar(); getchar();
        return;
    }
    FILE* temp = fopen("temp.txt", "w");
    if (!temp) {
        printf("Eroare la crearea fisierului temporar!\n");
        fclose(f);
        getchar(); getchar();
        return;
    }

    Rezervare r;
    int gasit = 0;
    while (fscanf(f, "%d %d %s %s", &r.idCamera, &r.persoane, r.startDate, r.endDate) == 4) {
        if (r.idCamera != id) {
            fprintf(temp, "%d %d %s %s\n", r.idCamera, r.persoane, r.startDate, r.endDate);
        } else {
            gasit = 1;
        }
    }
    fclose(f);
    fclose(temp);
    remove("rezervari.txt");
    rename("temp.txt", "rezervari.txt");

    if (gasit) {
        printf("Rezervare anulata.\n");
    } else {
        printf("Nu a fost gasita rezervarea pentru ID-ul ");
        color("\033[1;34m");
        printf("%d", id);
        reset_color();
        printf(".\n");
    }
    printf("Apasa ENTER pentru a reveni la meniu...");
    getchar(); getchar();
}

// afisarea rezervarilor curente
void afiseazaRezervari() {
    system(CLEAR);
    printSubmenuHeader("Rezervari Curente");

    FILE* f = fopen("rezervari.txt", "r");
    if (!f) {
        printf("Nu exista rezervari inregistrate momentan.\n");
        printf("Apasa ENTER pentru a reveni la meniu...");
        getchar(); getchar();
        return;
    }
    Rezervare r;
    while (fscanf(f, "%d %d %s %s", &r.idCamera, &r.persoane, r.startDate, r.endDate) == 4) {
        int index = -1;
        for (int i = 0; i < nrCamere; i++) {
            if (camere[i].id == r.idCamera) {
                index = i;
                break;
            }
        }
        if (index != -1) {
            int nrZile = computeDaysBetween(r.startDate, r.endDate);
            float totalPret = camere[index].pret * nrZile;

            printf("ID Camera: ");
            color("\033[1;34m");
            printf("%d", camere[index].id);
            reset_color();
            printf("\n");

            printf("Tipul camerei: ");
            color("\033[1;34m");
            printf("%s", camere[index].tip);
            reset_color();
            printf("\n");

            printf("Pret pe noapte: ");
            color("\033[1;32m");
            printf("%.2f RON", camere[index].pret);
            reset_color();
            printf("\n");

            printf("Facilitati: ");
            color("\033[1;35m");
            printf("%s", camere[index].facilitati);
            reset_color();
            printf("\n");

            printf("Numar persoane: ");
            color("\033[1;33m");
            printf("%d", r.persoane);
            reset_color();
            printf("\n");

            printf("Perioada: ");
            color("\033[1;33m");
            printf("%s", r.startDate);
            reset_color();
            printf(" pana la ");
            color("\033[1;33m");
            printf("%s", r.endDate);
            reset_color();
            printf(" (%d zile)\n", nrZile);

            printf("Total de plata: ");
            color("\033[1;32m");
            printf("%.2f RON", totalPret);
            reset_color();
            printf("\n--------------------------\n");
        } else {
            printf("Rezervare pentru o camera necunoscuta (ID: ");
            color("\033[1;34m");
            printf("%d", r.idCamera);
            reset_color();
            printf(")\n");
        }
    }
    fclose(f);
    printf("Apasa ENTER pentru a reveni la meniu...");
    getchar(); getchar();
}

// meniu principal
void meniuPrincipal() {
    int optiune;
    do {
        system(CLEAR);
        printf("========== Meniu Principal ==========\n");
        printf("1. Afiseaza camere disponibile\n");
        printf("2. Cauta camera dupa tip\n");
        printf("3. Cauta camera dupa facilitati\n");
        printf("4. Rezerva camera\n");
        printf("5. Anuleaza rezervare\n");
        printf("6. Afiseaza rezervarile curente\n");
        printf("0. Iesire\n");
        printf("Selectati o optiune: ");
        scanf("%d", &optiune);

        switch (optiune) {
            case 1: afisareCamere(); break;
            case 2: cautaCameraTip(); break;
            case 3: cautaCameraFacilitati(); break;
            case 4: rezervareCamera(); break;
            case 5: anuleazaRezervare(); break;
            case 6: afiseazaRezervari(); break;
            case 0: printf("La revedere!\n"); break;
            default:
                printf("Optiune invalida! Apasa ENTER pentru a reincerca...");
                getchar(); getchar();
        }
    } while (optiune != 0);
}

int main() {
    incarcaCamere();
    meniuPrincipal();
    return 0;
}
