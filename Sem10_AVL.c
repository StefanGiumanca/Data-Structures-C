#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

struct Nod {
    Masina info;
    struct Nod* st;
    struct Nod* dr;
};
typedef struct Nod Nod;


Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret= atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy(m1.model, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy(m1.numeSofer, aux);

	m1.serie = *strtok(NULL, sep);
	return m1;
}

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

int calculeazaInaltimeArbore(Nod* arbore) {
    if(arbore) {
        int inaltimeSt = calculeazaInaltimeArbore(arbore->st);
        int inaltimeDr = calculeazaInaltimeArbore(arbore->dr);
        return 1 + (inaltimeSt > inaltimeDr ? inaltimeSt : inaltimeDr);
    }
    return 0;
}

void rotireLaDreapta(Nod** arbore) {
    Nod* aux = (*arbore)->st;
    (*arbore)->st = aux->dr;
    aux->dr = (*arbore);
    (*arbore) = aux;
}

void rotireLaStanga(Nod** arbore) {
    Nod* aux = (*arbore)->dr;
    (*arbore)->dr = aux->st;
    aux->st = (*arbore);
    (*arbore) = aux;
}

int calculeazaGradDeEchilibru(Nod* arbore) {
    if(arbore) {
        return calculeazaInaltimeArbore(arbore->st) - calculeazaInaltimeArbore(arbore->dr);
    }
    return 0;
}

void adaugaMasinaInArboreEchilibrat(Nod** arbore, Masina masinaNoua) {
    if(*arbore) {
        if((*arbore)->info.id < masinaNoua.id) {
            adaugaMasinaInArboreEchilibrat(&((*arbore)->dr), masinaNoua);
        }
        else
        {
            adaugaMasinaInArboreEchilibrat(&((*arbore)->st), masinaNoua);
        }
        int gradEchilibru = calculeazaGradDeEchilibru(*arbore);
        if(gradEchilibru == 2) 
        {
            if(calculeazaGradDeEchilibru((*arbore)->st) == 1)
            {   
                rotireLaDreapta(arbore);
            }
            else 
            {
                rotireLaStanga(&((*arbore)->st));
                rotireLaDreapta(arbore);
            }
        }
        else if (gradEchilibru == -2)
        {
            if(calculeazaGradDeEchilibru((*arbore)->dr) != -1)
            {
                rotireLaDreapta(&((*arbore)->dr));
            }
            rotireLaStanga(arbore);
        }
    }
    else
    {
        Nod* nou = (Nod*)malloc(sizeof(Nod));
        nou->info = masinaNoua; // shallow copy
        nou->dr = NULL;
        nou->st = NULL;
        *arbore = nou;
    }
}

Nod* citireArboreDeMasiniDinFisier(const char* numeFisier) {
    FILE* f = fopen(numeFisier, "r");
    Nod* arbore = NULL;
    if(f) {
        while(!feof(f)) {
           Masina m = citireMasinaDinFisier(f);
           adaugaMasinaInArboreEchilibrat(&arbore, m);
        }
        return arbore;
    }
    return arbore;
}

void afisareMasiniDinArborePreOrdine(Nod* arbore) {
    if(arbore) {
	afisareMasina(arbore->info);
    afisareMasiniDinArborePreOrdine(arbore->st);
    afisareMasiniDinArborePreOrdine(arbore->dr);
    }
}

void dezalocareArboreDeMasini(Nod** arbore) {
	if(*arbore) {
        dezalocareArboreDeMasini(&(*arbore)->st);
        dezalocareArboreDeMasini(&(*arbore)->dr);
        free((*arbore)->info.model);
        free((*arbore)->info.numeSofer);
        (*arbore)->st = NULL;
        (*arbore)->dr = NULL;
        free(*arbore);
        *arbore = NULL;
    }
}

Masina getMasinaById(Nod* arbore, int id) {
    Masina m;
    m.id = -1;
    if(arbore)
    {
        if(arbore->info.id == id) {
            return arbore->info;
        }
        else if (arbore->info.id > id)
        {
            return getMasinaById(arbore->st, id);
        }
        else
        {
            return getMasinaById(arbore->dr, id);
        }
    }
    return m;
}

int determinaNumarNoduri(Nod* arbore)
{
    if(arbore) {
        return 1 + determinaNumarNoduri(arbore->st) + determinaNumarNoduri(arbore->dr);
    }
    return 0;
}

float calculeazaPretTotal(Nod* arbore) {
    float sum = 0;
    if(arbore) {
        sum += arbore->info.pret;
        sum += calculeazaPretTotal(arbore->st);
        sum += calculeazaPretTotal(arbore->dr);
        return sum;
    }
    else return sum;
}

float calculeazaPretulMasinilorUnuiSofer(Nod* arbore, const char* numeSofer) {
    float sum = 0;
    if(arbore) {
        if(strcmp(arbore->info.numeSofer, numeSofer) == 0) {
            sum += arbore->info.pret;
        }
        sum += calculeazaPretulMasinilorUnuiSofer(arbore->st, numeSofer);
        sum += calculeazaPretulMasinilorUnuiSofer(arbore->dr, numeSofer);
        return sum;
    }
    return sum;
}

int main() {

    Nod* arbore = NULL;
    arbore = citireArboreDeMasiniDinFisier("masini.txt");
    afisareMasiniDinArborePreOrdine(arbore);

    printf("\n\n--------------Masina cautata---------------\n");
    Masina m = getMasinaById(arbore, 2);
    afisareMasina(m);

    printf("\nNumarul total de noduri din arbore este: %d\n\n", determinaNumarNoduri(arbore));

    printf("\nPretul total de masini din arbore este: %.2f\n\n", calculeazaPretTotal(arbore));

    printf("\nPretul total de masini al soferului cautat este: %.2f\n\n", calculeazaPretulMasinilorUnuiSofer(arbore, "Ionescu"));


	return 0;
}