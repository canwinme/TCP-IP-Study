#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // linux standard file **important
#include <string.h> // string
#include <sys/socket.h> // socket library **important
#include <arpa/inet.h> //network library
// default setting 
int main(int argc, const char * argv[]) //보내주는 값을 수정하지 않겠다는 의미로 const 사용 [*char, *char, ...]
{
    // my server, client socket 
    int server_socket = 0;
    int client_socket = 0;

    struct sockaddr_in server_addr; // server address
    struct sockaddr_in client_addr; //client address
    socklen_t client_addr_size = 0ul;
    char message[BUFSIZ] = {'0'};
    if (argc != 2)
    {
        puts("./HELLO_SERVER 9999\r\n");
        return 1;

        //printf("Usage : %s 9999\r\n", argv[0]); // ip 
    }
    server_socket = socket(PF_INET, SOCK_STREAM, 0); //domain PF_INET -> IPv4 / SOCK_STREAM -> TCP / 0 -> default
    if(server_socket == -1) 
    {
        puts("socket() error\r\n");
        return 1;
    }
    //struct server_addr clear
    memset(&server_addr, 0, sizeof server_addr); //clear -> 구조체 내용물 정리
    server_addr.sin_family = AF_INET; //IPv4; 위에있는 PF_INET 와 이름은 다른데 동일한거긴함 관습적으로 위에는 저렇게적고 여기에는 이렇게적음
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); // 내컴퓨터 ip를 사용하거나 나의 ip 사용 ex) 127.0.0.01; host to network long 
    server_addr.sin_port = htons(atoi(argv[1])); // host to network short aski to int =atoi / argv[1] -> HELLO_WORLD[0] 9999[1]
    //default setting
    int bind_state = bind(server_socket, (const struct sockaddr*)&server_addr, sizeof server_addr);
    if (bind_state == -1)
    {
        puts("bind() error\r\n");
        return 1;
    }
    int listen_state = listen(server_socket, 5); //wait 5 into Queue
    if (listen_state == -1)
    {
        puts("listen() error\r\n");
        return 1;
    }
    
    // send to kernel
    for(;;)
    {
        // blocking 멈춰있다가 클라이언트 프로그램이 오면 그제서야 실행 -> connected
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_size);
        if (client_socket == -1)
        {
            puts("accept() error\r\n");
        }
        int str_length = 0;
        do{
            str_length = read(client_socket, message, BUFSIZ);
            write(client_socket, message, str_length);
        }while(str_length != 0);

    }
    // write(client_socket, message, sizeof message); //sending to client message
    close(client_socket);
    close(server_socket);
    return 0;
}