#include <pthread.h>
#include <unistd.h> //linux standard library
#include <stdio.h>

unsigned long long shared_number = 0ull; // global variable = critical area = 임계영역
pthread_mutex_t mutex; //key
const int MULTI_THREAD = 100;  
void* thread_increasing(void*);
void* thread_decreasing(void*);
int main()//int argc, const char* argv -> 콘솔창에 안나오고 그냥 실행하겠다 
{
    pthread_t ids[100]; //thread id 100 array
    fprintf(stdout, "The shared number : %llu\r\n", shared_number);
    // mutex key initialize
    pthread_mutex_init(&mutex /*key*/, NULL); 
    for (int i =0; i < MULTI_THREAD; ++i)
    {
        if (i % 2) // 홀수
        {
            int thread_state = pthread_create(&ids[i], NULL, thread_increasing, NULL); // shared number increasing  = ++1
            if(thread_state)
            {
                fprintf(stdout, "%s\r\n", "pthread_create() error");
            }
        }
        else //짝수
        {
            int thread_state = pthread_create(&ids[i], NULL, thread_decreasing, NULL); // shared number decreasing  = --1
            if(thread_state)
            {
                fprintf(stdout, "%s\r\n", "pthread_create() error");
            }
        }
    } 
    for (int i = 0; i < MULTI_THREAD; ++i)
    {
        pthread_join (ids[i], NULL); //main thread waiting
    }
    fprintf(stdout, "The shared number : %llu\r\n", shared_number);
    pthread_mutex_destroy(&mutex); // mutex release
    return 0;
}

void* thread_increasing(void* args)
{
    pthread_mutex_lock(&mutex); // gain a key
    for (int i = 0; i< 1000000; ++i)
    {
        ++shared_number;
    }
    pthread_mutex_unlock(&mutex); // return a key
    return NULL; 
}

void* thread_decreasing(void* args)
{
    pthread_mutex_lock(&mutex); // gain a key
    for (int i = 0; i< 1000000; ++i)
    {
        --shared_number;
    }
    pthread_mutex_unlock(&mutex); // return a key
    return NULL; 
}