//Bryce Egley and Bruce Garcia
//CS 444
//Concurrency exercise 2: The Dining Philosophers Problem
//We re-used some of the code from concurrency exercise #1
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include "mt19937ar.c"
#include <assert.h>
#define maxSize 5 //Table will hold 5 plates and forks

typedef struct status {
    int fork;
    char status[15];
} Status;

//Three resources the process will work on
Status resource1[maxSize];
Status resource2[maxSize];
Status resource3[maxSize];

char names [maxSize][15] = {"one", "two","three","four"};

pthread_cond_t thread_one, thread_two, thread_three, thread_four;
pthread_mutex_t thread_mutex;

void think() {
    //Think for 1-20 seconds
    unsigned int think_sleep = (genrand_int32() % 20) + 1;
    while(think_sleep)
        think_sleep = sleep(think_sleep);
}
void eat() {
    //Eat for 2-9 seconds
    unsigned int eat_sleep = (genrand_int32() % 8) + 2;
    while(eat_sleep)
        eat_sleep = sleep(eat_sleep);
}
//Print current results
void print_results() {
    printf("Fork Status\n");
    int i;
    for (i = 0; i < maxSize; i++) {
        if (arrPhilosophers[i].fork == 1) {
            printf("Fork %d is currently held by %s\n",i+1,names[i]);
        } else {
            printf("Fork %d is currently not being held\n",i+1);
        }
    }
    printf("Philosopher Status\n");
    int j;
    for (j = 0; j < maxSize; j++) {
        printf("%s is currently %s\n",names[j],arrPhilosophers[j].status);
    }
    printf("***************************************\n");
}
void get_forks(int n, pthread_cond_t phil) {
    //Change philospher status
    Status getFork;
    //0 for false since you don't have the fork yet
    getFork = (Status){.status = "Getting Fork",.fork = 0};
    pthread_mutex_lock(&thread_mutex);
    arrPhilosophers[n] = getFork;
    //Print new status
    print_results();
    pthread_mutex_unlock(&thread_mutex);
    //Change Status
    getFork = (Status){.status = "Eating",.fork = 1};
    arrPhilosophers[n] = getFork;
    //Lock Mutex for thread use
    pthread_mutex_lock(&thread_mutex);
    pthread_cond_signal(&phil);
    ////Unlock mutex so other threads can use data structure
    pthread_mutex_unlock(&thread_mutex);
    //Output new results
    print_results();
}
void put_forks(int n, pthread_cond_t phil) {
    //Change philospher status
    Status putFork;
    //1 for true since you still have the fork
    putFork = (Status){.status = "Putting Fork",.fork = 1};
    pthread_mutex_lock(&thread_mutex);
    arrPhilosophers[n] = putFork;
    //Print new status
    print_results();
    pthread_mutex_unlock(&thread_mutex);
    //Change Status
    putFork = (Status){.status = "Thinking",.fork = 0};
    arrPhilosophers[n] = putFork;
    //Lock Mutex for thread use
    pthread_mutex_lock(&thread_mutex);
    pthread_cond_signal(&phil);
    ////Unlock mutex so other threads can use data structure
    pthread_mutex_unlock(&thread_mutex);
    //Output new results
    print_results();
}

void *one(void* ptr)
{
    while(1) {
        think();
        get_forks(0,thread_one);
        eat();
        put_forks(0,thread_one);
    }
}

void *two(void* ptr)
{
    while(1) {
        think();
        get_forks(1,thread_two);
        eat();
        put_forks(1,thread_two);
    }
}

void *three(void* ptr)
{
    while(1) {
        think();
        get_forks(2,thread_three);
        eat();
        put_forks(2,thread_three);
    }
}

void *four(void* ptr)
{
    while(1) {
        think();
        get_forks(3,thread_four);
        eat();
        put_forks(3,thread_four);
    }
}

int main() {
    //Have seed generate for random sequnce of numbers with genrand
    init_genrand(time(NULL));
    // Thread ID.
    pthread_t pid_one;
    pthread_t pid_two;
    pthread_t pid_three;
    pthread_t pid_four;
    
    //Create mutex so threads can both use shared resource
    if(pthread_mutex_init(&thread_mutex, NULL)) {
        fprintf(stderr, "Error creating mutex");
        return 1;
    }
    
    //Initialize conditional vars so thread can wait until condition occurs
    pthread_cond_init(&thread_one, NULL);
    pthread_cond_init(&thread_two, NULL);
    pthread_cond_init(&thread_three, NULL);
    pthread_cond_init(&thread_four, NULL);
    
    // Create consumer and producer thread.
    pthread_create(&pid_one, NULL, one, NULL);
    printf("Thread 1 created.\n");
    pthread_create(&pid_two, NULL, two, NULL);
    printf("Thread 2 created.\n");
    pthread_create(&pid_three, NULL, three, NULL);
    printf("Thread 3 created.\n");
    pthread_create(&pid_four, NULL, four, NULL);
    printf("Thread 4 created.\n");
    printf("***************************************\n");
    
    // When done join threads.
    pthread_join(pid_one, NULL);
    pthread_join(pid_two, NULL);
    pthread_join(pid_three, NULL);
    pthread_join(pid_four, NULL);
    
    //Destroy pthreads so they don't continue after ctrl c
    pthread_mutex_destroy(&thread_mutex);
    pthread_cond_destroy(&thread_one);
    pthread_cond_destroy(&thread_two);
    pthread_cond_destroy(&thread_three);
    pthread_cond_destroy(&thread_four);
    printf("Threads are completed.\n");
    
    exit(0);
}
