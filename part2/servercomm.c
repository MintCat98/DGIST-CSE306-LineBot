#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "definitions.h"

#define SERVER_PORT 12345

void parse_map(const char *input, Node map[ROW][COL]) {
    const char *start = strstr(input, "==========PRINT MAP==========");
    if (!start) {
        printf("Invalid map data\n");
        return;
    }
    start += strlen("==========PRINT MAP==========\n");
    const char *end = strstr(start, "==========PRINT DONE==========");
    if (!end) {
        printf("Invalid map data\n");
        return;
    }

    char line[COL * 2];
    int row = 0;

    while (start < end && row < ROW) {
        int i = 0;
        while (*start != '\n' && start < end && i < sizeof(line) - 1) {
            line[i++] = *start++;
        }
        line[i] = '\0';
        start++;  // Move to next line

        if (strlen(line) == 0) {
            continue;
        }

        int col = 0;
        char *token = strtok(line, " ");
        while (token != NULL && col < COL) {
            map[row][col].item.score = atoi(token);
            map[row][col].item.status = (map[row][col].item.score > 0) ? item : nothing;
            token = strtok(NULL, " ");
            col++;
        }
        row++;
    }
}

void print_received_map(Node map[ROW][COL]) {
    for (int i = ROW-1; i >= 0; i--) {
        for (int j = 0; j < COL; j++) {
            if (map[i][j].item.score > 0 && map[i][j].item.score < 5) {
                printf("%d ", map[i][j].item.score);
            }
            else {
                printf("- ");
            }
        }
        printf("\n");
    }
}

int create_socket() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("socket");
        exit(1);
    }
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect");
        close(sock);
        exit(1);
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

int main() {
    int sock = create_socket();
    DGIST dgist;

    // 서버로부터 DGIST 구조체 수신
    receive_dgist(sock, &dgist);

    // 맵 출력
    printf("Received Map:\n");
    print_received_map(dgist.map);

    /*
    // 아래 input을 서버에서 받아온 결과로 바꿔야 함.
    const char *input = "==========PRINT MAP==========\n"
                        "- - - - - \n"
                        "- 2 - - - \n"
                        "- - - - - \n"
                        "- - - - - \n"
                        "- - - 4 - \n"
                        "==========PRINT DONE==========\n";

    int map[ROW][COL];
    parse_map(input, map);

    printf("Parsed Map:\n");
    print_map(map);
    */

    close(sock);

    return 0;
}
