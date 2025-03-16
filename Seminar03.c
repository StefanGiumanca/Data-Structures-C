#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// trebuie sa folositi fisierul masini.txt
// sau va creati un alt fisier cu alte date

struct StructuraMasina
{
    int id;
    int nrUsi;
    float pret;
    char* model;
    char* numeSofer;
    unsigned char serie;
};
typedef struct StructuraMasina Masina;

void afisareMasina(Masina masina)
{
    printf("Id: %d\n", masina.id);
    printf("Nr usi: %d\n", masina.nrUsi);
    printf("Pret: %f\n", masina.pret);
    printf("Model: %s\n", masina.model);
    printf("Nume sofer: %s\n", masina.numeSofer);
    printf("Serie: %c\n", masina.serie);
}

void afisareVectorMasini(Masina* masini, int nrMasini)
{
    for (int i = 0; i < nrMasini; i++)
    {
        afisareMasina(masini[i]);
    }
}

void adaugaMasinaInVector(Masina** masini, int* nrMasini, Masina masinaNoua)
{
    Masina* aux;
    aux = (Masina*)malloc(sizeof(Masina) * ((*nrMasini) + 1));
    for (int i = 0; i < *nrMasini; i++)
    {
        aux[i] = (*masini)[i]; // shallow copy
    }
    aux[(*nrMasini)] = masinaNoua;
    free(*masini);
    (*masini) = aux;
    (*nrMasini) = (*nrMasini) + 1;
}

Masina citireMasinaFisier(FILE* file)
{
    Masina m;
    char buffer[50];
    char delim[4] = ",\n"; // 3 delimitatoare : , ; respectiv \n
    fgets(buffer, 50, file);
    char* aux;
    aux = strtok(buffer, delim);
    m.id = atoi(aux);
    aux = strtok(NULL, delim);
    if (!aux) { printf("Eroare la citirea nrUsi\n"); exit(1); }
    m.nrUsi = atoi(aux);
    aux = strtok(NULL, delim);
    if (!aux) { printf("Eroare la citirea pretului\n"); exit(1); }
    m.pret = atof(aux);
    aux = strtok(NULL, delim);
    m.model = malloc(strlen(aux) + 1);
    strcpy_s(m.model, strlen(aux) + 1, aux);
    aux = strtok(NULL, delim);
    m.numeSofer = malloc(strlen(aux) + 1);
    strcpy_s(m.numeSofer, strlen(aux) + 1, aux);
    aux = strtok(NULL, delim);
    m.serie = aux[0];
    return m;
}

Masina* citireVectorMasiniFisier(const char* numeFisier, int* nrMasiniCitite)
{
    FILE* f = fopen(numeFisier, "r");
    Masina* masini = NULL;
    while (!feof(f))
    {
        Masina m = citireMasinaFisier(f);
        adaugaMasinaInVector(&masini, nrMasiniCitite, m);
    }
    fclose(f);
    return masini;
}

void dezalocareVectorMasini(Masina** vector, int* nrMasini)
{
    for (int i = 0; i < *nrMasini; i++)
    {
        free((*vector)[i].model);
        free((*vector)[i].numeSofer);
    }
    free(*vector);
    (*vector) = NULL;
    (*nrMasini) = 0;
}

int main()
{
    Masina* masini = NULL;
    int nrMasini = 0;
    masini = citireVectorMasiniFisier("fisier.txt", &nrMasini);
    afisareVectorMasini(masini, nrMasini);
    dezalocareVectorMasini(&masini, &nrMasini);
    return 0;
}