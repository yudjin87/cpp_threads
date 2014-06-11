#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <atomic>
#include "mt.h"
#include "sched.h"

sem_t beginSema1;
sem_t beginSema2;
sem_t endSema;

//std::atomic<int> X, Y;
//std::atomic<int> r1, r2;
int X, Y;
int r1, r2;

void *thread1Func(void *param)
{
    MersenneTwister random(1);
    for (;;)
    {
        sem_wait(&beginSema1);  // Wait for signal
        while (random.integer() % 8 != 0) {}  // Random delay

        X = 1;
        asm volatile( "" ::: "memory");
        r1 = Y;

        sem_post(&endSema);  // Notify transaction complete
    }
    return NULL;  // Never returns
};

void *thread2Func(void *param)
{
    MersenneTwister random(2);
    for (;;)
    {
        sem_wait(&beginSema2);  // Wait for signal
        while (random.integer() % 8 != 0) {}  // Random delay

        Y = 1;
        asm volatile( "" ::: "memory");
        r2 = X;
        sem_post(&endSema);  // Notify transaction complete
    }
    return NULL;  // Never returns
};

int main()
{
    // Initialize the semaphores
    sem_init(&beginSema1, 0, 0);
    sem_init(&beginSema2, 0, 0);
    sem_init(&endSema, 0, 0);

    // Spawn the threads
    pthread_t thread1, thread2;
    pthread_create(&thread1, NULL, thread1Func, NULL);
    pthread_create(&thread2, NULL, thread2Func, NULL);

    cpu_set_t cpus;
    CPU_ZERO(&cpus);
    CPU_SET(0, &cpus);
    pthread_setaffinity_np(thread1, sizeof(cpu_set_t), &cpus);
    CPU_ZERO(&cpus);
    CPU_SET(1, &cpus);
    pthread_setaffinity_np(thread2, sizeof(cpu_set_t), &cpus);


    int detected = 0;
    for (int iterations = 1; ; iterations++)
    {
        // Reset X and Y
        X = 0;
        Y = 0;

        // Signal both threads
        sem_post(&beginSema1);
        sem_post(&beginSema2);

        // Wait for both threads
        sem_wait(&endSema);
        sem_wait(&endSema);

        // Check if there was a simultaneous reorder
        if (r1 == 0 && r2 == 0)
        {
            detected++;
            printf("%d reorders detected after %d iterations\n", detected, iterations);
        }
    }

    return 0;  // Never returns
}

