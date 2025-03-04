#include<stdio.h>
#include<stdlib.h>

// Adaugare commit 

struct Masina {
	int id;
	int anFabricatie;
	char* sofer;
	float kilometriiParcursi;
	char initialaProducator;
};

struct Masina initializare(int id, int anFabricatie, const char* sofer, float kilometriiParcursi, char initialaProducator) {
	struct Masina m;
	m.id = 1;
	m.anFabricatie = anFabricatie;
	m.sofer = (char*)malloc(strlen(sofer) + 1);
	strcpy_s(m.sofer, strlen(sofer) + 1, sofer);
	m.kilometriiParcursi = kilometriiParcursi;
	m.initialaProducator = initialaProducator;
	return m;
}

void afisare(struct Masina m) {
	printf("ID: %d \n", m.id);
	printf("An fabricatie: %d \n", m.anFabricatie);
	printf("Sofer: %s \n", m.sofer);
	printf("Kilometri: %f \n", m.kilometriiParcursi);
	printf("Initiala Producator: %c \n", m.initialaProducator);
}

void modifica_Sofer(struct Masina m, const char* nouSofer) {
	if (strlen(nouSofer) >= 2)
	{
		if (m.sofer != NULL)
		{
			free(m.sofer);
		}
		m.sofer = (char*)malloc(strlen(nouSofer) + 1);
		strcpy_s(m.sofer, strlen(nouSofer) + 1, nouSofer);
	}
}

void dezalocare(struct Masina* m) {
	if (m->sofer != NULL)
	{
		free(m->sofer);
		m->sofer = NULL;
	}
}

int main() {
	struct Masina masina;
	masina = initializare(1, 2004, "Stefan", 2000, 'S');
	afisare(masina);
	modifica_Sofer(masina, "Marian");
	printf("\n");
	afisare(masina);
	dezalocare(&masina);
	printf("\n");
	afisare(masina);
	return 0;
}