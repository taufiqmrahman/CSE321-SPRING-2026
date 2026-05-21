#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

int n;
sem_t supp;
sem_t makerA;
sem_t makerB;
sem_t makerC;
pthread_mutex_t table;

void *supplier(void *arg);
void *maker_A(void *arg);
void *maker_B(void *arg);
void *maker_C(void *arg);

int main(){
	printf("Enter the number of times supplier places ingredients: ");
	scanf("%d",&n);
	printf("\n");
	sem_init(&supp, 0,1);
	sem_init(&makerA, 0,0);
	sem_init(&makerB, 0,0);
	sem_init(&makerC, 0,0);
	pthread_mutex_init(&table, NULL);
	pthread_t s;
	pthread_t a;
	pthread_t b;
	pthread_t c;
	pthread_create(&s, NULL, supplier, NULL);
	pthread_create(&a, NULL, maker_A, NULL);
	pthread_create(&b, NULL, maker_B, NULL);
	pthread_create(&c, NULL, maker_C, NULL);
	pthread_join(s, NULL);
	printf("Simulation finished after %d rounds.\n", n);
	return 0;
	
	
}

void *supplier(void *arg){
	for(int i=0; i<n;i++){
		sem_wait(&supp);
		pthread_mutex_lock(&table);
		int select = rand() % 3;
		if(select ==0){
			printf("Supplier places: Cheese and Lettuce\n");
            		sem_post(&makerA);
		}
		else if(select ==1){
			printf("Supplier places: Bread and Lettuce\n");
            		sem_post(&makerB);
		}
		else{
			printf("Supplier places: Bread and Cheese\n");
            		sem_post(&makerC);
		}
		pthread_mutex_unlock(&table);
	}
	return NULL;
}

void *maker_A(void *arg){
	while(1){
		sem_wait(&makerA);
		pthread_mutex_lock(&table);
		printf("Maker A picks up Cheese and Lettuce\n");
		printf("Maker A is making sandwich...\n");
		printf("Maker A finished making the sandwich and eats it\n");
		printf("Maker A signals Supplier\n");
		printf("\n");
		pthread_mutex_unlock(&table);
		sem_post(&supp);
	}
	return NULL;
	
}

void *maker_B(void *arg){
	while(1){
		sem_wait(&makerB);
		pthread_mutex_lock(&table);
		printf("Maker B picks up Bread and Lettuce\n");
		printf("Maker B is making sandwich...\n");
		printf("Maker B finished making the sandwich and eats it\n");
		printf("Maker B signals Supplier\n");
		printf("\n");
		pthread_mutex_unlock(&table);
		sem_post(&supp);
	}
	return NULL;
	
}

void *maker_C(void *arg){
	while(1){
		sem_wait(&makerC);
		pthread_mutex_lock(&table);
		printf("Maker C picks up Bread and Cheese\n");
		printf("Maker C is making sandwich...\n");
		printf("Maker C finished making the sandwich and eats it\n");
		printf("Maker C signals Supplier\n");
		printf("\n");
		pthread_mutex_unlock(&table);
		sem_post(&supp);
	}
	return NULL;
	
}
