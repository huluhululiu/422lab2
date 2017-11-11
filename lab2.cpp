//
//  main.c
//  lab2
//
//  Created by Leiquan Pan on 11/5/17.
//  Copyright © 2017 Leiquan Pan. All rights reserved.
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <sched.h>

volatile int data[81];
volatile int read_count = 0;
int data_num;
volatile int rounds;
volatile int iterations;
pthread_mutex_t lock_m;
pthread_spinlock_t lock_s;
char *wait_strategy;
char *scheduler;
volatile int num = 0;
pthread_mutex_t lock_tm;
pthread_spinlock_t lock_ts;

void *cube_function(void *ptr)
{
    if (!strcmp(wait_strategy, "spin"))
        pthread_spin_lock(&lock_ts);
    else
        pthread_mutex_lock(&lock_tm);

    int iter = iterations;
    int temp;
    int round = rounds;
    printf("Runnng\n");
    while (round--)
    {
        if (!strcmp(wait_strategy, "spin"))
            pthread_spin_lock(&lock_s);
        else
            pthread_mutex_lock(&lock_m);
        
        //critical section
        temp = data[read_count];
//	printf("The number is %d",temp);
        if(read_count++ >= data_num)
            read_count = 0;
        while (iter--)
            temp = temp*temp*temp;
        //end
        
        if (!strcmp(wait_strategy, "spin"))
            pthread_spin_unlock(&lock_s);
        else
            pthread_mutex_unlock(&lock_m);
    }
	if (!strcmp(wait_strategy, "spin"))
            pthread_spin_unlock(&lock_ts);
        else
            pthread_mutex_unlock(&lock_tm);

//    printf("Finished\n");
}

int main(int argc, char * argv[]) {
    const char *message1 = "Thread 1";
    const char *message2 = "Thread 2";
    int thread_s2, thread_s1;
    int t_cpu_s1, t_cpu_s2;
    cpu_set_t cpuset;
    int core_num;
    int policy;
    
    pthread_t thread[8];
    int i = 0;
    
    //check input
    if(argc < 6)
    {
        printf("Usage <program_name> <scheduler> <wait-strategy> <rounds> <iterations> <number>+ ");
    }
    
    scheduler = argv[1];
    wait_strategy = argv[2];
    rounds = atoi(argv[3]);
    iterations = atoi(argv[4]);
    for(i = 5; i < argc ; i ++)
        data[i - 5] = atoi(argv[i]);
    data_num = argc - 5;
    
    if( (strcmp(scheduler,"SCHED_RR") && strcmp(scheduler,"SCHED_FIFO") && strcmp(scheduler,"SCHED_NORMAL")) || (strcmp(wait_strategy,"spin") && strcmp(wait_strategy,"sleep")))
    {
        printf("Invalid aruments\n");
        return -1;
    }
    
    // initialize lock
    pthread_mutex_init(&lock_m, NULL);
    pthread_spin_init(&lock_s, 0);
    pthread_mutex_init(&lock_tm, NULL);
    pthread_spin_init(&lock_ts, 0);

    //check the policy
    if (!strcmp(scheduler, "SCHED_RR"))
        policy = SCHED_RR;
    else if (!strcmp(scheduler, "SCHED_FIFO"))
        policy = SCHED_FIFO;
    else if (!strcmp(scheduler, "SCHED_NORMAL"))
        policy = SCHED_OTHER;

    //lock the thread
    if (!strcmp(wait_strategy, "spin"))
        pthread_spin_lock(&lock_ts);
    else
        pthread_mutex_lock(&lock_tm);

    //create thread
    for (core_num = 0; core_num < 4; core_num++)
    {
	printf("Thread %d is creating \n",core_num*2); 
        thread_s1 = pthread_create( &thread[core_num*2], NULL, cube_function, (void*) message1);
        if(thread_s1)
        {
            fprintf(stderr,"Error - pthread_create(thread1) return code: %d\n",thread_s1);
            return -1;
        }
	printf("Thread %d is creating\n",core_num*2+1); 
	thread_s2 = pthread_create( &thread[core_num*2+1], NULL, cube_function, (void*) message2);
        if(thread_s2)
        {
            fprintf(stderr,"Error - pthread_create(thread2) return code: %d\n",thread_s2);
            return -1;
        }
       // printf("Thread %d is created but not set",core_num*2+1);
        //set cpu
        CPU_ZERO(&cpuset);
        CPU_SET(core_num, &cpuset);
    
        // set affinity
	printf("Thread %d is setting\n",core_num*2);
        t_cpu_s1 = pthread_setaffinity_np(thread[core_num*2], sizeof(cpu_set_t), &cpuset);
        if (t_cpu_s1 != 0)
        {
            fprintf(stderr,"Error - pthread_setaffinity_np(thread1) return code: %d\n",t_cpu_s1);
        }
	printf("Thread %d is setting\n",core_num*2+1);
//        printf("Thread %d has already created\n",core_num*2);
        t_cpu_s2 = pthread_setaffinity_np(thread[core_num*2+1], sizeof(cpu_set_t), &cpuset);
        if (t_cpu_s2 != 0)
        {
            fprintf(stderr,"Error - pthread_setaffinity_np(thread2) return code: %d\n",t_cpu_s2);
        }
//        printf("Thread %d has already created\n",core_num*2+1);
        //
    }

    if (!strcmp(wait_strategy, "spin"))
        pthread_spin_unlock(&lock_ts);
    else
        pthread_mutex_unlock(&lock_tm);

    for(int i = 0; i < 8; i++)
    {
	pthread_join( thread[i],NULL);
    }
    //destroy lock
    pthread_mutex_destroy(&lock_m);
    pthread_spin_destroy(&lock_s);
    pthread_mutex_destroy(&lock_tm);
    pthread_spin_destroy(&lock_ts);

    printf("Hello, World!\n");
    exit(EXIT_SUCCESS);
    return 0;
}

