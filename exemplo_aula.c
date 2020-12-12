#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>

#define TAM 1000 /* Tamanho do Buffer */
#define N_PROD 1 /* Quantidade de produtores */
#define N_CONS 1 /* Quantidade de consumidores */

/* Protótipos */
void *producer(void *arg);
void *consumer(void *arg);
void insert_item(char item);
char remove_item();

/* Semáforos */
sem_t mutex, empty, full;

/* Buffer */
char buffer[TAM];
int buffer_pos = 0;

int main(int argc, const char *argv[])
{
	pthread_t prods[N_PROD], cons[N_CONS];
	int i;
	sem_init(&mutex, 0, 1);
	sem_init(&empty, 0, TAM);
	sem_init(&full, 0, 0);

	/* Cria as Threads de produtores */
	for (i = 0; i < N_PROD; i++) {
		pthread_create(&prods[i], NULL, producer, NULL);
	}

	/* Cria as Threads de consumidores */
	for (i = 0; i < N_CONS; i++) {
		pthread_create(&cons[i], NULL, consumer, NULL);
	}

	/* Faz o join em todas as threads */
	for (i = 0; i < N_PROD; i++) {
		pthread_join(prods[i], NULL);
	}

	for (i = 0; i < N_CONS; i++) {
		pthread_join(cons[i], NULL);
	}
	
	return 0;
}

/** Produtor */
void *producer(void *arg)
{
	char item;
	while(1) {
		item = 'A';
		sem_wait(&empty);
		sem_wait(&mutex);
		insert_item(item);
		sem_post(&mutex);
		sem_post(&full);
	}
}
/** Consumidor */
void *consumer(void *arg)
{
	char item;
	while(1) {
		sem_wait(&full);
		sem_wait(&mutex);
		item = remove_item();
		sem_post(&mutex);
		sem_post(&empty);
		printf("Consumi: %c\n", item); /* consome item */
	}
}

/* Insere no buffer */
void insert_item(char item)
{
	printf("Produzi: %c\n", item);
	buffer[buffer_pos++] = item;
}

/* Remove do buffer */
char remove_item()
{
	return buffer[buffer_pos--];
}