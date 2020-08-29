#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>



struct student
{
	int varsta;
	char* nume;
	float medie;
};

struct nodCoada
{
	student inf;
	nodCoada* next;
};


student citire_tastatura()
{
	student s;
	printf("\nVarsta: ");
	scanf_s("%d", &s.varsta);
	printf("\n Nume: ");
	getchar();
	char buffer[30];
	gets_s(buffer, 30);
	int dim = strlen(buffer) + 1;
	s.nume = (char*)malloc(sizeof(char) * dim);
	strcpy_s(s.nume, dim, buffer);
	printf("Medie: ");
	scanf_s("%f", &s.medie);
	return s;
}
void put(nodCoada** prim, nodCoada** ultim, student s)
{
	nodCoada* nou = (nodCoada*)malloc(sizeof(nodCoada));
	nou->inf.varsta = s.varsta;
	nou->inf.nume = (char*)malloc((strlen(s.nume) + 1) * sizeof(char));
	strcpy(nou->inf.nume, s.nume);
	nou->inf.medie = s.medie;
	nou->next = NULL;
	if (*prim == NULL && *ultim == NULL)
	{
		*prim = nou;
		*ultim = nou;
	}
	else {
		(*ultim)->next = nou;
		*ultim = nou;
	}
}

void numara(nodCoada* prim, int& nr)
{
	nodCoada* cursor = prim;
	while (cursor != NULL)
	{
		cursor = cursor->next;
		nr++;
	}
}

int get(nodCoada** prim, nodCoada** ultim, student* s)
{
	if (*prim != NULL && *ultim != NULL)
	{

			(*s).varsta = (*prim)->inf.varsta;
			(*s).nume = (char*)malloc((strlen((*prim)->inf.nume) + 1) * sizeof(char));
			strcpy((*s).nume, (*prim)->inf.nume);
			(*s).medie = (*prim)->inf.medie;
			nodCoada* temp = *prim;
			*prim = (*prim)->next;
			free(temp->inf.nume);
			free(temp);
			return 0;
		
	}
	else if (*prim == NULL)
	{
		*ultim = NULL;
		return -1;
	}
}

void traversare(nodCoada* prim)
{
	nodCoada* temp = prim;
	while (temp)
	{
		printf("\nVarsta=%d, Nume=%s, Medie=%5.2f", temp->inf.varsta, temp->inf.nume, temp->inf.medie);
		temp = temp->next;
	}
}

student citire_fisier(FILE* fisier)
{
	student s;
	fscanf_s(fisier, "%d", &s.varsta);
	char buffer[30];
	fgetc(fisier);
	fgets(buffer, 30, fisier);
	strtok(buffer, "\n");
	int dim = strlen(buffer) + 1;
	s.nume = (char*)malloc(sizeof(char) * dim);
	strcpy(s.nume, buffer);
	fscanf_s(fisier, "%f", &s.medie);
	return s;
}

void initializare_coada(nodCoada** primul, nodCoada** ultimul) {


	FILE* fisier;
	fopen_s(&fisier, "fisier.txt", "r");
	if (fisier != NULL)
	{
		int nr = 0;
		fscanf_s(fisier, "%d", &nr);
		if (nr > 0) {
			
			for (int i = 1; i <= nr; i++) {
				student s = citire_fisier(fisier);
				put(primul, ultimul, s);
			}

		}
		fclose(fisier);

	}
	else {
		printf("Fisierul nu se poate deschide");
	}
}

void conversieCoadaVector(nodCoada** prim, nodCoada** ultim, student* vect, int* nr)
{
	student s;
	while (get(prim, ultim, &s) == 0)
	{
		vect[*nr] = s;
		(*nr)++;
	}
}

void main()
{
	nodCoada* primul_c = NULL;
	nodCoada* ultimul_c = NULL;
	initializare_coada(&primul_c, &ultimul_c);
	traversare(primul_c);
	student s=citire_tastatura();
	printf("\n----Adaugare student in coada----");
	put(&primul_c, &ultimul_c, s);
	traversare(primul_c);
	printf("\n----Stergere student din coada----");
	get(&primul_c, &ultimul_c, &s);
	traversare(primul_c);

	int nr = 0;
	numara(primul_c, nr);
	student* vect = (student*)malloc(nr * sizeof(student));
	nr = 0;
	conversieCoadaVector(&primul_c, &ultimul_c, vect, &nr);
	printf("\n----Vector-----");
	for (int i = 0; i < nr; i++)
		printf("\nVarsta=%d, Nume=%s, Medie=%5.2f",
			vect[i].varsta, vect[i].nume, vect[i].medie);
}