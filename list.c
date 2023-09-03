#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"

Lista aloc_cell(void *info)
{
	Lista aux = NULL;

	aux = (Lista)malloc(sizeof(Celula));
	if (aux == NULL)
		return NULL;
	aux->info = malloc(sizeof(so_task_t));
	if (aux->info == NULL) {
		free(aux);
		return NULL;
	}
	memcpy(aux->info, info, sizeof(so_task_t));
	aux->urm = NULL;
	return aux;
}

int insert_after_priority(Lista *L, void *info)
{
	Lista aux = NULL;

	aux = aloc_cell(info);
	if (aux == NULL)
		return 0;
	if (*L == NULL) {
		*L = aux;
		return 1;
	}
	so_task_t *task = (so_task_t *)info;
	Lista p = *L, ant = NULL;
	int ok = 0;
	/*punem threadurile in ordine descrescatoare dupa prioritate*/
	for (; p != NULL; ant = p, p = p->urm) {
		so_task_t *task1 = (so_task_t *)p->info;
		if (task->priority > task1->priority) {
			/*threadul task trebuie introdus inaintea threadului task1*/
			ok = 1;
			break;
		} else if (task->priority == task1->priority) {
			/*threadul task trebuie introdus dupa threadul task1*/
			ok = 2;
			break;
		}
	}
	if (ok == 1) {
		if (ant == NULL) {
			aux->urm = *L;
			*L = aux;
		} else {
			aux->urm = p;
			ant->urm = aux;
		}
	} else if (ok == 2) {
		p->urm = aux;
	}
	return 1;
}

void destroy(Lista *L)
{
	Lista aux = NULL;

	while (*L != NULL) {
		aux = *L;
		if (aux == NULL)
			return;
		*L = (*L)->urm;
		so_task_t *task = (so_task_t *)aux->info;

		free(task->sem);
		free(task);
		free(aux);
	}
}
