#include "so_scheduler.h"
#include "semaphore.h"
#include "list.h"
/*structura pentru un task/thread gestionat de planificator*/
typedef struct so_task_t {
	tid_t thread_id;
	int priority;
	int quantum;
	so_handler *handler;
	sem_t *sem; /*primitiva de sincronizare = semafor*/
} so_task_t;
/*structura pentru planificatorul de threaduri*/
typedef struct scheduler_t {
	int time_quantum;
	int max_io;
	so_task_t *running; /*ce thread ruleaza pe procesor*/
	Lista ready; /*coada de prioritati*/
} scheduler_t;
