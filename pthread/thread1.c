#include <pthread.h>
#include <unistd.h> //linux standard library
#include <stdio.h>
void* func_thread1(void*);
void* func_thread2(void*);
unsigned share_variable = 0u;
int main()
{
    pthread_t p_id1 = 0ul;
    pthread_t p_id2 = 0ul;
    int thread_state1 = pthread_create(&p_id1, NULL, func_thread1, NULL);
    if(thread_state1 != 0) // 0 IS OK , others are NOT OK
    {
        puts("pthread_create() error");
        return 1;
    }
    int thread_state2 = pthread_create(&p_id2, NULL, func_thread2, NULL);
    if(thread_state2 != 0) // 0 IS OK , others are NOT OK
    {
        puts("pthread_create() error");
        return 1;
    }
    //sleep(11);
    pthread_join(p_id1, NULL);
    puts("Thread1 End");
    pthread_join(p_id2, NULL);
    puts("Thread2 End");
    puts("Main end");
    return 0;
}

void* func_thread1(void* param)
{
    for(int i = 0; i< 10; ++i)
    {
        puts("RIGHT");
         sleep(1);
    }
    // wait 1second
}

void* func_thread2(void* param)
{
    for (int i = 0; i < 10; ++i)
    {
        puts ("LEFT");
        sleep(1);
    }
    
}