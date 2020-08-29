// Camera_Romana.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define _CRT_SECURE_NO_WARNINGS 
#include <iostream>
#include<stdio.h>
#include<stdlib.h>

struct Camera
{
	char* tip_camera;
	int etaj;
	int nr_camera;
	char* client;
	float pret;
};

struct BinaryTree
{
	int nr_camera;
	Camera info;
	BinaryTree* stanga;
	BinaryTree* dreapta;
};

struct nod
{
	Camera info;
	nod* next;
	nod* prev;
};

Camera creareCamera(char* t, int e, int n, char* c, float p)
{
	Camera camera;
	camera.tip_camera = (char*)malloc(sizeof(char)*(strlen(t) + 1));
	strcpy(camera.tip_camera, t);
	camera.etaj = e;
	camera.nr_camera = n;
	camera.client = (char*)malloc(sizeof(char)*(strlen(c) + 1));
	strcpy(camera.client, c);
	camera.pret = p;
	return camera;
}

BinaryTree* creareNod(int nr_camera, Camera camera)
{
	BinaryTree* nod = (BinaryTree*)malloc(sizeof(BinaryTree));
	nod->dreapta = NULL;
	nod->stanga = NULL;
	nod->info.tip_camera= (char*)malloc(sizeof(char)*(strlen(camera.tip_camera) + 1));
	strcpy(nod->info.tip_camera,camera.tip_camera);
	nod->info.etaj = camera.etaj;
	nod->info.nr_camera = camera.nr_camera;
	nod->info.client= (char*)malloc(sizeof(char)*(strlen(camera.client) + 1));
	strcpy(nod->info.client,camera.client);
	nod->info.pret = camera.pret;
	nod->nr_camera = nr_camera;
	
	return nod;
}

BinaryTree* inserareNod(BinaryTree* radacina, Camera c, int nr_camera )
{
	if (radacina == NULL)
	{
		BinaryTree* nodNou = creareNod(nr_camera, c);
		return nodNou;
	}
	if (radacina->nr_camera == nr_camera)
	{
		return radacina;
	}
	else
		if (radacina->nr_camera < nr_camera)
			radacina->dreapta = inserareNod(radacina->dreapta, c, nr_camera);
		else
			radacina->stanga = inserareNod(radacina->stanga, c, nr_camera);
	return radacina;
}

void afisare_camera(Camera camera)
{
	printf("\nClientul %s are camera %d care are tipul %s, se afla la etajul %d si are pretul %5.2f ", camera.client, camera.nr_camera, camera.tip_camera, camera.etaj, camera.pret);
}

void afisare_inordine(BinaryTree* radacina)
{
	if (radacina) 
	{
		afisare_inordine(radacina->stanga);
		afisare_camera(radacina->info);
		afisare_inordine(radacina->dreapta);
	}
}

void dezalocareArbore(BinaryTree* radacina)
{
	if (radacina)
	{
		dezalocareArbore(radacina->stanga);
		dezalocareArbore(radacina->dreapta);
		free(radacina->info.client);
		free(radacina->info.tip_camera);
		free(radacina);
	}
}

void numarCamerePeEtaj(int* &vectorEtaje, BinaryTree* radacina)
{
	if (radacina)
	{
		numarCamerePeEtaj(vectorEtaje, radacina->stanga);
		vectorEtaje[radacina->info.etaj - 1]++;
		numarCamerePeEtaj(vectorEtaje, radacina->dreapta);
	}
}

int* vectorCamerePeEtaj(BinaryTree* radacina)
{
	int* vector = (int*)malloc(sizeof(int) * 10);
	for (int i = 0; i < 10; i++)
		vector[i] = 0;
	numarCamerePeEtaj(vector, radacina);
	return vector;
}

void creareLista(BinaryTree* radacina,nod* &cap,float pret)
{
	if (radacina)
	{
		creareLista(radacina->stanga, cap,pret);
		if (radacina->info.pret > pret)
		{		
			if (cap == NULL)
			{
				cap = (nod*)malloc(sizeof(nod));
				cap->info.client = (char*)malloc(sizeof(char)*(strlen(radacina->info.client) + 1));
				strcpy(cap->info.client, radacina->info.client);
				cap->info.etaj = radacina->info.etaj;
				cap->info.nr_camera = radacina->info.nr_camera;
				cap->info.tip_camera = (char*)malloc(sizeof(char)*(strlen(radacina->info.tip_camera) + 1));
				strcpy(cap->info.tip_camera, radacina->info.tip_camera);
				cap->info.pret = radacina->info.pret;
				cap->next = NULL;
				cap->prev = NULL;
			}
			else
			{
				nod* cursor = cap;
				while (cursor->next != NULL)
				{
					cursor = cursor->next;
				}
				nod* nou = (nod*)malloc(sizeof(nod));
				nou->info.client = (char*)malloc(sizeof(char)*(strlen(radacina->info.client) + 1));
				strcpy(nou->info.client, radacina->info.client);
				nou->info.etaj = radacina->info.etaj;
				nou->info.nr_camera = radacina->info.nr_camera;
				nou->info.tip_camera = (char*)malloc(sizeof(char)*(strlen(radacina->info.tip_camera) + 1));
				strcpy(nou->info.tip_camera, radacina->info.tip_camera);
				nou->info.pret = radacina->info.pret;
				nou->next = NULL;
				nou->prev = cursor;
				cursor->next = nou;
				cursor = nou;
			}
		}

		creareLista(radacina->dreapta, cap,pret);
	}
}

void afisareLista(nod* cap)
{
	if (cap)
	{
		nod* cursor = cap;
		while (cursor != NULL)
		{
			afisare_camera(cursor->info);
			cursor = cursor->next;
		}
	}
}

void dezalocareLista(nod* cap)
{
	if (cap)
	{		
		nod* cursor = cap;
		while (cursor)
		{
			nod* temp = cursor->next;
			free(cursor->info.client);
			free(cursor->info.tip_camera);
			free(cursor);
			cursor = temp;
		}
			
	}	
}

void inserareElement(nod* &cap, BinaryTree* radacina, const char* client)
{
	if (radacina)
	{
		inserareElement(cap, radacina->stanga, client);
		if (strcmp(radacina->info.client, client) == 0)
		{
			if (cap == NULL)
			{
				cap = (nod*)malloc(sizeof(nod));
				cap->info.client = (char*)malloc(sizeof(char)*(strlen(radacina->info.client) + 1));
				strcpy(cap->info.client, radacina->info.client);
				cap->info.etaj = radacina->info.etaj;
				cap->info.nr_camera = radacina->info.nr_camera;
				cap->info.tip_camera = (char*)malloc(sizeof(char)*(strlen(radacina->info.tip_camera) + 1));
				strcpy(cap->info.tip_camera, radacina->info.tip_camera);
				cap->info.pret = radacina->info.pret;
				cap->next = NULL;
				cap->prev = NULL;
			}
			else
			{
				nod* cursor = cap;
				while (cursor->next != NULL)
					cursor = cursor->next;
				nod* nou = (nod*)malloc(sizeof(nod));
				nou->info.client = (char*)malloc(sizeof(char)*(strlen(radacina->info.client) + 1));
				strcpy(nou->info.client, radacina->info.client);
				nou->info.etaj = radacina->info.etaj;
				nou->info.nr_camera = radacina->info.nr_camera;
				nou->info.tip_camera = (char*)malloc(sizeof(char)*(strlen(radacina->info.tip_camera) + 1));
				strcpy(nou->info.tip_camera, radacina->info.tip_camera);
				nou->info.pret = radacina->info.pret;
				nou->next = NULL;
				nou->prev = cursor;
				cursor->next = nou;
				cursor = nou;
			}
		}
		inserareElement(cap, radacina->dreapta, client);
	}
}

int main()
{
	BinaryTree* radacina = NULL;
	FILE* fisier;
	fopen_s(&fisier, "camera.txt", "r");
	int nr_camere = 0;
	fscanf(fisier, "%d", &nr_camere);
	for (int i = 0; i < nr_camere; i++)
	{
		char bufferTip[50];
		int etaj;
		int nr_camera;
		char bufferClient[50];
		float pret;

		fgetc(fisier);
		fgets(bufferTip, 50, fisier);
		strtok(bufferTip, "\n");
		fscanf(fisier, "%d", &etaj);
		fscanf(fisier, "%d", &nr_camera);
		fgetc(fisier);
		fgets(bufferClient, 50, fisier);
		strtok(bufferClient, "\n");
		fscanf(fisier, "%f", &pret);

		Camera camera = creareCamera(bufferTip, etaj, nr_camera, bufferClient, pret);
		radacina = inserareNod(radacina, camera, nr_camera);
	}
	fclose(fisier);
	afisare_inordine(radacina);

	int* vector = vectorCamerePeEtaj(radacina);
	for (int i = 0; i < 10; i++)
		printf("\nEtajul %d are %d camere", i + 1,vector[i]);

	printf("\nAfisare lista:");
	nod* cap = NULL;
	creareLista(radacina, cap,60);
	afisareLista(cap);

	printf("\nAfisare lista2:");
	inserareElement(cap, radacina, "Popescu Gigel");
	afisareLista(cap);

	dezalocareLista(cap);
	dezalocareArbore(radacina);	

	return 0;
}

