#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int n;
int *fibonacci;

void *calc_fibonacci(void *arg);
void *search(void *arg);

int main(){
	pthread_t t1;
	pthread_t t2;
	printf("ENTER THE TERM OF FIBONACCI SEQUENCE:\n");
	scanf("%d",&n);
	pthread_create(&t1,NULL, calc_fibonacci, NULL);
	pthread_join(t1,NULL);
	
	for(int i=0;i<=n;i++){
		printf("a[%d] = %d\n", i, fibonacci[i]);
	}
	pthread_create(&t2,NULL, search, NULL);
	pthread_join(t2,NULL);
	free(fibonacci);
	return 0;
}

void *calc_fibonacci(void *arg){
	fibonacci = (int*)malloc((n+1)*sizeof(int));
	int f =0;
	int s =1;
	int next;
	for(int i=0;i<=n;i++){
		fibonacci[i] = f;
		next = f+s;
		f = s;
		s= next;
	} 
	pthread_exit(fibonacci);
}
void *search(void *arg){
	int count;
	printf("HOW MANY NUMBERS ARE YOU WILLING TO SEARCH?:\n");
	scanf("%d",&count);
	
	for(int i=0;i<count;i++){
		int idx;
		printf("ENTER SEARCH %d:\n",i+1);
		scanf("%d",&idx);
		if(idx <=n && idx >=0){
			printf("RESULT OF SEARCH #%d = %d\n", i+1,fibonacci[idx]);
		}
		else{
			printf("RESULT OF SEARCH #%d = -1\n", i+1);
		}
	}
	pthread_exit(NULL);

}
	
	
	
	
	

