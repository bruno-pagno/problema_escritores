#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <semaphore.h>
#include <pthread.h>

#define STRING_MAX 100

#define MAX_READERS 10
#define MAX_WRITERS 1
#define FILE_NAME "dataRepository.txt"

int line_count = 0; // contador de linhas do arquivo

sem_t mutex, empty;

typedef char write_t;
typedef char read_t;


void write_data(FILE *arq , write_t *write){
  fseek(arq, 0, SEEK_END);
  fwrite(write, sizeof(write), 1, arq);
}

void *writer() {
	char item;
	while(1) {
    write_t to_write[STRING_MAX];
    sprintf(to_write, "%s %d\n", "linha ", line_count++);
    
		sem_wait(&mutex);
    
    FILE *arq = (FILE *) fopen(FILE_NAME, "a");
		write_data(arq, to_write);
    fclose(arq);
    
		sem_post(&mutex);
	}
}

read_t* read_data(FILE *arq, long start) {
  //char * , tamanho >= tamanho da string , aloca isso antes
  read_t *string_return;

  string_return = (read_t*) malloc(STRING_MAX*sizeof(read_t)); 
  
  fseek(arq ,start , SEEK_SET);
  fscanf(arq,"%[^\n]s", string_return);
  
  return string_return;
}

void process_data(read_t* data , int start) {
  printf("LEITURA %d: %s\n", start, data);
}

void *reader(FILE *arq) {
  int start;
  while(1) {
    
    sem_wait(&empty);
    
    srand(time(NULL));
    start = rand() % line_count;
    read_t *string_return = read_data(arq , start);

    sem_post(&empty);
  }
}

int main(void) {
  pthread_t writers[MAX_WRITERS], readers[MAX_READERS];
	sem_init(&empty, 0, MAX_READERS);
  int i;
  FILE *arq = fopen(FILE_NAME, "r");

  for (i = 0; i < MAX_WRITERS; i++) {
		pthread_create(&writers[i], NULL, writer, NULL);
	}

	/* Cria as Threads de consumidores */
	for (i = 0; i < MAX_READERS; i++) {
		pthread_create(&readers[i], NULL, reader(arq), NULL);
	}

	/* Faz o join em todas as threads */
	for (i = 0; i < MAX_WRITERS; i++) {
		pthread_join(writers[i], NULL);
	}

	for (i = 0; i < MAX_READERS; i++) {
		pthread_join(readers[i], NULL);
	}

  return 0;
}

//Cai de novo
//Estou só alterando o necessario por enquanto [blz]
// ah, tava falando sozinho kkjk
