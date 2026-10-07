#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // linux standard file **important
#include <string.h> // string
#include <sys/socket.h> // socket library **important
#include <arpa/inet.h> //network library
int main(int argc, const char* argv[])
{
    if(argc != 3)
    {
        puts("./HELLO_CLIENT 127.0.0.1 9999\r\n");
        puts("./HELLO_CLIENT 172.22.126.152 9999\r\n");
        return 1;
    }
    int server_socket = 0;
    struct sockaddr_in server_addr;
    char message[BUFSIZ] = {'0'}; //message buffer that sent to me 
    server_socket = socket(PF_INET, SOCK_STREAM, 0); //IPv4 , TCP
    if(server_socket == -1)
    {
        puts("SOCKET() ERROR!");
        return 1;
    }
    memset(&server_addr, 0 , sizeof server_addr); //initialize
    server_addr.sin_family = AF_INET; //IPv4
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr.sin_port = htons(atoi(argv[2]));
    int connect_state = connect(server_socket, (const struct sockaddr*)&server_addr, sizeof server_addr);
    if(connect_state == -1)
    {
        puts("CONNECT() ERROR");
        return 1;
    }
    int length = read(server_socket,message, sizeof message - 1);
    if (length == -1)
    {
        puts("Read() ERROR");
        return 1;
    }
    fprintf(stdout, "Message from server : %s\r\n", message);
    close(server_socket);
    return 0;

}