#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//Structura pentru Artist
struct Artist {				
	char* nume;
	int nrConcerte;
	float popularitate; // exprimat in procente
	char* gen;
};
typedef struct Artist Artist;

// Structura pentru HEAP
struct Heap {
	Artist* vector;				// un vector dinamic de Artisti
	int lungime;				// lungimea intregului heap-ului (inclusiv artistii ascunsi)
	int nrArtisti;				// numarul real de artisti prezenti in structura la momentul dat
};
typedef struct Heap Heap;
	
void afisareArtist(Artist a) {						// afisarea unui artist
	printf("Nume: %s\n", a.nume);
	printf("Nr concerte: %d\n", a.nrConcerte);
	printf("Popularitate: %.2f\n", a.popularitate);
	printf("Gen muzical: %s\n\n", a.gen);
}

Artist citireArtistDinFisier(FILE* file)			// citim un artist din fisier 
{
	char buffer[100];
	char dem[4] = ",;\n";
	fgets(buffer, 100, file);
	char* aux;
	Artist a;
	aux = strtok(buffer, dem);
	a.nume = (char*)malloc(strlen(aux) + 1);
	strcpy_s(a.nume, strlen(aux) + 1, aux);
	aux = strtok(NULL, dem);
	a.nrConcerte = atoi(aux);
	aux = strtok(NULL, dem);
	a.popularitate = atof(aux);
	aux = strtok(NULL, dem);
	a.gen = (char*)malloc(strlen(aux) + 1);
	strcpy_s(a.gen, strlen(aux) + 1, aux);
	return a;
}

void filtreazaHeap(Heap heap, int pozitie) 
{
	int pozFiuSt = 2 * pozitie + 1;					// calculam pozitia fiului de stanga, respectiv de dreapta
	int pozFiuDr = 2 * pozitie + 2;
	int pozMax = pozitie;							// presupunem ca pozitia maxima este chiar cea primita
	if (pozFiuSt < heap.nrArtisti && heap.vector[pozFiuSt].popularitate > heap.vector[pozMax].popularitate) 
	{
		pozMax = pozFiuSt;
	}
	if (pozFiuDr < heap.nrArtisti && heap.vector[pozFiuDr].popularitate > heap.vector[pozMax].popularitate)
	{
		pozMax = pozFiuDr;
	}
	if (pozMax != pozitie)					// daca premisa este gresita, facem interschimbarea
	{
		Artist aux = heap.vector[pozMax];
		heap.vector[pozMax] = heap.vector[pozitie];
		heap.vector[pozitie] = aux;
		if (pozMax <= (heap.nrArtisti - 2) / 2) // plecand de la ultimul nod parinte, facem filtrarea prin autoapelul functiei 
		{
			filtreazaHeap(heap, pozMax);
		}
	}
}

void afisareHeap(Heap heap)				// functie de afisare a heap-ului
{
	if (heap.nrArtisti > 0) {
		for (int i = 0; i < heap.nrArtisti; i++) {
			afisareArtist(heap.vector[i]);
		}
	}
	else
		printf("Heap-ul este gol.\n");
}

void afisareHeapAscuns(Heap heap)		// functie pentru afisarea heap-ului ascuns
{
	if (heap.nrArtisti > 0) {
		for (int i = heap.nrArtisti; i < heap.lungime; i++)
		{
			afisareArtist(heap.vector[i]);
		}
	}
	else
		printf("Nu exista artisti ascunsi.");
}

Heap initializareHeap(int lungime) {			// functie de initializare a heap-ului
	Heap heap;
	heap.lungime = lungime;
	heap.nrArtisti = 0;
	heap.vector = (Artist*)malloc(sizeof(Artist) * lungime);
	return heap;
}

Heap citireArtistiDinFisierInHeap(const char* numeFisier) {				// functia de citire a artistilor din fisier
	FILE* f = fopen(numeFisier, "r");
	Heap heap = initializareHeap(5);
	while (!feof(f))
	{
		Artist artist = citireArtistDinFisier(f);
		heap.vector[heap.nrArtisti] = artist; // shallow copy
		heap.nrArtisti++;
	}
	fclose(f);

	for (int i = (heap.nrArtisti - 2) / 2; i >= 0; i--)					// dupa ce citim din fisier facem filtrarea si returnam heap-ul
	{
		filtreazaHeap(heap, i);
	}
	return heap;
}

Artist extragereHeap(Heap* heap)
{
	if (heap && heap->nrArtisti > 0)		// verificam daca avem heap
	{
		Artist aux = heap->vector[0];								// facem interschimbarea intre primul artist si ultimul din heap 
		heap->vector[0] = heap->vector[heap->nrArtisti - 1];
		heap->vector[heap->nrArtisti - 1] = aux;
		heap->nrArtisti--;											// decrementam numarul de artisti  *ca sa il ascundem*
		for (int i = (heap->nrArtisti - 2) / 2; i >= 0; i--) {
			filtreazaHeap(*heap, i);								// facem filtrarea heap-ului plecand de la ultimul nod parinte (heap->nrArtisti - 2) / 2
		}
		return aux;
	}
	else
	{
		Artist a;
		a.nrConcerte = -1;
		return a;
	}
}

void dezalocareHeap(Heap* heap) {						// la final, dezalocam intreaga structura, cat si membri alocati dinamic
	if (heap && heap->vector) {
		for (int i = 0; i < heap->lungime; i++) {
			if (heap->vector[i].nume) {
				free(heap->vector[i].nume);
			}
			if (heap->vector[i].gen) {
				free(heap->vector[i].gen);
			}
		}
		free(heap->vector);
		heap->lungime = 0;
		heap->nrArtisti = 0;
	}
	else
		printf("Heap-ul nu este alocat.");
}

int main()
{
	Heap heap = citireArtistiDinFisierInHeap("artisti.txt");			// Testam citirea din fisier si afisam intregul heap
	afisareHeap(heap);


	printf("\nArtistul extras este: \n");				
	afisareArtist(extragereHeap(&heap));						// Testam daca artistul extras este cel cu popularitatea cea mai mare

	afisareArtist(extragereHeap(&heap));				// Extragem si restul artistilor pentru exemplificarea afisarii celor ascunsi
	afisareArtist(extragereHeap(&heap));
	afisareArtist(extragereHeap(&heap));

	printf("\nHeap-ul ascuns: \n\n");
	afisareHeapAscuns(heap);						// Testam afisarea heap-ului ascuns, o sa fie afisati in ordine inversa

	printf("\n\nDezalocare heap: \n");				// Testam daca structura s-a dezalocat cu succes
	dezalocareHeap(&heap);
	afisareHeap(heap);				 

	return 0;
}