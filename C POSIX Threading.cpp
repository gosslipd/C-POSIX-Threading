/******************************************************************************

                            Online C Compiler.
                Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.
                See https://www.onlinegdb.com/online_c_compiler
*******************************************************************************/

// Copyright Peter Gossling 17/06/2024
//

#include <stdio.h>
#include <pthread.h>
#include <unistd.h> // For sleep

pthread_cond_t cv;               // Condition variable used to signal that a thread has comleted its turn.
pthread_mutex_t m;                // Mutex used by condition variable.

int turn = 1;           // Indicates which thread has clearance to run at any one time.
#define nThreads (4)    // Number of identical threads to create.

// Thread function run be each thread.
void* thread(void* pData)
{
    int threadNumber = (int)pData;  // Warnings! This is why C++ has reinterpret_cast

    printf("Thread%d started\n", threadNumber);

    sleep(5);   // Sleep 5 seconds

    for (int i = 0; i < 10; ++i)
    {
        pthread_mutex_lock(&m);

        while (turn != threadNumber)
        {
            pthread_cond_wait(&cv, &m);
        }

        printf("Thread%d: i = %d\n", threadNumber, i);

        ++turn;
        if (turn > nThreads)
        {
            turn = 1;
        }

        pthread_cond_broadcast(&cv);

        pthread_mutex_unlock(&m);
    }
    printf("Thread %d exiting!\n", threadNumber);

    pthread_exit(NULL);
}

// Create multiple threads running identical code that take it in turns to run.
int main(int argc, char* argv[])
{
    printf("Multi-threading using POSIX threads!\n");

    pthread_mutex_init(&m, NULL);
    pthread_cond_init(&cv, NULL);

    pthread_t t[nThreads];

    printf("Creating...\n");
    for (int i = 0; i < nThreads; ++i)
    {
        int rc = pthread_create(&t[i], NULL, thread, (void*)(i + 1));  // Warnings! This is why C++ has reinterpret_cast
        if (rc)
        {
            printf("Error creating thread %d: %d\n", i + 1, rc);
        }
        else
        {
            printf("Created thread %d\n", i + 1);
        }
    }
    printf("\n");

    printf("Joining...\n");
    for (int i = 0; i < nThreads; ++i)
    {
        int rc = pthread_join(t[i], NULL);
        if (rc)
        {
            printf("Error creating thread %d: %d\n", i + 1, rc);
        }
        else
        {
            printf("Joined thread %d\n", i + 1);
        }
    }
    printf("\n");

    pthread_mutex_destroy(&m);
    pthread_cond_destroy(&cv);
    pthread_exit(NULL);

    return 0;
}