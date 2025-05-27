#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;


// Structura pentru STACK, o vom reprezenta prin Lista Simpla (LS)
struct Nod {
	Masina info;
	struct Nod* next;
};
typedef struct Nod Nod;


Masina citireMasinaDinFisier(FILE* file) 
{
	char buffer[100];
	char dem[4] = ",;\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m;
	aux = strtok(buffer, dem);
	m.id = atoi(aux);
	m.nrUsi = atoi(strtok(NULL, dem));
	m.pret = atof(strtok(NULL, dem));
	aux = strtok(NULL, dem);
	m.model = (char*)malloc(strlen(aux) + 1);
	strcpy_s(m.model, strlen(aux) + 1, aux);
	aux = strtok(NULL, dem);
	m.numeSofer = (char*)malloc(strlen(aux) + 1);
	strcpy_s(m.numeSofer, strlen(aux) + 1, aux);
	m.serie = *strtok(NULL, dem);
	return m;
}

void afisareMasina(Masina m) {
	printf("ID: %d\n", m.id);
	printf("Nr usi: %d\n", m.nrUsi);
	printf("Pret: %.2f\n", m.pret);
	printf("Model: %s\n", m.model);
	printf("Nume Sofer: %s\n", m.numeSofer);
	printf("Serie: %c\n\n", m.serie);
}

void pushStack(Nod** stiva, Masina masina) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = masina; // shallow copy
	nou->next = *stiva;
	*stiva = nou;
}

Masina popStack(Nod** stiva)
{
	Masina m;
	m.id = -1;
	if (*stiva) 
	{
		Nod* temp = *stiva;
		m = temp->info;
		*stiva = temp->next;
		free(temp);
		return m;
	}
	return m;
}

void afisareStack(Nod* stack) {
	if (stack == NULL) {
		printf("Stiva este goala.");
	}
	Nod* temp = stack;
	while (temp) {
		afisareMasina(temp->info);
		temp = temp->next;
	}
}

Nod* citireStackMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	Nod* stack = NULL;
	if (f) {
		while (!feof(f)) {
			Masina m = citireMasinaDinFisier(f);
			pushStack(&stack, m);
		}
		return stack;
	}
	return stack;
}

unsigned char isEmptyStack(Nod* stiva) {
	return stiva == NULL;
}

void dezalocareStivaDeMasini(Nod** stiva) {
	while (*stiva) {
		Masina m = popStack(stiva);
		free(m.model);
		free(m.numeSofer);  // nu mai e nevoie sa facem altceva deoarece dezalocarea lui temp si setearea pe NULL se realizeaza in functia popStack()
	}
}

Masina getMasinaByID_Stiva(Nod* stiva, int id) {
	Nod* temp = stiva;
	while (temp != NULL) {
		if (temp->info.id == id) {
			return temp->info; 
		}
		temp = temp->next;
	}
	Masina m;
	m.id = -1; 
	return m;
}

int stackSize(Nod* stack) {
	int count = 0;
	while (stack) {
		count++;
		stack = stack->next;
	}
	return count;
}

//Structura pentru QUEUE (Coada) realizata prin Lista Dubla inlantuita (LD)
struct NodDublu {
	Masina info;
	struct NodDublu* next;
	struct NodDublu* prev;
};
typedef struct NodDublu NodDublu;

struct Coada {
	NodDublu* front;
	NodDublu* rear;
};
typedef struct Coada Coada;

void enqueue(Coada* coada, Masina masina) {
	NodDublu* nou = (NodDublu*)malloc(sizeof(NodDublu));
	nou->info = masina;
	nou->next = NULL;
	nou->prev = coada->rear;
	if (coada->rear)
	{
		coada->rear->next = nou;
	}
	else
	{
		coada->front = nou;
	}
	coada->rear = nou;
}

Masina dequeue(Coada* coada) {
	if (coada->front) {
		Masina m = coada->front->info;
		NodDublu* temp = coada->front;
		coada->front = coada->front->next;
		free(temp);
		if (coada->front == NULL) {
			coada->rear = NULL;
		}
		else {
			coada->front->prev = NULL;
		}
		return m;
	}
	else {
		Masina m;
		m.id = -1;
		return m;
	}
}

Coada citireCoadaDeMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	Coada coada;
	coada.front = NULL;
	coada.rear = NULL;
	while (!feof(f)) {
		Masina m = citireMasinaDinFisier(f);
		enqueue(&coada, m);
	}
	fclose(f);
	return coada;
}

void afisareCoada(Coada coada) {
	NodDublu* p = coada.front;
	if (p == NULL) {
		printf("Coada este goala.\n");
		return;
	}

	while (p) {
		afisareMasina(p->info);
		p = p->next;
	}
}

float calculeazaPretTotal(Nod** stiva) {
	Nod* temp = NULL;
	float suma = 0;
	while (!isEmptyStack(*stiva)) {
		Masina m = popStack(stiva);
		suma += m.pret;
		pushStack(&temp, m);
	}
	while (!isEmptyStack(temp)) {
		pushStack(stiva, popStack(&temp));
	}
	return suma;
}

float calculeazaPretTotalCoada(Coada* coada) {
	float suma = 0;
	Coada aux;
	aux.front = aux.rear = NULL;

	while (coada->front != NULL) {
		Masina m = dequeue(coada);   
		suma += m.pret;              
		enqueue(&aux, m);         
	}

	while (aux.front != NULL) {
		enqueue(coada, dequeue(&aux));
	}

	return suma;
}

Masina getMasinaByID_Coada(Coada coada, int id)
{
	NodDublu* temp = coada.front;
	while (temp) {
		if (temp->info.id == id)
		{
			return temp->info;
		}
		temp = temp->next;
	}
	Masina m;
	m.id = -1;
	return m;
}

void dezalocareCoadaDeMasini(Coada* coada) {
	while (coada->front != NULL) {
		NodDublu* temp = coada->front;
		coada->front = coada->front->next;
		free(temp->info.model);
		free(temp->info.numeSofer);
		free(temp); 
	}
	coada->rear = NULL;
}



int main() {

	// STACK

	Nod* stack = citireStackMasiniDinFisier("masini.txt");
	//Masina m = popStack(&stack); 
	//afisareMasina(m);
	afisareStack(stack);

	printf("\n\nTotal: %.2f", calculeazaPretTotal(&stack));

	printf("\n\nMasina cautata: \n");
	Masina masinaCautataStiva = getMasinaByID_Stiva(stack, 3);
	afisareMasina(masinaCautataStiva);

	printf("\nDezalocare: \n");
	dezalocareStivaDeMasini(&stack);
	afisareStack(stack);


	printf("\n\n----------------------------------------------\n\n");

	// COADA

	Coada coada = citireCoadaDeMasiniDinFisier("masini.txt");
	//Masina m1 = dequeue(&coada);
	afisareCoada(coada);
	
	printf("\n\nTotal: %.2f", calculeazaPretTotal(&coada));

	printf("\n\nMasina cautata: \n");
	Masina masinaCautataCoada = getMasinaByID_Coada(coada, 7);
	afisareMasina(masinaCautataCoada);


	printf("\nDezalocare: \n");
	dezalocareCoadaDeMasini(&coada);
	afisareCoada(coada);




	return 0;
}
