#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//trebuie sa folositi fisierul masini.txt
//sau va creati un alt fisier cu alte date

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

//creare structura pentru un nod dintr-un arbore binar de cautare

struct Nod {
    struct Nod* st;
    struct Nod* dr;
    Masina info;
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


void adaugaMasinaInArbore(Nod** radacina, Masina masinaNoua) {
	//adauga o noua masina pe care o primim ca parametru in arbore,
	//astfel incat sa respecte princiippile de arbore binar de cautare
	//dupa o anumita cheie pe care o decideti - poate fi ID
    if (*radacina != NULL)
    {
        if (masinaNoua.id <(*radacina)->info.id)
        {
            adaugaMasinaInArbore(&((*radacina)->st), masinaNoua);
        }
        else if (masinaNoua.id > (*radacina)->info.id)
        {
            adaugaMasinaInArbore(&((*radacina)->dr),masinaNoua);
        }
        else
        {
            // implementam suprascrierea daca idurile sunt egale
            (*radacina)->info.nrUsi = masinaNoua.nrUsi;
            (*radacina)->info.pret = masinaNoua.pret;
            (*radacina)->info.serie = masinaNoua.serie;

            if ((*radacina)->info.model)
            {
                free((*radacina)->info.model); // trebuie oricand
            }
            
            if ((*radacina)->info.numeSofer)
            {
                free((*radacina)->info.numeSofer);
            }

            (*radacina)->info.model = (char*)malloc(strlen(masinaNoua.model) + 1);
            strcpy((*radacina)->info.model, masinaNoua.model);
            
            (*radacina)->info.numeSofer = (char*)malloc(strlen(masinaNoua.numeSofer) + 1);
            strcpy((*radacina)->info.numeSofer, masinaNoua.numeSofer);
        }
    }
    else
    {
        *radacina = (Nod*)malloc(sizeof(Nod)); // nod simplu nu *
        (*radacina)->info = masinaNoua;
        (*radacina)->info.model = (char*)malloc(strlen(masinaNoua.model) + 1);
            strcpy((*radacina)->info.model, masinaNoua.model);
            
        (*radacina)->info.numeSofer = (char*)malloc(strlen(masinaNoua.numeSofer) + 1);
            strcpy((*radacina)->info.numeSofer, masinaNoua.numeSofer);
    }
}

void* citireArboreDeMasiniDinFisier(const char* numeFisier) { // am iesit la tabla
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaDinFisier()
	//ATENTIE - la final inchidem fisierul/stream-ul
}

void afisareMasiniDinArbore(/*arbore de masini*/) { // am iesit la tabla
	//afiseaza toate elemente de tip masina din arborele creat
	//prin apelarea functiei afisareMasina()
	//parcurgerea arborelui poate fi realizata in TREI moduri
	//folositi toate cele TREI moduri de parcurgere
}

void dezalocareArboreDeMasini(/*arbore de masini*/) { // am iesit la tabla
	//sunt dezalocate toate masinile si arborele de elemente
}

Masina getMasinaByID(Nod* radacina, int id) {
	Masina m;
    m.id = -1;
    
    if (id > radacina->info.id)
    {
        return getMasinaByID(radacina->dr, id);
    }
    else if (id < radacina->info.id)
    {
        return getMasinaByID(radacina->st, id);
    }
    else
    {
        m = radacina->info;

        m.model = (char*)malloc(strlen(radacina->info.model) + 1);
        strcpy(m.model, radacina->info.model);

        m.numeSofer = (char*)malloc(strlen(radacina->info.numeSofer) + 1);
        strcpy(m.numeSofer, radacina->info.numeSofer);

        return m;
    }

	return m;
}

int determinaNumarNoduri(Nod* radacina) {
	//calculeaza numarul total de noduri din arborele binar de cautare
    if (radacina != NULL)
    {
        return 1 + determinaNumarNoduri(radacina->st) + determinaNumarNoduri(radacina->dr);
    }
    else {
        return 0;
    }
}

int calculeazaInaltimeArbore(/*arbore de masini*/) {
	//calculeaza inaltimea arborelui care este data de 
	//lungimea maxima de la radacina pana la cel mai indepartat nod frunza
	return 0;
}

float calculeazaPretTotal(/*arbore de masini*/) {
	//calculeaza pretul tuturor masinilor din arbore.
	return 0;
}

float calculeazaPretulMasinilorUnuiSofer(/*arbore de masini*/ const char* numeSofer) {
	//calculeaza pretul tuturor masinilor unui sofer.
	return 0;
}

int main() {

	return 0;
}
