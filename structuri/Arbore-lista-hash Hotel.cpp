// Hotel.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define _CRT_SECURE_NO_WARNINGS 
#include <iostream>
#include<stdio.h>
#include<stdlib.h>

struct Hotel
{
	char* floor;
	int room_number;
	char* room_type;//sau int habar n am
	float price_per_night;
};

struct BinaryTree
{
	char floor[10];
	int room_number;
	Hotel info;
	BinaryTree* stanga;
	BinaryTree* dreapta;
};

Hotel creareHotel(char* f,int n,char* t,float p)
{
	Hotel h;
	h.floor = (char*)malloc(sizeof(char)*(strlen(f) + 1));
	strcpy(h.floor, f);
	h.room_number = n;
	h.room_type = (char*)malloc(sizeof(char)*(strlen(t) + 1));
	strcpy(h.room_type, t);
	h.price_per_night = p;
	return h;
}

struct nod
{
	Hotel info;
	nod* next;
};

struct Hash
{
	int dimensiune;
	nod** elemente;
};

BinaryTree* creareNod(const char* floor1, int room_number, Hotel hotel)
{
	BinaryTree* nod = (BinaryTree*)malloc(sizeof(BinaryTree));
	nod->dreapta=NULL;
	nod->stanga = NULL;
	nod->info.floor = (char*)malloc(sizeof(char)*(strlen(hotel.floor) + 1));
	strcpy(nod->info.floor, hotel.floor);
	nod->info.room_number = hotel.room_number;
	nod->info.room_type = (char*)malloc(sizeof(char)*(strlen(hotel.room_type) + 1));
	strcpy(nod->info.room_type, hotel.room_type);
	nod->info.price_per_night = hotel.price_per_night;
	strcpy(nod->floor, floor1);
	nod->room_number = room_number;

	return nod;
}

BinaryTree* inserareNod(BinaryTree* radacina, Hotel hotel, const char* floor, int room_number)
{
	if (radacina == NULL)
	{
		BinaryTree* nodNou = creareNod(floor, room_number, hotel);
		return nodNou;
	}

	if (strcmp(radacina->floor, floor) == 0 && radacina->room_number == room_number)
	{
		return radacina;
	}
	else
		if (strcmp(radacina->floor, floor) < 0 && radacina->room_number < room_number)
			radacina->dreapta = inserareNod(radacina->dreapta, hotel, floor, room_number);
		else
			radacina->stanga = inserareNod(radacina->stanga, hotel, floor, room_number);
	return radacina;
}

void afisareHotel(Hotel h)
{
	printf("\nCamera %d este la etajul %s de tipul %s cu pretul pe noapte %5.2f ", h.room_number, h.floor, h.room_type, h.price_per_night);
}

void afisareArborePreordine(BinaryTree* radacina)
{
	if (radacina)
	{
		afisareHotel(radacina->info);
		printf(" si cheia compusa din etaj %s si numarul camerei %d", radacina->floor, radacina->room_number);
		afisareArborePreordine(radacina->stanga);
		afisareArborePreordine(radacina->dreapta);
	}
}

void dezalocareArbore(BinaryTree* &radacina)
{
	if (radacina)
	{
		dezalocareArbore(radacina->dreapta);
		dezalocareArbore(radacina->stanga);
		free(radacina->info.floor);
		free(radacina->info.room_type);
		free(radacina);
		radacina = NULL;
	}
}

//BinaryTree* cautare_cheie(BinaryTree* radacina,const char* floor,int room_number)
//{
//	if (radacina)
//	{
//		if (strcmp(radacina->floor, floor) == 0 && radacina->room_number == room_number)
//			return radacina;
//		else
//			if (strcmp(radacina->floor, floor) < 0 && radacina->room_number < room_number)
//				/*radacina->dreapta = */cautare_cheie(radacina->dreapta, floor, room_number);
//			else
//				/*radacina->stanga = */cautare_cheie(radacina->stanga, floor, room_number);
//	}
//	else
//	{
//		printf("\nNu exista cheia cautata!");
//		return NULL;
//	}
//	return radacina;
//}

void extragere_spargere(BinaryTree* &radacina1, const char* floor1, int room_number1, BinaryTree* &radacina2)
{
	if (radacina1)
	{		
		if (strcmp(radacina1->floor, floor1) == 0 && radacina1->room_number == room_number1)
		{
			if (radacina2 == NULL)
			{
				radacina2 = creareNod(floor1, room_number1, radacina1->info);
				radacina1 = NULL;
				return;
			}		
		}
		if (radacina1->dreapta != NULL)
		{
			if (strcmp(radacina1->dreapta->floor, floor1) == 0 && radacina1->dreapta->room_number == room_number1)
			{
				radacina2 = inserareNod(radacina1->dreapta, radacina1->dreapta->info, floor1, room_number1);
				radacina1->dreapta = NULL;
			}
			extragere_spargere(radacina1->dreapta, floor1, room_number1, radacina2);
		}
		if (radacina1->stanga != NULL)
		{
			if (strcmp(radacina1->stanga->floor, floor1) == 0 && radacina1->stanga->room_number == room_number1)
			{
				radacina2 = inserareNod(radacina1->stanga, radacina1->stanga->info, floor1, room_number1);
				radacina1->stanga = NULL;
			}
			else
				extragere_spargere(radacina1->stanga, floor1, room_number1, radacina2);
		}
	}
}

void suma_totala(BinaryTree* radacina, const char* room_type, const char* floor,float &sum)
{
	if (radacina->dreapta)
	{
		suma_totala(radacina->dreapta, room_type, floor, sum);	
	}
	if (radacina->stanga)
	{
		suma_totala(radacina->stanga, room_type, floor,sum);
	}
	if (strcmp(radacina->info.floor, floor) == 0 && strcmp(radacina->info.room_type, room_type) == 0)
	{
		sum+=radacina->info.price_per_night;
	}
}

Hash initializeaza_hash(int nrEtajeHotel)
{
	Hash h;
	h.dimensiune = nrEtajeHotel;
	h.elemente = (nod**)malloc(sizeof(nod*)*h.dimensiune);
	for (int i = 0; i < h.dimensiune; i++)
		h.elemente[i] = NULL;
	return h;
}

void adauga_camera_lista(nod* &cap, Hotel info)
{
	if (cap == NULL)
	{
		cap = (nod*)malloc(sizeof(nod));
		cap->info = info;
		cap->next=NULL;
	}
	else
	{
		nod* cursor = cap;
		while (cursor->next != NULL)
			cursor = cursor->next;
		nod* nou = (nod*)malloc(sizeof(nod));
		nou->next = NULL;
		nou->info = info;
		cursor->next = nou;
	}
}

void inserare_hash(Hash &h, Hotel info)
{
	if(strcmp(info.floor,"I")==0)
		adauga_camera_lista(h.elemente[0], info);
	if (strcmp(info.floor, "II") == 0)
		adauga_camera_lista(h.elemente[1], info);
	if (strcmp(info.floor, "III") == 0)
		adauga_camera_lista(h.elemente[2], info);
	if (strcmp(info.floor, "IV") == 0)
		adauga_camera_lista(h.elemente[3], info);
	if (strcmp(info.floor, "V") == 0)
		adauga_camera_lista(h.elemente[4], info);
	if (strcmp(info.floor, "VI") == 0)
		adauga_camera_lista(h.elemente[5], info);
	if (strcmp(info.floor, "VII") == 0)
		adauga_camera_lista(h.elemente[6], info);
	if (strcmp(info.floor, "VIII") == 0)
		adauga_camera_lista(h.elemente[7], info);
	if (strcmp(info.floor, "IX") == 0)
		adauga_camera_lista(h.elemente[8], info);
	if (strcmp(info.floor, "X") == 0)
		adauga_camera_lista(h.elemente[9], info);
}

void arbore_to_hash(BinaryTree* radacina, Hash h)
{
	if (radacina)
		inserare_hash(h, radacina->info);
	if (radacina->stanga)
		arbore_to_hash(radacina->stanga, h);
	if (radacina->dreapta)
		arbore_to_hash(radacina->dreapta, h);
}

void afisare_lista(nod* cap)
{
	if (cap != NULL)
	{
		nod* cursor = cap;
		while (cursor != NULL)
		{
			afisareHotel(cursor->info);
			cursor = cursor->next;
		}
	}
}

void afisare_hash(Hash h)
{
	for (int i = 0; i < h.dimensiune; i++)
	{
		printf("\nEtajul %d:\n", i + 1);
		afisare_lista(h.elemente[i]);
	}
}

int inaltime_arbore(BinaryTree* radacina)
{
	if (radacina) 
	{
		int inaltime_stanga = inaltime_arbore(radacina->stanga);
		int inaltime_dreapta = inaltime_arbore(radacina->dreapta);
		return 1 + (inaltime_stanga > inaltime_dreapta ? inaltime_stanga : inaltime_dreapta);
	}
	else 
	{
		return 0;
	}
}

void afisare_pe_nivel(BinaryTree* radacina, int nivelCautat, int nivelCurent)
{
	if (radacina)
	{
		if (nivelCautat == nivelCurent) 
		{
			afisareHotel(radacina->info);
		}
		else
		{
			afisare_pe_nivel(radacina->stanga, nivelCautat, nivelCurent + 1);
			afisare_pe_nivel(radacina->dreapta, nivelCautat, nivelCurent + 1);
		}
	}
}

void dezalocare_lista(nod* cap)
{
	if (cap)
	{
		cap = cap->next;
		//free(cap->info.floor);
		//free(cap->info.room_type);
		free(cap);
	}
}

void dezalocare_hash(Hash h)
{
	for (int i = 0; i < h.dimensiune; i++)
	{
		dezalocare_lista(h.elemente[i]);
	}
}

int main()
{
	BinaryTree* radacina1=NULL;
	FILE* fisier;
	fopen_s(&fisier, "hotel.txt", "r");
	if (fisier == NULL)
	{
		printf("\nNu se poate deschide fisierul!");
	}
	int nrHoteluri;
	fscanf(fisier, "%d", &nrHoteluri);
	for (int i = 0; i < nrHoteluri; i++)
	{
		char bufferFloor[10];
		int room_number;
		char bufferRoomType[50];
		float price_per_night;

		fgetc(fisier);
		fgets(bufferFloor, 10, fisier);
		strtok(bufferFloor, "\n");
		fscanf(fisier, "%d", &room_number);
		fgetc(fisier);
		fgets(bufferRoomType, 50, fisier);
		strtok(bufferRoomType, "\n");
		fscanf(fisier, "%f", &price_per_night);

		Hotel h = creareHotel(bufferFloor, room_number, bufferRoomType, price_per_night);
		radacina1 = inserareNod(radacina1, h, bufferFloor, room_number);
	}
	fclose(fisier);
	printf("\nAfisare arbore:");
	afisareArborePreordine(radacina1);
	//BinaryTree* radacina2 = NULL;
	///*radacina1 = */extragere_spargere(radacina1, "VIII", 29, radacina2);
	//printf("\nAfisare arbore1:");
	//afisareArborePreordine(radacina1);
	//printf("\nAfisare arbore2:");
	//afisareArborePreordine(radacina2);

	//BinaryTree* radacina3 = NULL;
	//radacina3 = cautare_cheie(radacina1, "VIII", 29);
	//printf("\n Arbore cautat dupa cheie:");
	//afisareArborePreordine(radacina3);

	//BinaryTree* radacina4 = NULL;
	//radacina4 = cautare_cheie(radacina1, "II", 7);
	//printf("\n Arbore cautat dupa cheie:");
	//afisareArborePreordine(radacina4);

	BinaryTree* radacina2 = NULL;
	extragere_spargere(radacina1, "VIII", 29, radacina2);
	printf("\nArbore1:");
	afisareArborePreordine(radacina1);
	printf("\nArbore2:");
	afisareArborePreordine(radacina2);

	BinaryTree* radacina3 = NULL;

	FILE* fisier1;
	fopen_s(&fisier1, "hotel.txt", "r");
	if (fisier1 == NULL)
	{
		printf("\nNu se poate deschide fisierul!");
	}
	int nrHoteluri1;
	fscanf(fisier1, "%d", &nrHoteluri1);
	for (int i = 0; i < nrHoteluri1; i++)
	{
		char bufferFloor[10];
		int room_number;
		char bufferRoomType[50];
		float price_per_night;

		fgetc(fisier1);
		fgets(bufferFloor, 10, fisier1);
		strtok(bufferFloor, "\n");
		fscanf(fisier1, "%d", &room_number);
		fgetc(fisier1);
		fgets(bufferRoomType, 50, fisier1);
		strtok(bufferRoomType, "\n");
		fscanf(fisier1, "%f", &price_per_night);

		Hotel h = creareHotel(bufferFloor, room_number, bufferRoomType, price_per_night);
		radacina3 = inserareNod(radacina3, h, bufferFloor, room_number);
	}
	
		
	float sum = 0;
	suma_totala(radacina3, "type 3", "IX",sum);
	printf("\nSuma este:%5.2f",sum);

	int inaltimeArbore = inaltime_arbore(radacina3);
	for (int i = 0; i < inaltimeArbore; i++)
	{
		printf("\nNivelul %d", i);
		afisare_pe_nivel(radacina3, i, 0);
	}


	Hash h = initializeaza_hash(10);
	arbore_to_hash(radacina3, h);
	afisare_hash(h);
	   

	

	dezalocareArbore(radacina1);
	dezalocareArbore(radacina2);
	dezalocareArbore(radacina3);
	dezalocare_hash(h);
	return 0;
}

