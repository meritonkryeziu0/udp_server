//-Serveri -
#include <iostream>
#include <winsock2.h>
#include <typeinfo>
#include <fstream>
#include <string>
using namespace std;

#pragma comment(lib,"ws2_32.lib") // Winsock Library
#pragma warning(disable:4996)

#define BUFLEN 1024
#define PORT 4444

int fileWrite(string filename, const char* textToWrite) {
    ofstream file;
    file.write(textToWrite, sizeof(textToWrite));    //write into file  
    file.close();   //close the file  
    file.open(filename);
    file << textToWrite << endl;
    file.close();
    return 0;
}

string fileContent(string filename) {	
    string data;	
    string str;	
    fstream rfile;	
    rfile.open(filename);	
    while (!rfile.eof()) {	
        rfile >> data;	
        str.append(data);	
        str.append("\n");	
    }	
    return str;	
}	
string recvfromError() {	
    return "recvfrom() failed with error code: " + WSAGetLastError();	
}	
string WSAStartupError() {	
    return "Could not create socket: "+ WSAGetLastError();	
}	
string fileList() {	
    const char* cmd = "FORFILES /c \"cmd /c echo @fname\" > files.txt";	
    system(cmd);	
    string contents = fileContent("files.txt");	
    //system("del files.txt");	
    return contents;	
}	
void executeFile(const char* filename) {	
    system(filename);	
}
    
int main() {
    system("title UDP Server");
    sockaddr_in server, client;

    // initialise winsock
    WSADATA wsa;
    printf("Initialising Winsock...");
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Failed. Error Code: %d", WSAGetLastError());
        exit(0);
    }
    printf("Initialised.\n");

    // create a socket
    SOCKET server_socket;
    if ((server_socket = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET) {
        printf("Could not create socket: %d", WSAGetLastError());
    }
    printf("Socket created.\n");

    // prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    // bind
    if (bind(server_socket, (sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
    {
        printf("Bind failed with error code: %d", WSAGetLastError());
        exit(EXIT_FAILURE);
    }
    puts("Bind done.");
    while (true) {
        printf("Waiting for data...");
        fflush(stdout);
        char message[BUFLEN] = {};
        int message_len;
        int slen = sizeof(sockaddr_in);
        if (message_len = recvfrom(server_socket, message, BUFLEN, 0, (sockaddr*)&client, &slen) == SOCKET_ERROR) {
            cout << recvfromError();
            exit(0);
        }
        printf("Received packet from %s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
        cout << "userHasAccess: " << userHasAccess << endl;

        if (sendto(server_socket, message, strlen(message), 0, (sockaddr*)&client, sizeof(sockaddr_in)) == SOCKET_ERROR){
            printf("sendto() failed with error code: %d", WSAGetLastError());
            return 3;
        }
        cout << message << endl;

        closesocket(server_socket);
        WSACleanup();
    }
    return 0;
}
