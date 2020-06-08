
#include <unistd.h>     /* Symbolic Constants */
#include <sys/types.h>  /* Primitive System Data Types */
#include <errno.h>      /* Errors */
#include <stdio.h>      /* Input/Output */
#include <stdlib.h>     /* General Utilities */
#include <pthread.h>    /* POSIX Threads */
#include <string.h>     /* String handling */
#include <iostream>
#include <time.h>
#include <sys/syscall.h>
using namespace std;

pthread_mutex_t mutex1;
char name[4]={'B','M','S','C'};
char nameCh[4][50]={"咖啡豆","牛奶","焦糖","肉桂"};
int ingredient[4]={0}; //0->B, 1->M, 2->S, 3->C
int CM_num[4]={0}; //總共做出幾杯
int fc_num;
int coffee=0;

struct thread_FCdata {
    unsigned long thread_id;
    char hold[1];
};
struct thread_CMdata{
    int pos; //No one is holding
};

void* CM_Threading(void *ptr)
{
    thread_CMdata *data;
    data = (thread_CMdata *) ptr;  /* type cast to a pointer to thdata */
    
    int sum;
    while(coffee<100)
    {
        sum = 0;
        for(int i=0;i<4;i++) // check the number of ingredient
            if(ingredient[i]==1)
                sum++;
        
        if(sum==3 && ingredient[data->pos]==1);
        else if(sum==4) sleep(5);
        else
        {
            while(pthread_mutex_lock(&mutex1)!=0);
            if(coffee>=100) break;
            int n;
            for(n=rand()%4; ingredient[n]!=0; n=rand()%4);
            
            ingredient[n]=1;
            cout<<"CM: ";
            switch(n)
            {
                case 0:
                    cout<<"B"<<endl;
                    break;
                case 1:
                    cout<<"M"<<endl;
                    break;
                case 2:
                    cout<<"S"<<endl;
                    break;
                case 3:
                    cout <<"C"<<endl;
            }
            pthread_mutex_unlock(&mutex1);
        }
    }
    pthread_exit(0);
}

void* FC_Threading(void *ptr)
{
    thread_FCdata *data;
    data = (thread_FCdata *) ptr;  /* type cast to a pointer to thdata */

    if(!strcmp(data->hold,"B"))//0
    {
        while(coffee<100)
        {
            while(pthread_mutex_lock(&mutex1)!=0);
          
            if(ingredient[1] && ingredient [2] &&ingredient[3])
            {
                ingredient[1]=0;
                ingredient[2]=0;
                ingredient[3]=0;
                cout<<"FC(B): 第 "<<++coffee<<" 杯特製咖啡"<<endl;//FC(S): 第 1 杯特製咖啡
                CM_num[0]++;
            }
            pthread_mutex_unlock(&mutex1);
        }
        
    }
    else if(!strcmp(data->hold,"M")) //1
    {
        while(coffee<100)
        {
            while(pthread_mutex_lock(&mutex1)!=0);
            
            if(ingredient[0] && ingredient[2] &&ingredient[3])
            {
                ingredient[0]=0;
                ingredient[2]=0;
                ingredient[3]=0;
                cout<<"FC(M): 第 "<<++coffee<<" 杯特製咖啡"<<endl;
                CM_num[1]++;
            }
            pthread_mutex_unlock(&mutex1);
        }
    }
    else if(!strcmp(data->hold,"S"))//2
    {
        while(coffee<100)
        {
            while(pthread_mutex_lock(&mutex1)!=0);
         
            if(ingredient[0] && ingredient [1] && ingredient[3])
            {
                ingredient[0]=0;
                ingredient[1]=0;
                ingredient[3]=0;
                cout<<"FC(S): 第 "<<++coffee<<" 杯特製咖啡"<<endl;
                CM_num[2]++;
            }
            pthread_mutex_unlock(&mutex1);
        }
    }
    
    else if(!strcmp(data->hold,"C"))//3
    {
        while(coffee<100)
        {
            while(pthread_mutex_lock(&mutex1)!=0);
            
            if(ingredient[0] && ingredient [1] && ingredient[2])
            {
                ingredient[0]=0;
                ingredient[1]=0;
                ingredient[2]=0;
                cout<<"FC(C): 第 "<<++coffee<<" 杯特製咖啡"<<endl;
                CM_num[3]++;
            }
            pthread_mutex_unlock(&mutex1);
        }
    }
    pthread_exit(0);
}



int main(int argc, char* argv[]) {
    srand(time(NULL));
    int option;
    cin >> option;
    if(option==0)
{
        fc_num=3;

    pthread_t CM_t;
    pthread_t* FC_t = new pthread_t [fc_num];
    thread_FCdata* FC_data = new thread_FCdata [fc_num];
    thread_CMdata CM_data;
    
    for (int i = 0; i < fc_num; i++)//input B or M or S or C
    {
        cin >> FC_data[i].hold;
    
        for(int j=0;j<4;j++)
        {
            if(FC_data[i].hold[0]!=name[j])
                CM_data.pos = j;
            else
                FC_data[i].thread_id = j;
        }
    }
    
    pthread_mutex_init(&mutex1, NULL); //initialization
    
    /* create threads */
    pthread_create (&CM_t, NULL, CM_Threading, (void *) &CM_data);
    for (int i = 0; i < fc_num; i++)
        pthread_create(&FC_t[i], NULL, FC_Threading, (void*) &FC_data[i]);
    
    pthread_join(CM_t, NULL);
    for(int i = 0; i < fc_num; i++)
        pthread_join(FC_t[i], NULL);
    
    for (int i = 0; i < fc_num; i++)//input B or M or S or C
    {
        cout << "FC(" << FC_data[i].hold << "): 做出 " <<  CM_num[FC_data[i].thread_id]<<"杯，剩 "
        << 100-CM_num[FC_data[i].thread_id] << " 份"<< nameCh[FC_data[i].thread_id] << endl;
    }
	pthread_mutex_destroy(&mutex1);
}
    return 0;
}

