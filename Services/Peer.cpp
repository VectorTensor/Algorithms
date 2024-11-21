#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib,"Ws2_32.lib")

void ServerCode(int p){

    std::cout<<"Server function \n";

    WSADATA wsaData;
    SOCKET serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;

    int clientAddrLen = sizeof(clientAddr);
    char buffer[1024] = {0};


    if(WSAStartup(MAKEWORD(2,2), &wsaData) != 0){
        std::cerr <<"WSAStartup failed \n";
        return ;
    }

    // Create server socket

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (serverSocket == INVALID_SOCKET){

        std:: cerr <<"Socket creation failed \n";
        WSACleanup();

        return;

    }

    //Set up the server address structure

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(p);


    // Bind the socket

    if (bind(serverSocket,(struct sockaddr * )&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR){

        std::cerr <<"Bind failed \n";
        closesocket(serverSocket);
        WSACleanup();
        return ;

    }

    // Listen for incomming connections

    if (listen(serverSocket,3 ) == SOCKET_ERROR){
        std::cerr <<"Listen failed \n";

        closesocket(serverSocket);
        WSACleanup();
        return;
    }

    std:: cout << "Server Listening on port "<< p<<std::endl;

    // Accept an incomming connection
    clientSocket = accept(serverSocket,(struct  sockaddr *)&clientAddr,&clientAddrLen);
    if (clientSocket == INVALID_SOCKET){
        std::cerr<< "Accept failed \n";
        closesocket(serverSocket);
        WSACleanup();
        return;
    }

    recv(clientSocket, buffer,1024,0);
    std::cout <<"Message received: " <<buffer<< std::endl;

    const char *response = "Hello from server";
    send(clientSocket, response, strlen(response),0);

    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();


}

//#define SERVER_IP "127.0.0.1"

void ClientCode(char* server_ip, int p){

    std::cout<<"Client function \n";

    WSADATA wsaData;
    SOCKET clientSocket = 0;
    struct sockaddr_in serverAddr;
    char buffer[1024] = {0};

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0 ){
        std::cerr << "WSAStartup failed \n";
        WSACleanup();
        return;
    }

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET){

        std::cerr <<"Socket creation failed \n";
        WSACleanup();
        return ;

    }


    // Set up the server address structure
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(p);
    inet_pton(AF_INET, server_ip, &serverAddr.sin_addr);

    // Connect to the server

    if(connect(clientSocket,(struct sockaddr *) &serverAddr, sizeof(serverAddr)) == SOCKET_ERROR){

        std::cerr << "Connection failed \n";
        closesocket(clientSocket);
        WSACleanup();
        return;
    }

    const char *message = "Hello from client";

    send(clientSocket, message, strlen(message),0);

    recv(clientSocket, buffer, 1024,0);
    std::cout << "Message from server: " << buffer<< std::endl;

    closesocket(clientSocket);
    WSACleanup();

}