//Client
#include <iostream>
#include <winsock2.h>
using namespace std;
#pragma comment(lib,"ws2_32.lib")
#pragma warning(disable:4996)
//#define SERVER "127.0.0.1"  // or "localhost"
#define BUFLEN 1024  // max length of answer
#define PORT 4444  // the port on which to listen for incoming data

int main(int argc, char* argv[]){
    system("title Klienti");
    // initialise winsock
    WSADATA ws;
    printf("Initialising Winsock...");
    if (WSAStartup(MAKEWORD(2, 2), &ws) != 0)
    {
        printf("Failed. Error Code: %d", WSAGetLastError());
        return 1;
    }
    printf("Initialised.\n");

    // create socket
    sockaddr_in server;
    int client_socket;
    if ((client_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR){ // <<< UDP socket
        cout << WSAStartupError();
        return 2;
    }

    // setup address structure
    memset((char*)&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    //server.sin_addr.S_un.S_addr = inet_addr(SERVER);
    server.sin_addr.S_un.S_addr = inet_addr(argv[1]);

    // start communication
    //print login menu
    printf("[1] Login\n[2] Continue without login\n[9] Exit\n");
    while (true) {
        char message[BUFLEN];
        cout << ">"; //waits for client input
        cin.getline(message, BUFLEN);
        if (strcmp(message, "9") == 0) {
            //exit(3);
            break;
        }

        if (sendto(client_socket, message, strlen(message), 0, (sockaddr*)&server, sizeof(sockaddr_in)) == SOCKET_ERROR)
        {
            printf("sendto() failed with error code: %d", WSAGetLastError());
            return 3;
        }
        char answer[BUFLEN] = {};

        int slen = sizeof(sockaddr_in);
        int answer_length;
        if (answer_length = recvfrom(client_socket, answer, BUFLEN, 0, (sockaddr*)&server, &slen) == SOCKET_ERROR){
            cout << recvfromError();
            exit(0);
        }

    }
    //closesocket(client_socket);
    //WSACleanup();
}