#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
using namespace std;

struct AparatElectrocasnic {
	float pret;
	int greutate;
	char *firma;
	char *model;
};

struct nodarb {
	AparatElectrocasnic inf;
	nodarb *left, *right;
};

AparatElectrocasnic citireAparat(FILE* f) {

	AparatElectrocasnic a;
	char buffer[20];

	fscanf(f, "%s", buffer);
	a.firma = (char*)malloc(sizeof(char)*(strlen(buffer) + 1));
	strcpy(a.firma, buffer);

	fscanf(f, "%s", buffer);
	a.model = (char*)malloc(sizeof(char)*(strlen(buffer) + 1));
	strcpy(a.model, buffer);

	fscanf(f, "%f", &a.pret);

	fscanf(f, "%d", &a.greutate);

	return a;
}

void inserare(nodarb* &rad, AparatElectrocasnic a) {
	if (rad) {
		if (a.pret < rad->inf.pret) {
			inserare(rad->left, a);
		}
		else if (a.pret > rad->inf.pret) {
			inserare(rad->right, a);
		}else{
			throw "Apartul exista in arbore";
		}
	}
	else {
		nodarb* nou = (nodarb*)malloc(sizeof(nodarb));
		nou->inf = a;
		nou->left = NULL;
		nou->right = NULL;
		rad = nou;
	}
}

nodarb* citireArbore(const char* fileName) {
	FILE* f = fopen(fileName, "r");
	if (f) {
		nodarb *rad = NULL;
		while (!feof(f)) {
			AparatElectrocasnic a = citireAparat(f);
			inserare(rad, a);
		}
		fclose(f);
		return rad;
	}
	else {
		printf("Fisierul nu a fost deschis");
	}
}

void afisareArbore(nodarb *rad) { //INORDINE stanga radacina dreapta
	if (rad) {
		afisareArbore(rad->left);
		printf("\nFirma = %s, Model = %s, Pret = %5.2f, Greutate = %d", rad->inf.firma, rad->inf.model,rad->inf.pret, rad->inf.greutate);
		afisareArbore(rad->right);
	}
}

float pretMinim(nodarb *rad, float *pmin) {
	if (rad != NULL) {
		if (rad->inf.pret < *(pmin))
			*(pmin) = rad->inf.pret;
		pretMinim(rad->left, pmin);
		pretMinim(rad->right, pmin);
		return *(pmin);
	}
}

float pretMaxim(nodarb *rad, float *pmax) {
	if (rad != NULL) {
		if (rad->inf.pret > *(pmax))
			*(pmax) = rad->inf.pret;
		pretMaxim(rad->left, pmax);
		pretMaxim(rad->right, pmax);
		return *(pmax);
	}
}

void afisareAparat(AparatElectrocasnic a) {
	printf("Firma = %s, Model = %s, Pret = %5.2f, Greutate = %d", a.firma, a.model, a.pret, a.greutate);
}

nodarb *stergeRad(nodarb *rad)
{
	nodarb *aux = rad;
	if (aux->left != NULL)
	{
		rad = aux->left;
		if (aux->right != NULL)
		{
			nodarb* temp = aux->left;
			while (temp->right)
				temp = temp->right;
			temp->right = aux->right;
		}
	}
	else
		if (aux->right != NULL)
			rad = aux->right;
		else
			rad = NULL;
	printf("Aparatul sters este asta = ");
	afisareAparat(aux->inf);
	free(aux->inf.firma);
	free(aux->inf.model);
	free(aux);
	return rad;
}


nodarb* stergeNod(nodarb *rad, int cheie)
{
	if (rad == NULL)
		return NULL;
	else
		if (rad->inf.pret == cheie)
		{
			rad = stergeRad(rad);
			return rad;
		}
		else
		{
			nodarb *aux = rad;
			while (true)
			{
				if (cheie < aux->inf.pret)
					if (aux->left == NULL)
						break;
					else
						if (aux->left->inf.pret == cheie)
							aux->left = stergeRad(aux->left);
						else
							aux = aux->left;
				else
					if (cheie > aux->inf.pret)
						if (aux->right == NULL)
							break;
						else
							if (aux->right->inf.pret == cheie)
								aux->right = stergeRad(aux->right);
							else
								aux = aux->right;

			}
			return rad;
		}
}

AparatElectrocasnic cautareNod(nodarb* rad, int cheie)
{
	if (rad != NULL) {
		if (cheie == rad->inf.pret)
			return rad->inf;
		else
			if (cheie < rad->inf.pret)
				return cautareNod(rad->left, cheie);
			else
				return cautareNod(rad->right, cheie);
	}

}

void modificarePret(nodarb *rad) {
	float *p = (float*)malloc(sizeof(float));
	*p = 0;
	float *pmin = (float*)malloc(sizeof(float));
	*pmin = 99999;
	
	AparatElectrocasnic aux = cautareNod(rad, pretMaxim(rad, p));
	aux.firma = (char*)malloc(sizeof(char));
	strcpy(aux.firma, cautareNod(rad, pretMaxim(rad, p)).firma);
	aux.model = (char*)malloc(sizeof(char));
	strcpy(aux.model, cautareNod(rad, pretMaxim(rad, p)).model);
	stergeNod(rad, aux.pret);
	aux.pret = pretMinim(rad, pmin) - 1;
	inserare(rad, aux);
	
}

struct nodls {
	AparatElectrocasnic inf;
	nodls *next;
};

nodls* inserareLista(nodls* cap, AparatElectrocasnic a) {
	nodls* nou = (nodls*)malloc(sizeof(nodls));
	nou->inf.pret = a.pret;
	nou->inf.model = (char*)malloc((strlen(a.model) + 1) * sizeof(char));
	strcpy(nou->inf.model, a.model);
	nou->inf.firma = (char*)malloc((strlen(a.firma) + 1) * sizeof(char));
	strcpy(nou->inf.firma, a.firma);
	nou->inf.greutate = a.greutate;

	nou->next = NULL;

	if (cap == NULL)
		cap = nou;
	else {
		nodls* temp = cap;
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = nou;
	}
	return cap;
}

void conversieArboreLista(nodarb *rad, nodls** cap) {
	if (rad != NULL) {
		(*cap) = inserareLista(*cap, rad->inf);
		conversieArboreLista(rad->left, cap);
		conversieArboreLista(rad->right, cap);
	}
}


void dezalocareArbore(nodarb *rad) {
	if (rad != NULL) {
		nodarb *st = rad->left;
		nodarb *dr = rad->right;
		free(rad->inf.firma);
		free(rad->inf.model);
		free(rad);
		dezalocareArbore(rad->left);
		dezalocareArbore(rad->right);
	}


}

void dezalocareLista(nodls* cap)
{
	nodls* temp = cap;
	while (temp != NULL)
	{
		nodls *temp2 = temp->next;
		free(temp->inf.firma);
		free(temp->inf.model);
		free(temp);
		temp = temp2;
	}
}

void main() {
	nodarb *rad = NULL;
	rad = citireArbore("aparat.txt");
	printf("Exercitiul 1: ");
	afisareArbore(rad);

	printf("\n\n\n Exercitiul 2: ");
	float *pmin = (float*)malloc(sizeof(float));
	*pmin = 99999;
	printf("\nPretul minim este: %5.2f", pretMinim(rad, pmin));
	free(pmin);

	//printf("\n\n\n Exercitiul 3: ");
	float *pmax = (float*)malloc(sizeof(float));
	*pmax = 0;
	
	//
	//printf("\nPretul maxim este: %5.2f", pretMaxim(rad, pmax));
	//printf("\n\n\n\n\n");
	//stergeNod(rad, pretMaxim(rad, pmax));
	//afisareArbore(rad); // NU STIU DACA E BINE JEGUL ASTA CU DE EX. CA NU ANTELEG ENUNTZUL

	// DECOMENTATI-L VOI DAK VRETI CA EL MERGE

	printf("\n\n\n Exercitiul 4: \n"); //BAFTA SA INTELEGI ASTA DRAGILOR CA NICI EU NU AM PREA INTELES CE AM FACUT ACOLO
	modificarePret(rad);
	afisareArbore(rad);


	printf("\n\n\n Exercitiul 5: \n");
	printf("CONVERSIE ARBORE->LISTA:\n");
	nodls *cap = NULL;
	conversieArboreLista(rad, &cap);
	nodls* temp = cap;
	while (temp) {
		printf("\nFirma = %s, Model = %s, Pret = %5.2f, Greutate = %d", temp->inf.firma, temp->inf.model, temp->inf.pret, temp->inf.greutate);
		temp = temp->next;
	}

	printf("\n\n\n Exercitiul 6: \n");
	printf("Gata si alte chestii");
	dezalocareArbore(rad);
	dezalocareLista(cap);
	printf("Gata si alte chestii");



}