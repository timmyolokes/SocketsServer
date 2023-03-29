#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>

#pragma comment(lib, "ws2_32.lib")  // link with Winsock library

#define PACKETSIZE 1024


int main() {
    WSADATA wsa;
    SOCKET s;
    struct sockaddr_in server;
    int i = 0;
    int fileLen = 0;
    FILE* writefp;
    //char buffer[1024], packet[1024];
    char buffer[PACKETSIZE];
    char packet[PACKETSIZE + 1];
    int slen = sizeof(server);
    int recv_len;
    struct sockaddr_in si_other;



    printf("\n****** INITIALIZING WINSOCK ***********");
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Failed. Error Code : %d", WSAGetLastError());
        return 1;
    }
    else printf("\nWINSOCK INITIALIZED");

    /***** CREATE SERVER SOCKET ****/
    if ((s = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET) {
        printf("Could not create socket : %d", WSAGetLastError());
    }
    else printf("\nUDP SERVER SOCKET CREATED");

    server.sin_addr.s_addr = inet_addr("127.0.0.1"); // or INADDR_ANY
    server.sin_family = AF_INET;
    server.sin_port = htons(80);
    /***** BIND SOCKET ****/
    if (bind(s, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
        printf("Bind failed with error code : %d", WSAGetLastError());
        exit(EXIT_FAILURE);
    }
    puts("\nSERVER SOCKET BIND SUCCESS");

    /***** WAIT FOR DATA ****/
    printf("\nWaiting for data...");

    //while (waiting for data) {
    while (1) {
        fflush(stdout);
        memset(packet, '\0', PACKETSIZE); //clear buffer of previously received data
        /******** RECEIVE DATA PACKET - blocking *************/
        if ((recv_len = recvfrom(s, packet, PACKETSIZE + 1, 0, (struct sockaddr*)&si_other, &slen)) == SOCKET_ERROR)
        {
            printf("recvfrom() failed with error code : %d", WSAGetLastError());
            exit(EXIT_FAILURE);
        }
        else printf("\nSERVER Received packet IPaddr %s Port %d, seq no = %d: ", inet_ntoa(si_other.sin_addr),
            ntohs(si_other.sin_port), packet[PACKETSIZE]);


        while (i < fileLen) {
            buffer[i] = packet[i];
            i += 1;
        }// while(byteCount < fileLen)

        /*********** write contents of buffer to a file ************/
        writefp = fopen("test2.jpg", "wb");
        if (writefp == NULL) {
            printf("\nError Opening Image-write");
            fclose(writefp);
            exit(0);
        } else printf("\nfile opened for writing");
        fwrite(buffer, fileLen, 1, writefp);
        fclose(writefp);
        printf("\nc SAVED image, press any key");
    }

    closesocket(s);
    WSACleanup();
}