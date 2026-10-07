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

#define NICK_NAME 20
static void* send_message(void*);
static void* receive_message(void*);
static void error_handling(const char*);
char name[NICK_NAME] = {"[DEFAULT]"};
char message[BUFSIZ] = {'0'};

int main(int argc, const char* argv[])
{
    if(argc != 4)
    {
        error_handling("./MULTI_THREAD_MULTI_CHATTING_CLIENT 127.0.0.1 9999 [NICKNAME]");
    }
    sprintf(name, "[%s]", argv[3]);
    int server_sock = 0;
    struct sockaddr_in server_addr;
    pthread_t send_thread = 0ul;
    pthread_t receive_thread = 0ul;
    server_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(server_sock == -1)
    {
        error_handling("socket() error");
    }
    memset(&server_addr, 0, sizeof server_addr);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]); // -> host number
    server_addr.sin_port = htons(atoi(argv[2]));
    int connect_state = connect(server_sock, (const struct sockaddr*)&server_addr, sizeof server_addr);
    if(connect_state == -1)
    {
        error_handling("connect() error");
    }
    pthread_create(&send_thread, NULL, send_message, (void*)&server_sock); //send
    pthread_create(&receive_thread, NULL, receive_message, (void*)&server_sock); //receive
    pthread_join(send_thread, NULL);
    pthread_join(receive_thread, NULL);
    close(server_sock);
    return 0;
}

void error_handling(const char * _message)
{
    fputs(_message, stdout);
    fputs("\r\n", stdout);
    exit(1);
}

void* send_message(void* args)
{
    int server_sock = *((int*)args); //server_sock
    char nick_name_message[NICK_NAME + BUFSIZ] = {'0'};
    while(true)
    {
        fgets(message, BUFSIZ, stdin);
        // quit -> q / Q
        if(!strcmp(message,"q\n") || !strcmp(message, "Q\n"))
        {
            fputs("Goodbye", stdout);
            close(server_sock);
            break;
        }
        sprintf(nick_name_message, "%s : %s", name, message);
        write(server_sock, nick_name_message, strlen(nick_name_message));
    }
    return NULL;
}

void* receive_message(void* args)
{
    int server_sock = *((int*)args);
    char nick_name_message[NICK_NAME + BUFSIZ] = {'0'};
    int str_length = 0;

    do{
        str_length = read(server_sock, nick_name_message, NICK_NAME + BUFSIZ - 1);
        fputs(nick_name_message, stdout);
        memset(nick_name_message, 0, sizeof nick_name_message);
        
    }while(str_length != 0);

    return NULL;
}