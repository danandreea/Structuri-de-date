#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <malloc.h>
#include <string.h>

struct Librarie {
	int versiune;
	int tip;
	char* denumire;
};

struct nodBST {
	char cod[50];
	Librarie librarie;
	nodBST* st = NULL;
	nodBST* dr = NULL;
};

struct nodBSTArbore2 {
	char cod[50];
	Librarie* infoNodInitial;
	nodBSTArbore2* st;
	nodBSTArbore2* dr;
};

Librarie creareLibrarie(int v, int t, const char* d) {
	Librarie librarie;
	librarie.tip = t;
	librarie.versiune = v;
	librarie.denumire = (char*)malloc(strlen(d) + 1);
	strcpy(librarie.denumire, d);
	return librarie;
}

nodBST* creareNod(const char* cod, Librarie librarie) {
	nodBST* nod = (nodBST*)malloc(sizeof(nodBST));
	nod->dr = NULL;
	nod->st = NULL;
	//nod->librarie = librarie;	//destul de unsafe - shallow copy
	nod->librarie = creareLibrarie(
		librarie.versiune, librarie.tip, librarie.denumire);
	strcpy(nod->cod, cod);
	return nod;
}

nodBST* inserare(nodBST* root, Librarie librarie, const char* cod) {

	if (root == NULL) {
		nodBST* nodNou = creareNod(cod, librarie);
		return nodNou;
	}

	if (strcmp(root->cod, cod) == 0) {
		//cheia exista in arbore
		return root;
	}
	else
		if (strcmp(root->cod, cod) < 0)
			root->dr = inserare(root->dr, librarie, cod);
		else
			root->st = inserare(root->st, librarie, cod);
	return root;
}

nodBST* citireFisierInBST(const char* numeFisier) {
	FILE* pf = fopen(numeFisier, "r");

	if (pf == NULL) {
		printf("EROARE deschidere fisier");
		return NULL;
	}

	int nrLibrarii;
	fscanf(pf, "%d", &nrLibrarii);

	nodBST* root = NULL;

	for (int i = 0; i < nrLibrarii; i++) {
		char bufferCod[100];
		int versiune;
		int tip;
		char bufferDenumire[100];

		fscanf(pf, "%s", bufferCod);
		fscanf(pf, "%d", &versiune);
		fscanf(pf, "%d", &tip);
		fscanf(pf, "%s", bufferDenumire);

		Librarie librarie = creareLibrarie(versiune, tip, bufferDenumire);
		root = inserare(root, librarie, bufferCod);
	}

	fclose(pf);

	return root;
}

void afisareNod(nodBST* nod) {
	printf("\n Cod nod = %s", nod->cod);
	printf("\n Cod nod = %d", nod->librarie.versiune);
	printf("\n Cod nod = %d", nod->librarie.tip);
	printf("\n Cod nod = %s", nod->librarie.denumire);
	printf("\n ------------");
}

void inordine(nodBST* root) {
	if (root != NULL) {
		inordine(root->st);
		afisareNod(root);
		if (root->st == NULL && root->dr == NULL)
			printf("!!!!!!!!! Este frunza");
		inordine(root->dr);
	}
}

nodBSTArbore2* inserare(nodBSTArbore2* root, Librarie* adrLibrarie, const char* cod) {

	if (root == NULL) {
		nodBSTArbore2* nodNou = (nodBSTArbore2*)malloc(sizeof(nodBSTArbore2));
		nodNou->dr = NULL;
		nodNou->st = NULL;
		strcpy(nodNou->cod, cod);
		nodNou->infoNodInitial = adrLibrarie;
		return nodNou;
	}

	if (strcmp(root->cod, cod) == 0) {
		//cheia exista in arbore
		return root;
	}
	else
		if (strcmp(root->cod, cod) < 0)
			root->dr = inserare(root->dr, adrLibrarie, cod);
		else
			root->st = inserare(root->st, adrLibrarie, cod);
	return root;
}

//cerinta 2
nodBSTArbore2* copiereArboreInitial(nodBST* root, int filtruTip, nodBSTArbore2* root2) {
	if (root != NULL) {
		root2 = copiereArboreInitial(root->st, filtruTip, root2);
		if (root->librarie.tip == filtruTip) {
			root2 = inserare(root2, &root->librarie, root->cod);
		}
		root2 = copiereArboreInitial(root->dr, filtruTip, root2);
	}
	return root2;
}

void afisareNodArbore2(nodBSTArbore2* nod) {
	printf("\n Cod nod = %s", nod->cod);
	printf("\n versiune nod = %d", nod->infoNodInitial->versiune);
	printf("\n Tip nod = %d", nod->infoNodInitial->tip);
	printf("\n Denumire nod = %s", nod->infoNodInitial->denumire);
	printf("\n ------------");
}

void inordine(nodBSTArbore2* root) {
	if (root != NULL) {
		inordine(root->st);
		afisareNodArbore2(root);
		inordine(root->dr);
	}
}

//cerinta 3
struct nodLista {
	Librarie librarie;
	nodLista* next;
};

nodLista* creareNodLista(Librarie librarie) {
	nodLista* nod = (nodLista*)malloc(sizeof(nodLista));
	nod->next = NULL;
	nod->librarie = creareLibrarie(
		librarie.versiune, librarie.tip, librarie.denumire);
	return nod;
}

nodLista* inserareSfarsit(nodLista* capat, Librarie librarie) {

	nodLista* nou = creareNodLista(librarie);

	if (capat == NULL) {
		return nou;
	}

	nodLista* temp = capat;
	while (temp->next != NULL) {
		temp = temp->next;
	}
	temp->next = nou;

	return capat;
}

void afisareLista(nodLista* capat) {
	if (capat == NULL)
		printf("\n Lista este vida");
	while (capat != NULL) {
		printf("\n Librarie cu versiunea %d si denumirea %s",
			capat->librarie.versiune, capat->librarie.denumire);
		capat = capat->next;
	}
}

nodBST* getMaximStanga(nodBST* root) {
	if (root == NULL)
		return NULL;

	nodBST* fiuSt = root->st;

	if (fiuSt == NULL)
		return NULL;

	while (fiuSt->dr != NULL) {
		fiuSt = fiuSt->dr;
	}

	return fiuSt;
}

nodBST* stergereNodClasic(nodBST* root, const char* cod) {
	if (root == NULL)
		return NULL;

	if (strcmp(root->cod, cod) == 0) {
		if (root->st == NULL && root->dr == NULL) {
			free(root->librarie.denumire);	//fara memory leak
			free(root);
			return NULL;
		}
		else
		{
			if (root->st == NULL) {
				//avem fiu pe dreapta
				nodBST* temp = root->dr;
				free(root->librarie.denumire);	//fara memory leak
				free(root);
				return temp;
			}
			else if (root->dr == NULL) {
				//avem fiu pe stanga
				nodBST* temp = root->st;
				free(root->librarie.denumire);	//fara memory leak
				free(root);
				return temp;
			}
			else
			{
				nodBST* maxim = getMaximStanga(root);
				strcpy(root->cod, maxim->cod);
				free(root->librarie.denumire);
				root->librarie = creareLibrarie(maxim->librarie.versiune,
					maxim->librarie.tip, maxim->librarie.denumire);
				root->st = stergereNodClasic(root->st, maxim->cod);
				return root;
			}
		}
	}

	if (strcmp(root->cod, cod) < 0) {
		root->dr = stergereNodClasic(root->dr, cod);
	}
	else
		root->st = stergereNodClasic(root->st, cod);

	return root;
}

nodBST* stergereNod(nodBST* root, int versiuneDeSters) {
	if (root == NULL)
		return NULL;

	if (root->librarie.versiune == versiuneDeSters) {
		if (root->st == NULL && root->dr == NULL) {
			free(root->librarie.denumire);	//fara memory leak
			free(root);
			return NULL;
		}
		else
		{
			if (root->st == NULL) {
				//avem fiu pe dreapta
				nodBST* temp = root->dr;
				free(root->librarie.denumire);	//fara memory leak
				free(root);
				return stergereNod(temp, versiuneDeSters);
			}
			else if (root->dr == NULL) {
				//avem fiu pe stanga
				nodBST* temp = root->st;
				free(root->librarie.denumire);	//fara memory leak
				free(root);
				return stergereNod(temp, versiuneDeSters);
			}
			else
			{
				nodBST* maxim = getMaximStanga(root);
				strcpy(root->cod, maxim->cod);
				free(root->librarie.denumire);
				root->librarie = creareLibrarie(maxim->librarie.versiune,
					maxim->librarie.tip, maxim->librarie.denumire);
				root->st = stergereNodClasic(root->st, maxim->cod);

				//root->dr = stergereNod(root->dr, versiuneDeSters);
				//root->st = stergereNod(root->st, versiuneDeSters);
			}
		}
	}

	root->dr = stergereNod(root->dr, versiuneDeSters);
	root->st = stergereNod(root->st, versiuneDeSters);

	return root;
}

nodLista* copiereLibrariiInLista(nodBST* root, int filtruVersiune, nodLista* capat) {
	if (root == NULL)
		return capat;

	if (root->librarie.versiune == filtruVersiune) {
		capat = inserareSfarsit(capat, root->librarie);
	}

	capat = copiereLibrariiInLista(root->st, filtruVersiune, capat);
	capat = copiereLibrariiInLista(root->dr, filtruVersiune, capat);

	return capat;
}


nodLista* copiereLibrariiFrunzaInLista(nodBST* root, nodLista* capat, int* nrElemente) {
	if (root == NULL)
		return capat;

	if (root->st == NULL && root->dr == NULL) {
		capat = inserareSfarsit(capat, root->librarie);
		*nrElemente += 1;
	}

	capat = copiereLibrariiFrunzaInLista(root->st, capat, nrElemente);
	capat = copiereLibrariiFrunzaInLista(root->dr, capat, nrElemente);

	return capat;
}

char** inserareVectorDesc(char** vectorChei, int* n, char* cheie) {
	char** vectorNou = (char**)malloc((*n + 1) * sizeof(char*));

	if (vectorChei == NULL) {
		vectorNou[0] = cheie;
		*n = 1;
		return vectorNou;

	}

	int i = 0;
	while (strcmp(vectorChei[i], cheie) > 0) {
		vectorNou[i] = vectorChei[i];
		i += 1;
	}

	vectorNou[i] = cheie;
	i += 1;

	while (i < *n + 1) {
		vectorNou[i] = vectorChei[i - 1];
		i += 1;
	}

	*n += 1;

	return vectorNou;

}

char** copiereCheiFrunze(nodBST* root, int* pn, char** vectorChei) {
	if (root == NULL)
		return vectorChei;
	else {
		if (root->st == NULL && root->dr == NULL) {
			vectorChei = inserareVectorDesc(vectorChei, pn, root->cod);
			return vectorChei;
		}

		vectorChei = copiereCheiFrunze(root->st, pn, vectorChei);
		vectorChei = copiereCheiFrunze(root->dr, pn, vectorChei);
	}

	return vectorChei;
}

int main() {
	printf("\nCerinta1");
	nodBST* root = citireFisierInBST("Librarii.txt");
	inordine(root);

	printf("\nCerinta2");
	nodBSTArbore2* root2 = NULL;
	root2 = copiereArboreInitial(root, 3, root2);
	inordine(root2);

	printf("\nCerinta3");
	nodLista* capat = NULL;
	capat = copiereLibrariiInLista(root, 200, capat);
	afisareLista(capat);

	//root = stergereNod(root, 200);

	printf("\n ********************************");
	inordine(root);

	printf("\nCerinta4");
	char** vectorChei = NULL;
	int n = 0;
	vectorChei = copiereCheiFrunze(root, &n, vectorChei);

	for (int i = 0; i < n; i++) {
		printf("\n Cheie = %s", vectorChei[i]);
	}
}



//#define _CRT_SECURE_NO_WARNINGS
//#include<stdio.h>
//#include<stdlib.h>
//#include<string.h>
//
//struct Librarie
//{
//	int versiune;
//	int tip;
//	char* denumire;
//};
//
//
//struct BinaryTree
//{
//	char cod[50];
//	Librarie info;
//	BinaryTree* stanga;
//	BinaryTree* dreapta;
//};
//
//struct BinaryTree2
//{
//	char cod[50];
//	Librarie info2;
//	BinaryTree2* stanga;
//	BinaryTree2* dreapta;
//};
//
//struct nod
//{
//	Librarie info;
//	nod* next;
//};
//
//Librarie creareLibrarie(int v, int t, const char* d)
//{
//	Librarie librarie;
//	librarie.versiune = v;
//	librarie.tip = t;
//	librarie.denumire = (char*)malloc(sizeof(char) * (strlen(d) + 1));
//	strcpy(librarie.denumire, d);
//	return librarie;
//}
//
//BinaryTree* creareNod(const char* cod, Librarie librarie)
//{
//	BinaryTree* radacina = (BinaryTree*)malloc(sizeof(BinaryTree));
//	radacina->dreapta = NULL;
//	radacina->stanga = NULL;
//	radacina->info = creareLibrarie(librarie.versiune, librarie.tip, librarie.denumire);
//	strcpy(radacina->cod, cod);
//	return radacina;
//}
//
//BinaryTree* inserare(BinaryTree* radacina, Librarie librarie, const char* cod)
//{
//	if (radacina == NULL)
//	{
//		BinaryTree* nodNou = creareNod(cod, librarie);
//		return nodNou;
//	}
//
//	if (strcmp(radacina->cod, cod) == 0)
//	{
//		return radacina;
//	}
//	else
//		if (strcmp(radacina->cod, cod) < 0)
//			radacina->dreapta = inserare(radacina->dreapta, librarie, cod);
//		else
//			radacina->stanga = inserare(radacina->stanga, librarie, cod);
//	return radacina;
//}
//
//BinaryTree* citireFisier()
//{
//	FILE* pf;
//	fopen_s(&pf, "librarii.txt", "r");
//
//	if (pf == NULL) {
//		printf("EROARE deschidere fisier");
//		return NULL;
//	}
//
//	int nrLibrarii;
//	fscanf(pf, "%d", &nrLibrarii);
//
//	BinaryTree* root = NULL;
//
//	for (int i = 0; i < nrLibrarii; i++) {
//		char bufferCod[100];
//		int versiune;
//		int tip;
//		char bufferDenumire[100];
//
//		fscanf(pf, "%s", bufferCod);
//		fscanf(pf, "%d", &versiune);
//		fscanf(pf, "%d", &tip);
//		fgetc(pf);
//		fgets(bufferDenumire, 40, pf);
//		strtok(bufferDenumire, "\n");
//		//fscanf(pf, "%s", bufferDenumire);
//
//		Librarie librarie = creareLibrarie(versiune, tip, bufferDenumire);
//		root = inserare(root, librarie, bufferCod);
//	}
//
//	fclose(pf);
//
//	return root;
//}
//
//void afisareLibrarie(Librarie l)
//{
//	printf("\nLibraria %s cu versiunea %d si tipul %d ", l.denumire, l.versiune, l.tip);
//}
//
////void afisareNod(BinaryTree* nod) {
////	printf("\n Cod nod = %s", nod->cod);
////	printf("\n Cod nod = %d", nod->info.versiune);
////	printf("\n Cod nod = %d", nod->info.tip);
////	printf("\n Cod nod = %s", nod->info.denumire);
////	printf("\n ------------");
////}
//
//void afisareArborePreordine(BinaryTree* radacina)
//{
//	if (radacina != NULL)
//	{
//		afisareLibrarie(radacina->info);
//		afisareArborePreordine(radacina->stanga);
//		afisareArborePreordine(radacina->dreapta);
//	}
//}
//
//void afisareArborePreordine2(BinaryTree2* radacina)
//{
//	if (radacina != NULL)
//	{
//		afisareLibrarie(radacina->info2);
//		afisareArborePreordine2(radacina->stanga);
//		afisareArborePreordine2(radacina->dreapta);
//	}
//}
//BinaryTree2* inserare2(BinaryTree2* radacina, Librarie librarie2, const char* cod)
//{
//	if (radacina == NULL)
//	{
//		BinaryTree2* nodNou = (BinaryTree2*)malloc(sizeof(BinaryTree2));
//		nodNou->dreapta = NULL;
//		nodNou->stanga = NULL;
//		strcpy(nodNou->cod, cod);
//		nodNou->info2 = librarie2;
//		return nodNou;
//	}
//
//	if (strcmp(radacina->cod, cod) == 0)
//	{
//		return radacina;
//	}
//	else
//		if (strcmp(radacina->cod, cod) < 0)
//			radacina->dreapta = inserare2(radacina->dreapta, librarie2, cod);
//		else
//			radacina->stanga = inserare2(radacina->stanga, librarie2, cod);
//	return radacina;
//
//}
//
//BinaryTree2* copiereArboreInitial(BinaryTree* radacina, int filtruTip, BinaryTree2* radacina2)
//{
//	if (radacina != NULL)
//	{
//		radacina2 = copiereArboreInitial(radacina->stanga, filtruTip, radacina2);
//		if (radacina->info.tip == filtruTip) {
//			radacina2 = inserare2(radacina2, radacina->info, radacina->cod);
//		}
//		radacina2 = copiereArboreInitial(radacina->dreapta, filtruTip, radacina2);
//	}
//	return radacina2;
//}
//
//void dezalocare(BinaryTree*& radacina)
//{
//	if (radacina != NULL)
//	{
//		dezalocare(radacina->dreapta);
//		dezalocare(radacina->stanga);
//		/*free(radacina->cod);
//		free(radacina->info.denumire);*/
//		free(radacina);
//		radacina = NULL;
//	}
//}
//
//void dezalocare2(BinaryTree2*& radacina)
//{
//	if (radacina != NULL)
//	{
//		dezalocare2(radacina->dreapta);
//		dezalocare2(radacina->stanga);
//		/*free(radacina->cod);
//		free(radacina->info.denumire);*/
//		free(radacina);
//		radacina = NULL;
//	}
//}
//
//BinaryTree* stergere_inserare_lista(BinaryTree* radacina, int versiune, nod*& cap)
//{
//	if (radacina != NULL)
//	{
//		if (radacina->info.versiune == versiune)
//		{
//			if (cap == NULL)
//			{
//				cap = (nod*)malloc(sizeof(nod));
//				cap->info = radacina->info;
//				cap->next = NULL;
//				free(radacina);
//			}
//			else
//			{
//				nod* cursor = cap;
//				while (cursor->next != NULL)
//				{
//					cursor = cursor->next;
//				}
//				nod* nou = (nod*)malloc(sizeof(nod));
//				nou->next = NULL;
//				nou->info = radacina->info;
//				cursor->next = nou;
//				free(radacina);
//			}
//		}
//		radacina = stergere_inserare_lista(radacina->stanga, versiune, cap);
//		radacina = stergere_inserare_lista(radacina->dreapta, versiune, cap);
//	}
//	if (radacina != NULL)
//		return radacina;
//}
//
//void afisare_lista(nod* cap)
//{
//	while (cap != NULL)
//	{
//		afisareLibrarie(cap->info);
//		cap = cap->next;
//	}
//}
//
//
//
//int main()
//{
//	BinaryTree* radacina = citireFisier();
//	afisareArborePreordine(radacina);
//
//	printf("\n-----Afisare arbore 2----");
//	BinaryTree2* radacina2 = NULL;
//	radacina2 = copiereArboreInitial(radacina, 2, radacina2);
//	afisareArborePreordine2(radacina2);
//	//nod* cap = NULL;
//	//radacina = stergere_inserare_lista(radacina, 4, cap);
//	//printf("\nLista:");
//	//afisare_lista(cap);
//	//printf("\nArbore dupa stergere:");
//	//afisareArborePreordine(radacina);
//
//
//	//dezalocare(radacina);
//	//dezalocare2(radacina2);
//	return 0;
//}


