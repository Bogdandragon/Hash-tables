/* ANGHELOIU George-Bogdan - 314CB*/
#include <stdio.h>
#include "tlg.h"
#include "thash.h"
#include <string.h>
#include <ctype.h>
#include <limits.h>

typedef struct {
	int nrLit;
	TLG *cuvinte;
} TLit;

typedef struct {
	char *cuv;
	int aparitii;
} TCuv;

int codHash(void *element)
{
	char *cuv = (char *)element;
	return *cuv > 'Z' ? *cuv - 'a' : *cuv - 'A';
}

//genereaza tabela hash goala
TH *GenereazaTabelaHash()
{
	TH *hash = NULL;
	size_t M = ('Z' - 'A' + 1);
	hash = (TH *)InitTH(M, codHash);
	if (hash)
		return hash;
	return NULL;
}

//verifica daca lungimea sirului din el2 se afla in el1
int LitCmp(void *el1, void *el2)
{
	TLit *lit = (TLit *)el1;
	char *cuv = (char *)el2;
	return lit->nrLit == strlen(cuv);
}

//compara 2 cuvinte in functie de aparitii, apoi lexicografic
int CuvCmp(void *el1, void *el2)
{
	TCuv *cuv1 = (TCuv *) el1;
	TCuv *cuv2 = (TCuv *) el2;
	if (cuv1->aparitii == cuv2->aparitii)
		return strcmp(cuv1->cuv, cuv2->cuv);
	else if (cuv1->aparitii > cuv2->aparitii)
		return -1;
	else
		return 1;
}


int insertCuv(TLG *lista, void *el)
{
	TLG i, j;
	char *cuv = (char *) el;
	//verifica daca se afla in lista cuvantul
	TLG prec = NULL;
	for (i = *lista; i != NULL; i = i->urm) {
		if (strcmp(((TCuv *)i->info)->cuv, cuv) == 0) {
			((TCuv *)i->info)->aparitii++;
			//actualizeaza locatia in lista in functie de aparitii
			if (CuvCmp((*lista)->info, i->info) != 0) {
				if (CuvCmp((*lista)->info, i->info) > 0) {
					if (prec)
						prec->urm = i->urm;
					i->urm = *lista;
					*lista = i;
					return 1;
				}
				for (j = *lista; j->urm != i && j->urm != NULL; j = j->urm) {
					if (CuvCmp(j->urm->info, i->info) > 0) {
						if (prec != NULL)
							prec->urm = i->urm;
						i->urm = j->urm;
						j->urm = i;
						return 1;
					}
				}
			}
			return 1;
		}
		prec = i;
	}

	TLG aux = (TLG)malloc(sizeof(TCelulaG));
	//aux nealocat
	if (aux == NULL)
		return 0;

	aux->info = (TCuv *)malloc(sizeof(TCuv));
	//aux->info nealocat
	if (aux->info == NULL) {
		free(aux);
		return 0;
	}
	((TCuv *)aux->info)->cuv = (char *)malloc(sizeof(char) * (strlen(cuv) + 1));
	//cuvant nealocat
	if (((TCuv *)aux->info)->cuv == NULL) {
		free(aux->info);
		free(aux);
		return 0;
	}
	strncpy(((TCuv *)aux->info)->cuv, cuv, strlen(cuv) + 1);
	((TCuv *)aux->info)->aparitii = 1;
	//lista vida
	if (*lista == NULL) {
		aux->urm = NULL;
		*lista = aux;
		return 1;
	} else if (CuvCmp((*lista)->info, aux->info) >= 0) {//devine primul element
		aux->urm = *lista;
		*lista = aux;
		return 1;
	} else {
		//se introduce in restul listei
		for (i = *lista; i->urm != NULL && i->urm->info != NULL; i = i->urm) {
			if (CuvCmp(i->urm->info, aux->info) >= 0) {
				aux->urm = i->urm;
				i->urm = aux;
				return 1;
			}
		}
		//introduce la sfarsit
		aux->urm = NULL;
		i->urm = aux;
		return 1;
	}
}

int insertLit(TLG *lista, void *el)
{
	TLG aux = malloc(sizeof(TCelulaG));
	if (aux == NULL) {
		return 0;
	}
	char *cuv = (char *) el;
	aux->info = (TLit *)malloc(sizeof(TLit));
	if (aux->info == NULL) {
		free(aux);
		return 0;
	}
	((TLit *)aux->info)->nrLit = (int)strlen(cuv);
	((TLit *)aux->info)->cuvinte = (TLG *)malloc(sizeof(TLG));
	if (((TLit *)aux->info)->cuvinte == NULL) {
		free(aux->info);
		free(aux);
		return 0;
	}
	*(((TLit *)aux->info)->cuvinte) = NULL;
	aux->urm = NULL;
	if (*lista == NULL) {
		//nimic in lista
		*lista = aux;
		(*lista)->urm = NULL;
		return insertCuv(((TLit *)(*lista)->info)->cuvinte, el);
	} else if (((TLit *)(*lista)->info)->nrLit > strlen(cuv)) {
		//elementul adaugat trebuie sa fie primul
		aux->urm = *lista;
		*lista = aux;
		return insertCuv(((TLit *)(*lista)->info)->cuvinte, el);
	} else {
		//cautam sa inseram elementul in lista
		TLG i;
		for (i = *lista; i->urm != NULL; i = i->urm) {
			if (((TLit *)i->urm->info)->nrLit > ((TLit *)aux->info)->nrLit) {
				aux->urm = i->urm;
				i->urm = aux;
				return insertCuv(((TLit *)aux->info)->cuvinte, el);
			}
		}
		aux->urm = NULL;
		i->urm = aux;
		return insertCuv(((TLit *)aux->info)->cuvinte, el);
	}
}

int insert(TH *hash, char *p)
{
	char *cuv = malloc(sizeof(char) * (strlen(p) + 1));
	if (cuv == NULL)
		return 0;
	strncpy(cuv, p, strlen(p) + 1);
	int cod = hash->fh(cuv);
	int rez;
	TLG parc = NULL;
	for (parc = hash->v[cod]; parc != NULL; parc = parc->urm) {
		//daca exista cuvinte cu litera data de cod si de lungimea lui cuv
		if (LitCmp(parc->info, cuv)) {
			rez = insertCuv(((TLit *)parc->info)->cuvinte, cuv);
			free(cuv);
			return rez;
		}
	}
	//nu exista cuvinte cu litera data de cod si de lungimea lui cuv
	if (strlen(cuv) <= 2) {
		free(cuv);
		return 1;
	}
	rez = insertLit(hash->v+cod, cuv);
	free(cuv);
	return rez;
}

void print(TH *hash)
{
	size_t i = 0;
	TLG j, k;
	for (; i < hash->M; i++) {
		if (hash->v[i]) {
			printf("pos %zu: ", i);
			for (j = hash->v[i]; j != NULL; j = j->urm) {
				printf("(%d:", ((TLit *)j->info)->nrLit);
				for (k = *(((TLit *)j->info)->cuvinte); k != NULL; k = k->urm) {
					printf("%s/%d", ((TCuv *)k->info)->cuv, ((TCuv *)k->info)->aparitii);
					if (k->urm != NULL)
						printf(", ");
				}
				printf(")");
			}
			printf("\n");
		}
	}
}

void printLetLen(TH *hash, char c, int n)
{
	TLG i, j;
	int cod = hash->fh(&c);
	if (hash->v[cod]) {
		for (i = hash->v[cod]; i != NULL; i = i->urm) {
			if (((TLit *)i->info)->nrLit == n)
				break;
		}
		if (i) {
			printf("(%d:", n);
			for (j = *(((TLit *)i->info)->cuvinte); j != NULL; j = j->urm) {
				printf("%s/%d", ((TCuv *)j->info)->cuv, ((TCuv *)j->info)->aparitii);
				if (j->urm != NULL)
					printf(", ");
			}
			printf(")\n");
		}
	}
}

int maxCuv(TLG list, int n)
{
	TLG i;
	for (i = list; i != NULL; i = i->urm) {
		if (((TCuv *)i->info)->aparitii <= n)
			return 1;
	}
	return 0;
}

int maxLit(TLG list, int n)
{
	TLG i;
	for (i = list; i != NULL; i = i->urm) {
		if (maxCuv(*(((TLit *)i->info)->cuvinte), n))
			return 1;
	}
	return 0;
}

void printMax(TH *hash, int n)
{
	size_t i = 0;
	TLG j, k;
	for (; i < hash->M; i++) {
		if (hash->v[i] && maxLit(hash->v[i], n)) {
			printf("pos%zu: ", i);
			for (j = hash->v[i]; j != NULL; j = j->urm) {
				if (maxCuv(*(((TLit *)j->info)->cuvinte), n)) {
					printf("(%d: ", ((TLit *)j->info)->nrLit);
					for (k = *(((TLit *)j->info)->cuvinte); k != NULL; k = k->urm) {
						if (((TCuv *)k->info)->aparitii <= n) {
							printf("%s/%d", ((TCuv *)k->info)->cuv, ((TCuv *)k->info)->aparitii);
							if (maxCuv(k->urm, n))
								printf(", ");
						}
					}
					printf(")");
				}
			}
			printf("\n");
		}
	}
}

void elibTCuv(void *info)
{
	TCuv *cuv = (TCuv *)info;
	free(cuv->cuv);
	free(cuv);
	info = NULL;
}

void elibTLit(void *info)
{
	TLit *lit = (TLit *)info;
	DistrugeLG(lit->cuvinte, elibTCuv);
	free(lit->cuvinte);
	free(lit);
	info = NULL;
}

int main(int argc, char *argv[])
{
	FILE *f = fopen(argv[1], "r");
	TH *hash = GenereazaTabelaHash();
	if (!hash)
		printf("Tabela hash nu a fost generata cu succes!");
	size_t lineSize = 1000;
	char *line = malloc(sizeof(char) * (lineSize + 1));
	if (line == NULL) {
		printf("Eroare de alocare!");
		return 0;
	}
	char *p;
	int n, rez;
	char c;
	while (getline(&line, &lineSize, f) > 0) {
		p = strtok(line, " ,.;:/\n");
		if (p) {
			//inserare
			if (strcmp(p, "insert") == 0) {
				while (p) {
					p = strtok(NULL, " ,.;:/\n");
					if (p && isalpha(*p)) {
						rez = insert(hash, p);
						if (rez == 0) {
							printf("Inserarea cuvantului %s a esuat!", p);
							return 0;
						}
					}
				}
			} else if (strcmp(p, "print") == 0) { //printare
				p = strtok(NULL, " ,.;:/\n");
				if (p) {
					if (isdigit(*p)) {
						n = atoi(p);
						printMax(hash, n);
					} else if (isalpha(*p)) {
						c = *p;
						p = strtok(NULL, " ,.;:/\n");
						if (p) {
							if (isdigit(*p)) {
								n = atoi(p);
								printLetLen(hash, c, n);
							}
						}
					}
				} else {
					print(hash);
				}
			}
		} else
			break;
	}
	free(line);
	DistrTH(&hash, elibTLit);
	fclose(f);
}
