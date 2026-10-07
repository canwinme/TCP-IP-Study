#include <stdio.h>
#include <stdlib.h> //BUFSIZ, malloc
#include <string.h>
#include <unistd.h> //linux standard
#include <stdbool.h> //true false
#include <arpa/inet.h> //inet() : 127.0.0.1 -> host number
#include <sys/types.h>
#include <sys/socket.h> //socket func
#include <netinet/in.h>
#include <pthread.h> 
#include <assert.h>
#define MAX_CLIENT 256
//const uint32_t MAX_CLIENT = 256; 
static int client_count = 0;
static int client_sockets[MAX_CLIENT] = {0,}; //stores client's socket number;
static pthread_mutex_t mutex_key = PTHREAD_MUTEX_INITIALIZER; //nullptr
static void error_handling(const char*);
static void sending_message(const char*, int);
static void* handling_client(void*); //thread
int main(int argc, const char* *argv) // char* *argv, char* argv[] 도 가능 
{
    int server_sock = 0;
    int client_sock = 0;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    pthread_t pthread_id = 0ul;
    memset(&server_addr, 0, sizeof server_addr); //server_addr clear
    memset(&client_addr, 0, sizeof client_addr); //client_addr clear
    size_t client_addr_size = sizeof client_addr; //client socket's size
    if(argc != 2)
    {
        error_handling("./MULTI_THREAD_MULTI_CHATTING_SEVER 9999");
    }
    pthread_mutex_init(&mutex_key, NULL); //initializing 
    server_sock = socket(PF_INET/*IPv4*/, SOCK_STREAM/*TCP*/,0);
    if(server_sock == -1)
    {
        error_handling("SOCKET() ERROR");
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[1]));
    server_addr.sin_addr.s_addr= htonl(INADDR_ANY);
    if(bind(server_sock, (const struct sockaddr*)&server_addr, sizeof server_addr) == -1)
    {
        error_handling("BIND() ERROR");
    } 
    if(listen(server_sock , 5/*default = 5*/) == -1)
    {
        error_handling("LISTEN() ERROR");
    }
    while(true)
    {
        if(client_count > MAX_CLIENT)
        {
            fprintf(stdout,"%s\r\n", "Server is full");
            continue;
        }
        client_sock = accept(server_sock, (struct sockaddr*)&client_addr, (socklen_t*)&client_addr_size);
        if(client_sock == -1)
        {
            error_handling("ACCEPT() ERROR");
        }
        int pthread_state = pthread_create(&pthread_id, NULL, handling_client, (void*)&client_sock);
        if(pthread_state != 0)
        {
            error_handling("PTHREAD_CREATE() ERROR");
        }
        //arrage clients 클라이언트들 정렬
        pthread_mutex_lock(&mutex_key);
        client_sockets[client_count++] = client_sock;
        pthread_mutex_unlock(&mutex_key);
        pthread_detach(pthread_id); //No join() // main thread가 안끝나고 있어줘야하는데 끝날때까지 멈춰주는 역할 그래서 떨어트려서 detach를사용
        fprintf(stdout, "Connected Client IP :  %s\r\n", inet_ntoa(client_addr.sin_addr)); // check ip
    }
    close(server_sock);
    return 0;
}
void error_handling(const char* _message)
{
    fputs(_message, stdout);
    fputs("\r\n", stdout);
    exit(1);
}

void sending_message(const char* _message, int _str_length)
{
    pthread_mutex_lock(&mutex_key);
    for(int i = 0; i< client_count; ++i)
    {
        write(client_sockets[i], _message, _str_length);
    }
    pthread_mutex_unlock(&mutex_key);
}

//threading
void* handling_client(void *args)
{
    int client_socket = *((int*)args); //void type 이기 때문에 형변환을하고 값을 가져오라고해야함.
    int str_length = 0;
    char message[BUFSIZ] = {'0'};
    do{
        str_length = read(client_socket, message, BUFSIZ - 1); //read
        sending_message(message, str_length); //write
        memset(message, 0, BUFSIZ);
    }while(str_length != 0);
    //나간놈찾기
    pthread_mutex_lock(&mutex_key);
    for (int i = 0; i < client_count; ++i)
    {
        if(client_count == client_sockets[i])
        {
            while(i++ < client_count - 1)
            {
                client_sockets[i] = client_sockets[i + 1];
            }
            break;
        }
    }
    pthread_mutex_unlock(&mutex_key);
    --client_count;
    close(client_socket);
    return NULL;
}