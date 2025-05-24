#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraMasina
{
    int id;
    int nrUsi;
    float pret;
    char *model;
    char *numeSofer;
    unsigned char serie;
};
typedef struct StructuraMasina Masina;

struct Nod
{ // structura pentru un Arbore Binar de Cautare
    Masina info;
    struct Nod *st;
    struct Nod *dr;
};
typedef struct Nod Nod;

Masina initializare(int id, int nrUsi, float pret, char *model, char *numeSofer, unsigned char serie)
{
    Masina m;
    m.id = id;
    m.nrUsi = nrUsi;
    m.serie = serie;
    m.pret = pret;
    m.model = (char *)malloc(strlen(model) + 1);
    strcpy(m.model, model);
    m.numeSofer = (char *)malloc(strlen(numeSofer) + 1);
    strcpy(m.numeSofer, numeSofer);
    return m;
}

Masina citireMasinaDinFisier(FILE *file)
{
    char buffer[100];
    char sep[3] = ",\n";
    fgets(buffer, 100, file);
    char *aux;
    Masina m1;
    aux = strtok(buffer, sep);
    m1.id = atoi(aux);
    m1.nrUsi = atoi(strtok(NULL, sep));
    m1.pret = atof(strtok(NULL, sep));
    aux = strtok(NULL, sep);
    m1.model = malloc(strlen(aux) + 1);
    strcpy(m1.model, aux);

    aux = strtok(NULL, sep);
    m1.numeSofer = malloc(strlen(aux) + 1);
    strcpy(m1.numeSofer, aux);

    m1.serie = *strtok(NULL, sep);
    return m1;
}

void afisareMasina(Masina masina)
{
    printf("Id: %d\n", masina.id);
    printf("Nr. usi : %d\n", masina.nrUsi);
    printf("Pret: %.2f\n", masina.pret);
    printf("Model: %s\n", masina.model);
    printf("Nume sofer: %s\n", masina.numeSofer);
    printf("Serie: %c\n\n", masina.serie);
}

void adaugaMasinaInArbore(Nod **arbore, Masina masinaNoua) // Inserare Nod in Arbore Binar de Cautare respectand cerintele
{
    if (*arbore) // daca exista arborele il parcurgem pana la nodul frunza
    {
        if (masinaNoua.id > (*arbore)->info.id)
        {                                                       // verificam daca cheia primita este mai mare decat nodul radacina
            adaugaMasinaInArbore(&((*arbore)->dr), masinaNoua); // facem recursivitate ca sa ne deplasam pana la nodul frunza
        }
        else if (masinaNoua.id < (*arbore)->info.id)
        { // la fel pentru la dreapta
            adaugaMasinaInArbore(&((*arbore)->st), masinaNoua);
        }
        else
        {
            (*arbore)->info.id = masinaNoua.id;
            (*arbore)->info.nrUsi = masinaNoua.nrUsi;
            (*arbore)->info.pret = masinaNoua.pret;

            if ((*arbore)->info.model)
            {
                free((*arbore)->info.model);
            }
            if ((*arbore)->info.numeSofer)
            {
                free((*arbore)->info.numeSofer);
            }

            (*arbore)->info.model = (char *)malloc(strlen(masinaNoua.model) + 1);
            strcpy((*arbore)->info.model, masinaNoua.model);

            (*arbore)->info.numeSofer = (char *)malloc(strlen(masinaNoua.numeSofer) + 1);
            strcpy((*arbore)->info.numeSofer, masinaNoua.numeSofer);
        }
    }
    else // AICI ajungem in pozitia unde nu se afla nimic si trebuie sa introducem noul nod
    {
        (*arbore) = (Nod *)malloc(sizeof(Nod));
        (*arbore)->info.id = masinaNoua.id;
        (*arbore)->info.nrUsi = masinaNoua.nrUsi;
        (*arbore)->info.pret = masinaNoua.pret;
        (*arbore)->info.serie = masinaNoua.serie;
        (*arbore)->info.model = (char *)malloc(strlen(masinaNoua.model) + 1);
        strcpy((*arbore)->info.model, masinaNoua.model);
        (*arbore)->info.numeSofer = (char *)malloc(strlen(masinaNoua.numeSofer) + 1);
        strcpy((*arbore)->info.numeSofer, masinaNoua.numeSofer);
        (*arbore)->dr = NULL; // setam directiile pe NULL;
        (*arbore)->st = NULL;
    }
}

Nod *citireArboreDeMasiniDinFisier(const char *numeFisier)
{
    FILE *f = fopen(numeFisier, "r");
    Nod *arbore = NULL;
    if (f)
    {
        while (!feof(f))
        {
            Masina m = citireMasinaDinFisier(f);
            adaugaMasinaInArbore(&arbore, m);
        }
        fclose(f);
    }
    return arbore;
}

void afisareInOrdine(Nod *arbore) // afisam arborele in IN Ordine (Stanga-Radacina-Dreapta)
{
    if (arbore)
    {
        afisareInOrdine(arbore->st);
        afisareMasina(arbore->info);
        afisareInOrdine(arbore->dr);
    }
}

void afisarePreOrdine(Nod *arbore)
{ // afisam arborele in PRE Ordine (Radacina-Stanga-Dreapta)
    if (arbore)
    {
        afisareMasina(arbore->info);
        afisarePreOrdine(arbore->st);
        afisarePreOrdine(arbore->dr);
    }
}

void afisarePostOrdine(Nod *arbore)
{ // afisam arborele in POST Ordine (Stanga-Dreapta-Radacina)
    if (arbore)
    {
        afisarePostOrdine(arbore->st);
        afisarePostOrdine(arbore->dr);
        afisareMasina(arbore->info);
    }
}

void afisareToateParcurgerile(Nod *arbore)
{
    printf("\n--- INORDINE ---\n");
    afisareInOrdine(arbore);

    printf("\n--- PREORDINE ---\n");
    afisarePreOrdine(arbore);

    printf("\n--- POSTORDINE ---\n");
    afisarePostOrdine(arbore);
}

void dezalocareArboreDeMasini(Nod **arbore) // trebuie dezalocat in postOrdine, mai intai subarborii si dupa nodul principal

{
    if (*arbore) // verificam daca exista arbore
    {
        dezalocareArboreDeMasini(&(*arbore)->st); // mergem in subarborele stang prin apel recursiv si stergem
        dezalocareArboreDeMasini(&(*arbore)->dr); // la fel pt cel drept
        free((*arbore)->info.model);              // membri alocati dinamic
        free((*arbore)->info.numeSofer);
        free(*arbore);  // stergem arborele principal radacina
        *arbore = NULL; // le trecem pe null pt a evita dangling pointers
        (*arbore)->st = NULL;
        (*arbore)->dr = NULL;
    }
}

Masina getMasinaByID(Nod *arbore, int id) // extragem id-ul primit ca parametru din arbor
{
    Masina m; // ne cream o masina sa o returnam cu id ul implicit -1 daca nu exista arbore
    m.id = -1;
    if (arbore) // verificam daca exista arbore
    {
        if (arbore->info.id == id)
        { // daca id-ul primit este = cu nodul radacina returnam nodul radacina
            return arbore->info;
        }
        else if (arbore->info.id < id)
        {                                         // verificam daca id ul este mai mare decat nodul radacina si ne deplasam in subarborele drept
            return getMasinaByID(arbore->dr, id); // apelam recursiv functia pana gasim id ul corespunzator
        }
        else
        {
            return getMasinaByID(arbore->st, id); // acelasi principiu il aplicam daca id ul este mai mic mergem in subarborele stang
        }
    }
    return m; // returnam masina cu -1 daca nu gasim nimic sau nu exista arbore
}

int determinaNumarNoduri(Nod *arbore)
{
    if (arbore)
    {
        int nrNoduriStanga = determinaNumarNoduri(arbore->st);
        int nrNoduriDreapta = determinaNumarNoduri(arbore->dr);
        return 1 + nrNoduriStanga + nrNoduriDreapta;
    }
    return 0;
}

int calculeazaInaltimeArbore(Nod *arbore)
{
    int inaltime = 0;
    if (arbore)
    {
        int inaltimeStanga = calculeazaInaltimeArbore(arbore->st);
        int inaltimeDreapta = calculeazaInaltimeArbore(arbore->dr);
        return 1 + (inaltimeStanga > inaltimeDreapta ? inaltimeStanga : inaltimeDreapta);
    }
    return 0;
}

float calculeazaPretTotal(Nod *arbore)
{
    float sum = 0;
    if (arbore)
    {
        sum += arbore->info.pret;                             // adaugam la suma nodul radacina
        float totalStanga = calculeazaPretTotal(arbore->st);  // calculam pentru sub-arborele stang
        float totalDreapta = calculeazaPretTotal(arbore->dr); // calculam pentru sub-arborele drept
        sum += totalDreapta += totalStanga;                   // adunam la suma tot si returnam suma
        return sum;
    }
    return 0;
}

float calculeazaPretulMasinilorUnuiSofer(Nod *arbore, const char *numeSofer)
{
    float sum = 0;
    if (arbore)
    {
        if (strcmp(arbore->info.numeSofer, numeSofer) == 0)
        {
            sum += arbore->info.pret;
        }
        sum += calculeazaPretulMasinilorUnuiSofer(arbore->st, numeSofer);
        sum += calculeazaPretulMasinilorUnuiSofer(arbore->dr, numeSofer);
        return sum;
    }
    return 0;
}

int main()
{
    // Nod* arbore = NULL;                                                      VALORI HARDCODATE
    // Masina* masini = malloc(sizeof(Masina) * 5);
    // masini[0] = initializare(1, 4, 12000, "Superb", "Marian", 'A');
    // masini[1] = initializare(2, 2, 19500, "Logan", "Ion", 'C');
    // masini[2] = initializare(3, 4, 1750, "Urus", "Vasile", 'A');
    // masini[3] = initializare(4, 3, 80750, "CLA", "Ilie", 'B');
    // masini[4] = initializare(5, 4, 54300, "M5", "Andrei", 'A');
    // for(int i = 0; i < 5; i++) {
    //     adaugaMasinaInArbore(&arbore, masini[i]);
    // }
    // afisareToateParcurgerile(arbore);

    Nod *arbore = citireArboreDeMasiniDinFisier("masini.txt");      // citirea masinilor din fisier in arbore
    afisareToateParcurgerile(arbore);

    printf("--------Masina cautata: -------------\n\n\n");
    Masina masinaCautata = getMasinaByID(arbore, 4);            // cautarea masinii dupa id
    afisareMasina(masinaCautata);

    int totalNoduri = determinaNumarNoduri(arbore);
    printf("Numarul total de noduri din arbore este de %d noduri \n\n", totalNoduri);           // determinarea numarului total de noduri din arbore

    float sumaPretArbore = calculeazaPretTotal(arbore);
    printf("Pretul total al tuturor nodurilor din arbore este de %.2f \n\n", sumaPretArbore);       // suma preturilor tuturor masinilor din arbore

    int hArbore = calculeazaInaltimeArbore(arbore);     // calcularea inaltimii arborelui
    printf("Inaltimea arborelui este de %d noduri \n\n", hArbore);

    float sumaPretMasiniSoferCautat = calculeazaPretulMasinilorUnuiSofer(arbore, "Ionescu");                // suma preturilor masinilor unui sofer al carui nume este dat
    printf("Suma preturilor masinilor soferului cautat este de %.2f \n\n", sumaPretMasiniSoferCautat);

    return 0;
}
