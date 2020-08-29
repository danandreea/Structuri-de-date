#include <stdio.h>
#include <iostream>
#pragma warning(disable:4996)

using namespace std;

struct student {
	int cod;
	char *nume;
	float medie;
};

struct nodarb {
	student inf;
	nodarb *left, *right;
};

struct nodls {
	student inf;
	nodls *next;
};

nodarb *creare(student s, nodarb *st, nodarb *dr) {

	nodarb *nou = (nodarb*)malloc(sizeof(nodarb));
	nou->inf.cod = s.cod;
	nou->inf.nume = (char*)malloc((strlen(s.nume) + 1) * sizeof(char));
	strcpy(nou->inf.nume, s.nume);
	nou->inf.medie = s.medie;
	nou->left = st;
	nou->right = dr;
	return nou;
}

nodarb *inserare(student s, nodarb *rad) {
	nodarb *aux = rad;
	if (rad == NULL)
	{
		aux = creare(s, NULL, NULL);
		return aux;
	}
	else
		while (true) {
			if (s.cod < aux->inf.cod)
				if (aux->left != NULL)
					aux = aux->left;
				else {
					aux->left = creare(s, NULL, NULL);
					return rad;
				}
			else
				if (s.cod > aux->inf.cod)
					if (aux->right != NULL)
						aux = aux->right;
					else {
						aux->right = creare(s, NULL, NULL);
						return rad;
					}
				else
					return rad;
		}
}

void preordine(nodarb *rad) {
	if (rad != NULL) {
		printf("\nCod = %d, Nume = %s, Medie = %5.2f", rad->inf.cod, rad->inf.nume, rad->inf.medie);
		preordine(rad->left);
		preordine(rad->right);
	}
}

void inordine(nodarb *rad) {
	if (rad != NULL) {
		preordine(rad->left);
		printf("\nCod = %d, Nume = %s, Medie = %5.2f", rad->inf.cod, rad->inf.nume, rad->inf.medie);
		preordine(rad->right);
	}
}

void postordine(nodarb *rad) {
	if (rad != NULL) {
		preordine(rad->left);
		preordine(rad->right);
		printf("\nCod = %d, Nume = %s, Medie = %5.2f", rad->inf.cod, rad->inf.nume, rad->inf.medie);
	}
}

void dezalocare(nodarb *rad) {
	if (rad != NULL) {
		nodarb *st = rad->left;
		nodarb *dr = rad->right;
		free(rad->inf.nume);
		free(rad);
		dezalocare(rad->left);
		dezalocare(rad->right);
	}


}

nodarb *cautare(nodarb *rad, int cheie) {
	if (rad != NULL) {
		if (cheie == rad->inf.cod)
			return rad;
		else
			if (cheie < rad->inf.cod)
				return cautare(rad->left, cheie);
			else
				return cautare(rad->right, cheie);
	}
	else
		return NULL;
}

int maxim(int a, int b) {
	int max = a;
	if (max < b)
		max = b;
	return max;
}

int nrNiveluri(nodarb *rad) {
	if (rad != NULL)
		return 1 + maxim(nrNiveluri(rad->left), nrNiveluri(rad->right));
	else
		return 0;
}

void conversieArboreVector(nodarb *rad, student *vect, int *nr) {

	if (rad != NULL) {
		//vect[*nr] = rad->inf;
		//(*nr)++;
		//conversieArboreVector(rad->left, vect, nr);
		//conversieArboreVector(rad->right, vect, nr);

		vect[*nr].cod = rad->inf.cod;
		vect[*nr].nume = (char*)malloc((strlen(rad->inf.nume) + 1) * sizeof(char));
		strcpy(vect[*nr].nume, rad->inf.nume);
		vect[*nr].medie = rad->inf.medie;
		(*nr)++;

		nodarb *st = rad->left;
		nodarb *dr = rad->right;
		free(rad->inf.nume);
		free(rad);
		conversieArboreVector(st, vect, nr);
		conversieArboreVector(dr, vect, nr);
	}
}

nodls* inserareLista(nodls* cap, student s) {
	nodls* nou = (nodls*)malloc(sizeof(nodls));
	nou->inf.cod = s.cod;
	nou->inf.nume = (char*)malloc((strlen(s.nume) + 1) * sizeof(char));
	strcpy(nou->inf.nume, s.nume);
	nou->inf.medie = s.medie;

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
//sa se numere toti studentii care sunt restanti din arbore

/*int restantieri(nodarb* rad, int* nr) {
	if (rad != NULL) {
		if (rad->inf.medie < 5)
			(*nr)++;
		restantieri(rad->left, nr);
		restantieri(rad->right, nr);
		return (*nr);
	}
}*/


//suma notelor studentilor restantieri
/*void sumaMedii(nodarb* rad, int* suma) {
	8if (rad != NULL) {
		if (rad->inf.medie < 5)
			(*suma) += rad->inf.medie;
		sumaMedii(rad->left, suma);
		sumaMedii(rad->right, suma);
	}
}

//sa se copieze intr-un vector studentii restantieri
void conversieRestantieri(nodarb *rad, student *vect, int *nr) {

	if (rad != NULL) {
		if (rad->inf.medie < 5) {
			vect[*nr].cod = rad->inf.cod;
			vect[*nr].nume = (char*)malloc((strlen(rad->inf.nume) + 1) * sizeof(char));
			strcpy(vect[*nr].nume, rad->inf.nume);
			vect[*nr].medie = rad->inf.medie;
			(*nr)++;
		}
		nodarb *st = rad->left;
		nodarb *dr = rad->right;
		//free(rad->inf.nume);
		//free(rad);
		
		conversieRestantieri(st, vect, nr);
		conversieRestantieri(dr, vect, nr);
	}
}

nodarb *stergeRad(nodarb *rad) {
	nodarb *aux = rad;
	if (aux->left != NULL) {
		rad = aux->left;
		if (aux->right != NULL) {
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
	free(aux->inf.nume);
	free(aux);
	return rad;
}

nodarb *stergeRad2(nodarb *rad) {
	nodarb *aux = rad;
	if (aux->right != NULL) {
		rad = aux->right;
		if (aux->left != NULL) {
			nodarb* temp = aux->right;
			while (temp->left)
				temp = temp->left;
			temp->left = aux->left;
		}
	}
	else
		if (aux->left != NULL)
			rad = aux->left;
		else
			rad = NULL;
	free(aux->inf.nume);
	free(aux);
	return rad;
}

nodarb* stergeNod(nodarb *rad, int cheie) {
	if (rad == NULL)
		return NULL;
	else
		if (rad->inf.cod == cheie) {
			rad = stergeRad(rad);
			return rad;
		}
		else {
			nodarb *aux = rad;
			while (true) {
				if (cheie < aux->inf.cod)
					if (aux->left == NULL)
						break;
					else
						if (aux->left->inf.cod == cheie)
							aux->left = stergeRad(aux->left);
						else
							aux = aux->left;
				else
					if (cheie > aux->inf.cod)
						if (aux->right == NULL)
							break;
						else
							if (aux->right->inf.cod == cheie)
								aux->right = stergeRad(aux->right);
							else
								aux = aux->right;
			}
			return rad;
		}
}*/



void main() {
	int n;

	FILE *f = fopen("fisier.txt", "r");

	//printf("Nr. studenti = ");
	fscanf(f, "%d", &n);

	nodarb *rad = NULL;
	student s;
	char buffer[20];
	for (int i = 0; i < n; i++) {
		//printf("\nCod = ");
		fscanf(f, "%d", &s.cod);
		//printf("\nNume = ");
		fscanf(f, "%s", buffer);
		s.nume = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
		strcpy(s.nume, buffer);
		//printf("\nMedie = ");
		fscanf(f, "%f", &s.medie);

		rad = inserare(s, rad);
	}
	fclose(f);

	preordine(rad);
	printf("\n------------------\n");
	inordine(rad);
	printf("\n------------------\n");
	postordine(rad);

	nodarb *nodCautat = cautare(rad, 6);
	if (nodCautat != NULL)
		printf("\nStudentul cautat se numeste %s", nodCautat->inf.nume);
	else printf("\nStudentul nu exista.");


	printf("\nInaltime arbore = %d", nrNiveluri(rad));
	printf("\nInatime subarbore stang = %d", nrNiveluri(rad->left));
	printf("\nInatime subarbore drept = %d", nrNiveluri(rad->right));
	/*printf("\n--------VECTOR----------\n");
	student *vect = (student*)malloc(n * sizeof(student));
	int nr = 0;
	conversieArboreVector(rad, vect, &nr);
	for (int i = 0; i < n; i++)
		printf("\nCod = %d, Nume = %s, Medie = %5.2f", vect[i].cod, vect[i].nume, vect[i].medie);
	for (int i = 0; i < n; i++)
		free(vect[i].nume);
	free(vect);

	printf("\n--------------------------------------------\n");
	printf("\n--------------------------------------------\n");

	int sm = 0;
	sumaMedii(rad, &sm);

	printf("Suma medii: %d", sm);

	printf("\n--------VECTOR----------\n");
	student *vectRestantieri = (student*)malloc(n * sizeof(student));
	int nr = 0;
	conversieRestantieri(rad, vectRestantieri, &nr);
	for (int i = 0; i < n; i++)
		printf("\nCod = %d, Nume = %s, Medie = %5.2f", vectRestantieri[i].cod, vectRestantieri[i].nume, vectRestantieri[i].medie);
	for (int i = 0; i < n; i++)
		free(vectRestantieri[i].nume);
	
	printf("\n--------------------------------------------\n");
	printf("\n--------------------------------------------\n");

	//rad = stergeRad(rad);
	rad = stergeNod(rad, 7);
	inordine(rad);
	printf("\n--------------------------------------------\n");
	inordine(rad->left);
	printf("\n--------------------------------------------\n");
	inordine(rad->right);*/


	
	printf("\n --------------------------------------------------------\n");
	printf("CONVERSIE ARBORE->LISTA:");
	nodls *cap = NULL;
	conversieArboreLista(rad, &cap);
	nodls* temp = cap;
	while (temp) {
		printf("\nCod = %d, Nume = %s, Medie = %5.2f", temp->inf.cod, temp->inf.nume, temp->inf.medie);
		temp = temp->next;
	}


	dezalocare(rad);
}
