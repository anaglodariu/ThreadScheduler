#include <stdio.h>
#include <stdlib.h>
#include "so_scheduler.h"
#include "semaphore.h"
#include "struct.h"

static scheduler_t *scheduler;

int so_init(unsigned int time_quantum, unsigned int io)
{
	/*verificari argumente valide*/
	if (time_quantum == 0) {
		return -1;
	} else if (io > SO_MAX_NUM_EVENTS || time_quantum > 32) {
		return -1;
	} else if (scheduler != NULL) {
		/*planificatorul a fost initializat deja, iar so_end nu a fost apelat*/
		return -1;
	} else {
		/*aloc planificatorul de threaduri*/
		scheduler = (scheduler_t *)malloc(sizeof(scheduler_t));
		scheduler->time_quantum = time_quantum;
		scheduler->max_io = io;
		scheduler->running = NULL;
		scheduler->ready = NULL;
		return 0;
	}
}

/*simuleaza planificarea*/
void *start_thread(void *arg)
{
	so_task_t *task = (so_task_t *)arg;
	/*threadul asteapta sa fie planificat*/
	/*threadul se opreste pe semafor*/
	sem_wait(task->sem);
	/*se apeleaza handler-ul*/
	task->handler(task->priority);
	/*thread-ul se termina si trebuie lasat altcineva pe procesor*/
	/*luam un alt thread din ready si sa-l planificam pe ala*/
	return NULL;
}

void context_switch(so_task_t *old, so_task_t *new)
{
	scheduler->running = new;
	sem_post(new->sem);
	sem_wait(old->sem);
}

void schedule_thread(so_task_t *running, so_task_t *new)
{
	if (running->priority < new->priority)
		context_switch(running, new);
	else if (running->quantum == 0)
		context_switch(running, new);
}

/*so_fork apelat din contextul unui thread*/
static int flag;

tid_t so_fork(so_handler *func, unsigned int priority)
{
	/*verificari argumente valide*/
	if (func == NULL || priority > SO_MAX_PRIO)
		return INVALID_TID;
	/*se scade cuanta threadului curent*/
	if (scheduler->running != NULL)
		scheduler->running->quantum--;
	/*se aloca un nou thread so_task_t*/
	so_task_t *new_thread = (so_task_t *)malloc(sizeof(so_task_t));

	if (new_thread == NULL)
		return INVALID_TID;
	new_thread->priority = priority;
	new_thread->quantum = scheduler->time_quantum;
	new_thread->handler = func;
	new_thread->sem = (sem_t *)malloc(sizeof(sem_t));

	if (new_thread->sem == NULL)
		return INVALID_TID;
	/*initializam semaforul*/
	sem_init(new_thread->sem, 0, 1);
	/*se creeaza un thread nou*/
	/*dam ca parametru functiei start_thread structura*/
	/*in care imi salvez threadul nou creat deoarece*/
	/*am nevoie de prioritatea lui si de handler*/
	pthread_create(&new_thread->thread_id, NULL, start_thread, new_thread);
	/*daca e prima data cand se apeleaza functia so_fork*/
	if (flag == 0) {
		flag = 1;
		/*il punem in running si crestem semaforul*/
		/*adica il notific sa se trezeasca si sa iasa din wait*/
		scheduler->running = new_thread;
		sem_post(new_thread->sem);
	}
	return new_thread->thread_id;
}

int so_wait(unsigned int io)
{
	if (io > scheduler->max_io)
		return -1;
	return 0;
}

void so_exec(void)
{
	/*scade cuanta*/
}

void so_end(void)
{
	if (scheduler != NULL) {
		if (scheduler->running != NULL)
			pthread_join(scheduler->running->thread_id, NULL);
		free(scheduler);
		scheduler = NULL;
	}
}

int so_signal(unsigned int io)
{
	if (io > scheduler->max_io)
		return -1;
	return 0;
}
