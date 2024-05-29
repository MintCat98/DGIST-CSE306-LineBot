#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "definitions.h"

int create_socket(const char *server_ip, int server_port) {
    int sock;
    struct sockaddr_in server_addr;

    // 소켓 생성
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // 서버 주소 설정
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    server_addr.sin_addr.s_addr = inet_addr(server_ip);

    // 서버에 연결
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect");
        close(sock);
        exit(EXIT_FAILURE);
    }

    return sock;
}

void send_action(int sock, int x, int y, enum Action action) {
    ClientAction client_action;
    client_action.row = x;
    client_action.col = y;
    client_action.action = action;

    if (send(sock, &client_action, sizeof(client_action), 0) == -1) {
        perror("send");
        close(sock);
        exit(1);
    }
}

void receive_dgist(int sock, DGIST* dgist) {
    if (recv(sock, dgist, sizeof(DGIST), 0) == -1) {
        perror("recv");
        close(sock);
        exit(1);
    }
}