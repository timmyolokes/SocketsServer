#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>

#pragma comment(lib, "ws2_32.lib")

#define PACKET_SIZE 1024

typedef struct {
    int seqNum;
    char data[PACKET_SIZE];
} Packet;

int main() {
    WSADATA wsa;
    SOCKET s;
    struct sockaddr_in server, si_other;
    int slen = sizeof(si_other);

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
    else printf("\nUDP SERVER SOCKET CREATED.");

    /***** INITIALIZE SOCKET STRUCT - Server ****/
    server.sin_addr.s_addr = inet_addr("127.0.0.1"); //INADDR_ANY;
    server.sin_family = AF_INET;
    server.sin_port = htons(80);

    /***** BIND SOCKET ****/
    if (bind(s, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
        printf("Bind failed with error code : %d", WSAGetLastError());
        exit(EXIT_FAILURE);
    }
    printf("\nSERVER SOCKET BIND SUCCESS");

    /***** RECEIVE DATA PACKETS ****/
    printf("\nWaiting for data...");

    FILE* writefp = fopen("test2.jpg", "wb");
    if (writefp == NULL) {
        printf("Error Opening Image-write");
        exit(EXIT_FAILURE);
    }

    int expected_sequence_number = 0;

    while (1) {
        Packet recvPacket;
        int recv_len;
        if ((recv_len = recvfrom(s, (char*)&recvPacket, sizeof(recvPacket), 0, (struct sockaddr*)&si_other, &slen)) == SOCKET_ERROR) {
            printf("recvfrom() failed with error code : %d", WSAGetLastError());
            exit(EXIT_FAILURE);
        }
        else {
            printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
            printf("Packet sequence number: %d\n", recvPacket.seqNum);

            if (recvPacket.seqNum == expected_sequence_number) {
                fwrite(recvPacket.data, 1, recv_len - sizeof(int), writefp);
                expected_sequence_number++;
            }
            else {
                printf("Unexpected packet sequence number, expecting %d\n", expected_sequence_number);
            }

            // Send ACK for received packet
            Packet ackPacket;
            ackPacket.seqNum = recvPacket.seqNum;
            if (sendto(s, (char*)&ackPacket, sizeof(int), 0, (struct sockaddr*)&si_other, slen) == SOCKET_ERROR) {
                printf("sendto() failed with error code : %d", WSAGetLastError());
                exit(EXIT_FAILURE);
            }
            printf("ACK %d sent\n", recvPacket.seqNum);
        }
    }

    // Cleanup
    fclose(writefp);
    closesocket(s);
    WSACleanup();

    return 0;
}
