#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <stdio.h>

#define NUMCONS 1
#define NUMPROD 1
#define BUFFERSIZE 1000

pthread_t cons[NUMCONS]; 
pthread_t prod[NUMPROD];
pthread_mutex_t buffer_mutex;

int buffer[BUFFERSIZE]; 
int currentidx;
sem_t buffer_full, buffer_empty;

void *consumidor(void *arg);
void *produtor(void *arg);

int main(int argc, char **argv) 
{
 int i;
 
 currentidx = 0;
 pthread_mutex_init(&buffer_mutex, NULL);
 sem_init(&buffer_full, 0, 0);
 sem_init(&buffer_empty, 0, BUFFERSIZE);

 for(i=0; i<NUMCONS; i++) 
 {
          pthread_create(&(cons[i]), NULL, consumidor, NULL);
 }

 for(i=0; i<NUMPROD; i++) 
 {
          pthread_create(&(prod[i]), NULL, produtor, NULL);
 }

 for(i=0; i<NUMCONS; i++)
          pthread_join(cons[i], NULL);

 for(i=0; i<NUMPROD; i++)
          pthread_join(prod[i], NULL);
}

void *produtor(void *arg) 
{
 int data_write = 0;
 while(1) 
 {
    sem_wait(&buffer_empty);
    pthread_mutex_lock(&buffer_mutex);
	printf("Produzindo item %d com numero %d\n", currentidx+1, data_write);
    buffer[currentidx++] = data_write++;
    pthread_mutex_unlock(&buffer_mutex);
    sem_post(&buffer_full);
	sleep(1);
 }
}

void *consumidor(void *arg) 
{
 int data_read;
 while(1) 
 {
    sem_wait(&buffer_full);
    pthread_mutex_lock(&buffer_mutex);
    data_read = buffer[--currentidx];
    printf("Consumindo item %d com numero %d\n", currentidx+1, data_read);
    pthread_mutex_unlock(&buffer_mutex);
    sem_post(&buffer_empty);          
	sleep(5);
 }
}
