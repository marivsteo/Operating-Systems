#includestdlib.h>
#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<time.h>
#include<string.h>

pthread_mutex_t mutexNr=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexText=PTHREAD_MUTEX_INITIALIZER;
pthread_t tid[11];
pthread_cond_t cond;
int n,th;
char prop[256];

void* nr(void* param){
    for( ; ;){
        pthread_mutex_lock(&mutexNr);
        srand(time(NULL));
        n=1+n%strlen(prop);
        //generat=1;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutexNr);
        //generat=0;
        sleep(2);
    }
    return NULL;
}

void* text(void* param){
    for( ; ;){
        //while(generat!=1);
        pthread_cond_wait(&cond,&mutexNr);
        pthread_mutex_lock(&mutexText);
        memmove(&prop[n],&prop[n+1],strlen(prop)-n);
        printf("The new prop is: %s\n",prop);
        pthread_mutex_unlock(&mutexText);
    }
    return NULL;
}

int main(){
    pthread_cond_init(&cond,NULL);
    int i=0;
    printf("Introduceti o propozitie: ");
    scanf("%[^n]s",prop);
    printf("Introduceti nr: ");
    scanf("%d",&th);
    pthread_create(&tid[i],NULL,nr,&i);
    for(i=1;i<=th;i++)
        pthread_create(&tid[i],NULL,text,&i);
    for(i=0;i<=th;i++)
        pthread_join(tid[i],NULL);
    pthread_mutex_destroy(&mutexNr);
    pthread_mutex_destroy(&mutexText);
    pthread_cond_destroy(&cond);
    return 0;
}