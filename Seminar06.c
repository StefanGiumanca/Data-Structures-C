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
{ 
	Masina info;
	struct Nod *next;
};

struct HashTable
{
	int dim;
	struct Nod **tabela;
};
typedef struct Nod Nod;
typedef struct HashTable HashTable;

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

void afisareListaMasini(Nod *cap)
{
	while (cap)
	{
		afisareMasina(cap->info);
		cap = cap->next;
	}
}

void adaugaMasinaInLista(Nod *cap, Masina masinaNoua)
{
	Nod *nou = (Nod *)malloc(sizeof(Nod));
	nou->info = masinaNoua; // shallow copy
	nou->next = NULL;
	Nod *aux = cap;
	while (aux->next)
	{
		aux = aux->next;
	}
	aux->next = nou;
}

HashTable initializareHashTable(int dimensiune)
{
	HashTable ht;
	ht.dim = dimensiune;
	ht.tabela = (Nod **)malloc(sizeof(Nod *) * dimensiune);
	for (int i = 0; i < dimensiune; i++)
	{
		ht.tabela[i] = NULL;
	}
	return ht;
}

int calculeazaHash(const char *numeSofer, int dimensiune)
{
	int suma = 0;
	for (int i = 0; i < strlen(numeSofer); i++)
	{
		suma += numeSofer[i];
	}
	return suma % dimensiune;
}

void inserareMasinaInTabela(HashTable hash, Masina masina)
{
	int pozitie = calculeazaHash(masina.numeSofer, hash.dim);
	if (hash.tabela[pozitie] == NULL)
	{ // daca este loc in tabela inseram masina
		hash.tabela[pozitie] = (Nod *)malloc(sizeof(Nod));
		hash.tabela[pozitie]->info = masina;
		hash.tabela[pozitie]->next = NULL;
	}
	else
	{
		adaugaMasinaInLista(hash.tabela[pozitie], masina); // daca locul este ocupat si avem coliziune, vom insera folosind functia adaugaMasinaInLista
	}
}

HashTable citireMasiniDinFisier(const char *numeFisier, int dimensiune)
{
	HashTable ht = initializareHashTable(dimensiune);
	FILE *f = fopen(numeFisier, "r");
	while (!feof(f))
	{
		Masina m = citireMasinaDinFisier(f);
		inserareMasinaInTabela(ht, m);
	}
	fclose(f);
	return ht;
}

void afisareTabelaDeMasini(HashTable ht)
{
	for (int i = 0; i < ht.dim; i++)
	{
		if (ht.tabela[i])
		{
			printf("Masinile de pe pozitia %d sunt: \n", i);
			afisareListaMasini(ht.tabela[i]);
		}
		else
		{
			printf("\nPe pozitia %d nu avem masini \n", i);
		}
	}
}

void dezalocareListaMasini(Nod **cap)
{
	Nod *p = (*cap);
	while (p)
	{
		Nod *temp = p;
		p = p->next;
		if (temp->info.model)
		{
			free(temp->info.model);
		}
		if (temp->info.numeSofer)
		{
			free(temp->info.numeSofer);
		}
		free(temp);
	}
	*cap = NULL;
}

void dezalocareTabelaDeMasini(HashTable *ht)
{
	for (int i = 0; i < ht->dim; i++)
	{
		dezalocareListaMasini(&(ht->tabela[i]));
	}
	free(ht->tabela);
	ht->tabela = NULL;
	ht->dim = 0;
}

float calculeazaMedieLista(Nod *cap)
{
	float suma = 0;
	int contor = 0;
	while (cap)
	{
		suma += cap->info.pret;
		contor++;
		cap = cap->next;
	}
	return (contor > 0 ? (suma / contor) : 0);
}

float *calculeazaPreturiMediiPerClustere(HashTable ht, int *nrClustere)
{
	float *preturi = NULL;
	*nrClustere = 0;
	for (int i = 0; i < ht.dim; i++)
	{
		if (ht.tabela[i])
		{
			(*nrClustere)++;
		}
	}
	preturi = (float *)malloc(sizeof(float) * (*nrClustere));
	int contor = 0;
	for (int i = 0; i < ht.dim; i++)
	{
		if (ht.tabela[i])
		{
			preturi[contor] = calculeazaMedieLista(ht.tabela[i]);
			contor++;
		}
	}
	return preturi;
}

Masina getMasinaDinLista(Nod *cap, const char *nume)
{
	Masina m;
	m.id = -1;
	while (cap && strcmp(cap->info.numeSofer, nume) != 0)
	{
		cap = cap->next;
	}
	if (cap)
	{
		m = cap->info;
		m.model = (char *)malloc(strlen(cap->info.model) + 1);
		strcpy(m.model, cap->info.model);
		m.numeSofer = (char *)malloc(strlen(cap->info.numeSofer) + 1);
		strcpy(m.numeSofer, cap->info.numeSofer);
	}
	return m;
}

Masina getMasinaDupaNumeSofer(HashTable ht, const char *numeCautat)
{
	Masina m;
	m.id = -1;
	int pozitie = calculeazaHash(numeCautat, ht.dim);
	if (pozitie >= 0 && pozitie < ht.dim)
	{
		return getMasinaDinLista(ht.tabela[pozitie], numeCautat);
	}
	return m;
}

int main()
{

	HashTable ht = citireMasiniDinFisier("masini.txt", 7);
	afisareTabelaDeMasini(ht);

	int nrClustere = 0;
	float *preturi = calculeazaPreturiMediiPerClustere(ht, &nrClustere);
	printf("Preturi medii per clustere: ");
	for (int i = 0; i < nrClustere; i++)
	{
		printf("%.2f, ", preturi[i]);
	}
	printf("\n\n");
	Masina m1 = getMasinaDupaNumeSofer(ht, "Gheorgheclear");
	if (m1.id != -1)
	{
		printf("\n\nMasina cautata: \n");
		afisareMasina(m1);
		if (m1.model)
		{
			free(m1.model);
		}
		if (m1.numeSofer)
		{
			free(m1.numeSofer);
		}
	}
	else
	{
		printf("Masina cautata nu este gasita. Soferul respectiv nu are nicio masina!");
	}

	dezalocareTabelaDeMasini(&ht);
	return 0;
}
