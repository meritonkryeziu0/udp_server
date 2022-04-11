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
    return "Could not create socket: " + WSAGetLastError();
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
        bool userHasAccess = false;
        // try to receive some data, this is a blocking call
        int message_len;
        int slen = sizeof(sockaddr_in);
        if (message_len = recvfrom(server_socket, message, BUFLEN, 0, (sockaddr*)&client, &slen) == SOCKET_ERROR) {
            cout << recvfromError();
            exit(0);
        }
        else if (strcmp(message, "1") == 0) {
            //else if (true) {
            char username[BUFLEN] = {};
            char password[BUFLEN] = {};
            int username_len;
            int password_len;
            //Send login, print errors if they occured
            if (sendto(server_socket, "-Enter username:", strlen("-Enter username:"), 0, (sockaddr*)&client, slen) == SOCKET_ERROR) { cout << recvfromError(); return 3; }
            if (username_len = recvfrom(server_socket, username, BUFLEN, 0, (sockaddr*)&client, &slen) == SOCKET_ERROR) { cout << recvfromError(); exit(0); }
            if (sendto(server_socket, "-Enter password:", strlen("-Enter password:"), 0, (sockaddr*)&client, slen) == SOCKET_ERROR) { cout << recvfromError(); return 3; }
            if (password_len = recvfrom(server_socket, password, BUFLEN, 0, (sockaddr*)&client, &slen) == SOCKET_ERROR) { cout << recvfromError(); exit(0); }
            cout << "username: " << username << endl;
            cout << "passwrod: " << password << endl;
            if (strcmp(username, "admin") == 0 && strcmp(password, "admin") == 0) {
                userHasAccess = true;
            }
            else {
                userHasAccess = false;
            }

            if (userHasAccess) {
                while (true) {
                menu:
                    char choice[BUFLEN] = {};
                    int choice_len;
                    char filename[BUFLEN] = {};
                    int filename_len;
                    char overWriteText[1024 * 8] = {};
                    int overWriteText_len;
                    string fileStr;
                    if (sendto(server_socket, "[1] List files\n[2] Read files\n[3] Write to file\n[4] Execute\n[9] Exit", strlen("[1] List files\n[2] Read files\n[3] Write to file\n[4] Execute\n[9] Exit"), 0, (sockaddr*)&client, slen) == SOCKET_ERROR) {
                        cout << recvfromError(); return 3;
                    }
                    if (choice_len = recvfrom(server_socket, choice, BUFLEN, 0, (sockaddr*)&client, &slen) == SOCKET_ERROR) {
                        cout << recvfromError(); exit(0);
                    }
                    printf("Choice: %s", choice);
                    cout << "\n\n" << (int)choice[0] << endl;

                    switch (choice[0]) {
                    case '1': //list files
                        fileStr = fileList();
                        if (sendto(server_socket, fileStr.c_str(), strlen(fileStr.c_str()), 0, (sockaddr*)&client, sizeof(sockaddr_in)) == SOCKET_ERROR) {
                            cout << recvfromError(); return 3;
                        }
                        goto menu;
                        break;
                    case '2': //read files
                        if (sendto(server_socket, "-Jepni emrin e file: ", strlen("-Jepni emrin e file: "), 0, (sockaddr*)&client, sizeof(sockaddr_in)) == SOCKET_ERROR) { cout << recvfromError(); return 3; }

                        if (filename_len = recvfrom(server_socket, filename, BUFLEN, 0, (sockaddr*)&client, &slen) == SOCKET_ERROR) { cout << recvfromError(); exit(0); }

                        fileStr = fileContent(filename);
                        if (sendto(server_socket, fileStr.c_str(), strlen(fileStr.c_str()), 0, (sockaddr*)&client, sizeof(sockaddr_in)) == SOCKET_ERROR) { cout << recvfromError(); return 3; }
                        goto menu;
                        break;
                            case '3': //write to file	
                        if (sendto(server_socket, "-Jepni emrin e file te cilin deshironi ta ndryshoni: ", strlen("-Jepni emrin e file te cilin deshironi ta ndryshoni: "), 0, (sockaddr*)&client, sizeof(sockaddr_in)) == SOCKET_ERROR) {	
                            cout << recvfromError(); return 3;	
                        }	
                        if (filename_len = recvfrom(server_socket, filename, BUFLEN, 0, (sockaddr*)&client, &slen) == SOCKET_ERROR) { cout << recvfromError(); exit(0); }	
                        cout << filename;	
                        if (sendto(server_socket, "-Jepni permbajtjen e re:", strlen("-Jepni permbajtjen e re:"), 0, (sockaddr*)&client, sizeof(sockaddr_in)) == SOCKET_ERROR) {	
                            cout << recvfromError(); return 3;	
                        }	
                        if (overWriteText_len = recvfrom(server_socket, overWriteText, BUFLEN, 0, (sockaddr*)&client, &slen) == SOCKET_ERROR) { cout << recvfromError(); exit(0); }	
                        fileWrite(filename, overWriteText);	
                        cout << overWriteText;	
                        goto menu;	
                        break;	
                    case '4': //execute	
                        if (sendto(server_socket, "-Jepni emrin e file te cilin deshironi ta ekzekutoni: ", strlen("-Jepni emrin e file te cilin deshironi ta ekzekutoni: "), 0, (sockaddr*)&client, sizeof(sockaddr_in)) == SOCKET_ERROR) {	
                            cout << recvfromError(); return 3;	
                        }	
                        if (filename_len = recvfrom(server_socket, filename, BUFLEN, 0, (sockaddr*)&client, &slen) == SOCKET_ERROR) { cout << recvfromError(); exit(0); }
                            executeFile(filename);	
                        goto menu;	
                        break;	
                    case '9': //exit	
                        goto start;	
                        break;	
                    default:	
                        goto menu;	
                        break;	
                    }	
                }	
            }	
        }

            if (sendto(server_socket, message, strlen(message), 0, (sockaddr*)&client, sizeof(sockaddr_in)) == SOCKET_ERROR) {
                printf("sendto() failed with error code: %d", WSAGetLastError());
                return 3;
            }
            else if (strcmp(message, "2") == 0) {
                userHasAccess = false;
            menu_noaccess:
                char choice[BUFLEN] = {};
                int choice_len;
                char filename[BUFLEN] = {};
                int filename_len;
                char overWriteText[1024 * 8] = {};
                int overWriteText_len;
                string fileStr;
                if (sendto(server_socket, "[1] List files\n[2] Read files\n[9] Exit", strlen("[1] List files\n[2] Read files\n[9] Exit"), 0, (sockaddr*)&client, slen) == SOCKET_ERROR) {
                    cout << recvfromError(); return 3;
                }
                if (choice_len = recvfrom(server_socket, choice, BUFLEN, 0, (sockaddr*)&client, &slen) == SOCKET_ERROR) {
                    cout << recvfromError(); exit(0);
                }
                printf("Choice: %s", choice);
                switch (choice[0])
                {
                case '1': //list files
                    fileStr = fileList();
                    if (sendto(server_socket, fileStr.c_str(), strlen(fileStr.c_str()), 0, (sockaddr*)&client, sizeof(sockaddr_in)) == SOCKET_ERROR)
                    {
                        cout << recvfromError();
                        return 3;
                    }
                    goto menu_noaccess;
                    break;
                case '2': //read files
                    if (sendto(server_socket, "-Jepni emrin e file: ", strlen("-Jepni emrin e file: "), 0, (sockaddr*)&client, sizeof(sockaddr_in)) == SOCKET_ERROR) { cout << recvfromError(); return 3; }

                    if (filename_len = recvfrom(server_socket, filename, BUFLEN, 0, (sockaddr*)&client, &slen) == SOCKET_ERROR) { cout << recvfromError(); exit(0); }

                    fileStr = fileContent(filename);
                    if (sendto(server_socket, fileStr.c_str(), strlen(fileStr.c_str()), 0, (sockaddr*)&client, sizeof(sockaddr_in)) == SOCKET_ERROR) { cout << recvfromError(); return 3; }
                    goto menu_noaccess;
                    break;
                case '9': //exit
                    goto start;
                    //exit(1);
                    break;
                default:
                    goto menu_noaccess;
                    break;
                }

                cout << "\n\n" << (int)choice[0] << endl;
                closesocket(server_socket);
                WSACleanup();
            }
            return 0;
        }
