#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Structura pentru DestinatieTuristica
struct DestinatieTuristica {	
	char* nume;
	char* tara;
	int nrVizitatori;
	float recenzie; // (1.00 - 5.00)
};
typedef struct DestinatieTuristica Destinatie;

// Structura pentru Arbore Binar De Cautare Echilinrat (AVL)
struct Nod {
	Destinatie info;
	struct Nod* st;
	struct Nod* dr;
};
typedef struct Nod Nod;

void afisareDestinatieTuristica(Destinatie destinatie) {		// Functia de afisare a unei Destinatii
	printf("Destinatie: %s\n", destinatie.nume);
	printf("Tara: %s\n", destinatie.tara);
	printf("Nr vizitatori: %d\n", destinatie.nrVizitatori);
	printf("Recenzie: %.2f\n\n", destinatie.recenzie);
}

Destinatie initializareDestinatieTuristica(const char* nume, const char* tara, int nrVizitatori, float recenzie) {		// Initializare Destinatie pentru valori hardcodate
	Destinatie d;
	d.nume = nume;
	d.tara = tara;
	d.nrVizitatori = nrVizitatori;
	d.recenzie = recenzie;
	return d;
}

Destinatie citireDestinatieDinFisier(FILE* file)				// Functia de citire a unei Destinatii din fisier
{
	char buffer[100];
	char dem[4] = ",;\n";
	fgets(buffer, 100, file);
	char* aux;
	Destinatie d;
	aux = strtok(buffer, dem);
	d.nume = (char*)malloc(strlen(aux) + 1);
	strcpy_s(d.nume, strlen(aux) + 1, aux);
	aux = strtok(NULL, dem);
	d.tara = (char*)malloc(strlen(aux) + 1);
	strcpy_s(d.tara, strlen(aux) + 1, aux);
	d.nrVizitatori = atoi(strtok(NULL, dem));
	d.recenzie = atof(strtok(NULL, dem));
	return d;
}

int calculeazaGradDeEchilibru(Nod* arbore) {
	if (arbore)
		return calculeazaInaltimeArbore(arbore->st) - calculeazaInaltimeArbore(arbore->dr);
	else
		return 0;
}

void rotireLaDreapta(Nod** arbore) {				// Functie pentru rotirea arborului in partea Dreapta
	if (*arbore) {
		Nod* aux = (*arbore)->st;
		(*arbore)->st = aux->dr;
		aux->dr = (*arbore);
		(*arbore) = aux;
	}
}

void rotireLaStanga(Nod** arbore) {				// Functie pentru rotirea arborului in partea Stanga
	if (*arbore) {
		Nod* aux = (*arbore)->dr;
		(*arbore)->dr = aux->st;
		aux->st = (*arbore);
		(*arbore) = aux;
	}
}

void inserareDestinatieInArbore(Nod** arbore, Destinatie destinatie)		// Functia de inserare a unei Destinatii in Arbore NE-ECHILIBRAT (ABC)
{
	if (*arbore) {
		if ((*arbore)->info.recenzie < destinatie.recenzie) {
			inserareDestinatieInArbore(&((*arbore)->dr), destinatie);
		}
		if ((*arbore)->info.recenzie > destinatie.recenzie) {
			inserareDestinatieInArbore(&((*arbore)->st), destinatie);
		}
	}
	else
	{
		Nod* nou = (Nod*)malloc(sizeof(Nod));
		nou->info = destinatie;		// shallow copy
		nou->dr = NULL;
		nou->st = NULL;
		*arbore = nou;			// CEL MAI IMPORTANT ---> FACEM LEGATURA, SETAM ADRESA ARBORELUI CU NOUL NOD
	}
}

void inserareDestinatieInArboreEchilibrat(Nod** arbore, Destinatie destinatie)		// Functia de inserare a unei Destinatii in Arbore ECHILIBRAT (AVL)
{
	if (*arbore) {
		if ((*arbore)->info.recenzie < destinatie.recenzie) {
			inserareDestinatieInArboreEchilibrat(&((*arbore)->dr), destinatie);
		}
		else {
			inserareDestinatieInArboreEchilibrat(&((*arbore)->st), destinatie);
		}	
		int gradEchilibru = calculeazaGradDeEchilibru(*arbore);
		if (gradEchilibru == 2)			 // avem dezechilibru in partea stanga
		{
			if (calculeazaGradDeEchilibru((*arbore)->st) == 1)				// verificam daca e situatia in care GE este 1
			{
				rotireLaDreapta(arbore);
			}
			else										// verificam daca e situatia in care GE este - 1
			{
				rotireLaStanga(&((*arbore)->st));
				rotireLaDreapta(arbore);
			}
		}
		else if (gradEchilibru == -2) {				// avem dezechilibru in partea dreapta
			if (calculeazaGradDeEchilibru((*arbore)->dr) == -1)									// verificam daca suntem pe situatia GE = -1;
			{
				rotireLaDreapta(&((*arbore)->dr));
			}
			rotireLaStanga(arbore);
		}
	}
	else
	{
		Nod* nou = (Nod*)malloc(sizeof(Nod));
		nou->info = destinatie;		// shallow copy
		nou->dr = NULL;
		nou->st = NULL;
		*arbore = nou;			// CEL MAI IMPORTANT ---> FACEM LEGATURA, SETAM ADRESA ARBORELUI CU NOUL NOD
	}
}

Nod* citireDestinatiiDinFisierInArbore(const char* numeFisier)				// Functie pentru citirea destinatiilor din fisier
{
	FILE* f = fopen(numeFisier, "r");
	Nod* arbore = NULL;
	while (!feof(f))
	{
		Destinatie d = citireDestinatieDinFisier(f);
		inserareDestinatieInArboreEchilibrat(&arbore, d);
	}
	fclose(f);
	return arbore;
}

Destinatie getDestinatieByNrRecenzie(Nod* arbore, float recenzie) {				// Functie de extragere a Destinatiei a carei recenzie este trimisa drept parametru
	if (arbore) {
		if (arbore->info.recenzie == recenzie)
		{
			return arbore->info;
		}

		if (arbore->info.recenzie < recenzie)
		{
			return getDestinatieByNrRecenzie(arbore->dr, recenzie);
		}

		if (arbore->info.recenzie > recenzie)
		{
			return getDestinatieByNrRecenzie(arbore->st, recenzie);
		}
	}
	else
	{
		Destinatie d;
		d.recenzie = -1;
		return d;
	}
}

int calculeazaInaltimeArbore(Nod* arbore)				// Functie pentru calcularea inaltimii arborelui
{
	if (arbore)
	{
		int totalInaltimeStanga = calculeazaInaltimeArbore(arbore->st);
		int totalInaltimeDreapta = calculeazaInaltimeArbore(arbore->dr);
		return 1 + (totalInaltimeStanga > totalInaltimeDreapta ? totalInaltimeStanga : totalInaltimeDreapta);
	}
	return 0;
}

int calculeazaTotalNoduriArbore(Nod* arbore) {				// Functie pentru calcularea numarului total de noduri din arbore
	if (arbore) {
		return 1 + calculeazaTotalNoduriArbore(arbore->st) + calculeazaTotalNoduriArbore(arbore->dr);
	}
	return 0;
}

int calculeazaVizitatoriTotal(Nod* arbore) {		// Functie pentru calcularea numarului total de Vizitatori din toate nodurile
	if (arbore) {
		return arbore->info.nrVizitatori + calculeazaVizitatoriTotal(arbore->st) + calculeazaVizitatoriTotal(arbore->dr);
	}
	return 0;
}

int calculeazaNumarVizitatoriTara(Nod* arbore, const char* numeTara) {			// Functie pentru calcularea numarului total de vizitatori a unei tari a carui nume este trimis ca parametru
	float sum = 0;
	if (arbore) {
		if (strcmp(arbore->info.tara, numeTara) == 0) {
			sum += arbore->info.nrVizitatori;
		}
		sum += calculeazaNumarVizitatoriTara(arbore->st, numeTara);
		sum += calculeazaNumarVizitatoriTara(arbore->dr, numeTara);
		return sum;
	}
	else return sum;
}

void afisareArborePostOrdine(Nod* arbore)	{						// Functie de afisare POST-Ordine a Arborelui
	if (arbore)
	{
		afisareArborePostOrdine(arbore->st);
		afisareArborePostOrdine(arbore->dr);
		afisareDestinatieTuristica(arbore->info);
	}
}

void afisareArborePreOrdine(Nod* arbore) {						// Functie de afisare PRE-Ordine a Arborelui
	if (arbore)
	{
		afisareDestinatieTuristica(arbore->info);
		afisareArborePreOrdine(arbore->st);
		afisareArborePreOrdine(arbore->dr);
	}
}

void afisareArboreInOrdine(Nod* arbore) {						// Functie de afisare IN-Ordine a Arborelui
	if (arbore)
	{
		afisareArboreInOrdine(arbore->st);
		afisareDestinatieTuristica(arbore->info);
		afisareArboreInOrdine(arbore->dr);
	}
}

void afisareIntreagaParcurgereArbore(Nod* arbore)						// Functie pentru afisarea tuturor parcurgerilor 
{
	if (arbore) {
		printf("\n----Afisare Pre-Ordine(RSD): ------\n");
		afisareArborePreOrdine(arbore);
		printf("\n----Afisare In-Ordine(SRD): ------\n");
		afisareArboreInOrdine(arbore);
		printf("\n----Afisare Post-Ordine(SDR): ------\n");
		afisareArborePostOrdine(arbore);
	}
}

void dezalocareArboreDestinatii(Nod** arbore) {				// Dezalocarea arborelui, se face OBLIGATORIU in POST-Ordine(SDR)
	if (*arbore)
	{
		dezalocareArboreDestinatii(&(*arbore)->st);
		dezalocareArboreDestinatii(&(*arbore)->dr);
		if ((*arbore)->info.nume) {
			free((*arbore)->info.nume);
		}
		if ((*arbore)->info.tara) {
			free((*arbore)->info.tara);
		}
		(*arbore)->st = NULL;
		(*arbore)->dr = NULL;
		free(*arbore);
		*arbore = NULL;
	}
}

int main() {

	//Destinatie* destinatii = (Destinatie*)malloc(sizeof(Destinatie) * 5);					// Hardcodam valori in caz ca citirea din fisier nu functioneaza
	//destinatii[0] = initializareDestinatieTuristica("Valencia", "Spain", 50500, 4.89);
	//destinatii[1] = initializareDestinatieTuristica("Bergamo", "Italy", 10500, 4.20);
	//destinatii[2] = initializareDestinatieTuristica("Nice", "France", 25570, 4.75);
	//destinatii[3] = initializareDestinatieTuristica("Koln", "Germany", 30450, 3.90);
	//destinatii[4] = initializareDestinatieTuristica("Praga", "Czech Republic", 87500, 3.65);

	//Nod* arbore = NULL;

	//for (int i = 0; i < 5; i++) {
	//	inserareDestinatieInArbore(&arbore, destinatii[i]);
	//}
	//afisareArborePostOrdine(arbore);

	Nod* arbore = citireDestinatiiDinFisierInArbore("destinatii.txt");				// Testam citirea din fisier a destinatiilor
	afisareIntreagaParcurgereArbore(arbore);

	Destinatie destinatieCautata = getDestinatieByNrRecenzie(arbore, 4.70);			// Testam functia de cautare a Destinatiei dupa recenzie
	printf("\n\n--------Destinatia cautata este: ---------\n");
	afisareDestinatieTuristica(destinatieCautata);

	printf("\nInaltimea arborelui este: %d noduri. \n\n", calculeazaInaltimeArbore(arbore));			// Testam calcularea inaltimii arborelui

	printf("Numarul total de noduri al arborelui este: %d noduri. \n\n", calculeazaTotalNoduriArbore(arbore));			// Testam numarului total de noduri din arbore

	printf("Numarul total de vizitatori din toate Destinatiile este: %d vizitatori. \n\n", calculeazaVizitatoriTotal(arbore));			// Testam numarului total de vizitatori din arbore

	printf("Numarul total de vizitatori al tarii cautate este: %d vizitatori\n\n", calculeazaNumarVizitatoriTara(arbore, "Spania"));		//Testarea functiei de calculare a vizitatorilor unei tari

	dezalocareArboreDestinatii(&arbore);			// Testam daca dezalocarea s-a efectuat cu succes
	printf("\n----Dezalocare: ------\n");
	afisareIntreagaParcurgereArbore(arbore);

	return 0; 
}