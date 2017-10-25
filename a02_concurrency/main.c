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

//Information on building structs used and modified from the following
//https://stackoverflow.com/questions/32698293
typedef struct data {
    int number;
    int time;
} Data;

pthread_cond_t thread_plato, thread_aristotle, thread_socrates, thread_confucius, thread_epicurus;
pthread_mutex_t thread_mutex;

//Array data structure of 32 size
Data forks[maxSize];
Data plates[maxSize];

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
void get_forks() {
    
    //TODO
    
    
}
void put_forks() {
    
    //TODO
    
}

void *plato(void* ptr)
{
    while(1) {
        think();
        get_forks();
        eat();
        put_forks();
    }
}

void *aristotle(void* ptr)
{
    while(1) {
        think();
        get_forks();
        eat();
        put_forks();
    }
}

void *socrates(void* ptr)
{
    while(1) {
        think();
        get_forks();
        eat();
        put_forks();
    }
}

void *confucius(void* ptr)
{
    while(1) {
        think();
        get_forks();
        eat();
        put_forks();
    }
}

void *epicurus(void* ptr)
{
    while(1) {
        think();
        get_forks();
        eat();
        put_forks();
    }
}

int main() {
    //Have seed generate for random sequnce of numbers with genrand
    init_genrand(time(NULL));
    // Thread ID.
    pthread_t tidPlato;
    pthread_t tidAristotle;
    pthread_t tidSocrates;
    pthread_t tidConfucius;
    pthread_t tidEpicurus;
    
    //Create mutex so threads can both use shared resource
    if(pthread_mutex_init(&thread_mutex, NULL)) {
        fprintf(stderr, "Error creating mutex");
        return 1;
    }
    
    //Initialize conditional vars so thread can wait until condition occurs
    pthread_cond_init(&thread_plato, NULL);
    pthread_cond_init(&thread_aristotle, NULL);
    pthread_cond_init(&thread_socrates, NULL);
    pthread_cond_init(&thread_confucius, NULL);
    pthread_cond_init(&thread_epicurus, NULL);
    
    // Create consumer and producer thread.
    pthread_create(&tidPlato, NULL, plato, NULL);
    printf("PLato thread created.\n");
    pthread_create(&tidAristotle, NULL, aristotle, NULL);
    printf("Aristotle thread created.\n");
    pthread_create(&tidSocrates, NULL, socrates, NULL);
    printf("Socrates thread created.\n");
    pthread_create(&tidConfucius, NULL, confucius, NULL);
    printf("Confucius thread created.\n");
    pthread_create(&tidEpicurus, NULL, epicurus, NULL);
    printf("Epicurus thread created.\n");
    
    // When done join threads.
    pthread_join(tidPlato, NULL);
    pthread_join(tidAristotle, NULL);
    pthread_join(tidSocrates, NULL);
    pthread_join(tidConfucius, NULL);
    pthread_join(tidEpicurus, NULL);
    
    //Destroy pthreads so they don't continue after ctrl c
    pthread_mutex_destroy(&thread_mutex);
    pthread_cond_destroy(&thread_plato);
    pthread_cond_destroy(&thread_aristotle);
    pthread_cond_destroy(&thread_socrates);
    pthread_cond_destroy(&thread_confucius);
    pthread_cond_destroy(&thread_epicurus);
    printf("Threads are completed.\n");
    
    exit(0);
}
