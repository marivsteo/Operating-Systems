#includestdlib.h>
#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<time.h>
#include<string.h>
#include<sys/wait.h>
#include<font1.h>

int pana=0;
int var=0;
void* function(void* param){
    int index = *((int*)param);
    if(index<pana){
        printf("Fiul a creat threadul %d\n",index);
        pthread_t th;
        index+=1;
        pthread_create(&th,NULL,&function,&index);
        pthread_join(th,NULL);
        var+=index-1;
    printf("Threadul %d incrementeaza (var=%d)\n",index-1,var);
    }
    return NULL;
}

int main(int argc, char** argv){
    int numar=atoi(argv[1]);
    pana=numar;
    int p2c[2],c2p[2];
    pipe(p2c);pipe(c2p);
    if(fork()==0){
        close(c2p[0]);close(p2c[1]);close(p2c[0]);
        int i=1;
        pthread_t th;
        pthread_create(&th,NULL,&function,&i);
        pthread_join(th,NULL);
        printf("Fiul a scris in pipe: %d\n",var);
        write(p2c[1],&var,sizeof(int));
        close(c2p[1]);
        exit(0);
    }
    else{
        close(p2c[0]);close(p2c[1]);close(c2p[1]);
        int rd=0;
        read(c2p[0],&rd,sizeof(int));
        printf("Parintele a citit din pipe: %d\n",rd);
        close(c2p[0]);
        wait(0);
    }
    return 0;
}