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

Point find_next_destination(Point current, Node map[ROW][COL]) {
    int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    Point best_point = current;
    int best_score = -1;

    // 거리가 1인 곳 확인
    for (int i = 0; i < 4; i++) {
        int new_x = current.x + directions[i][0];
        int new_y = current.y + directions[i][1];

        if (new_x >= 0 && new_x < ROW && new_y >= 0 && new_y < COL) {
            int score = map[new_x][new_y].item.score;
            if (score > best_score) {
                best_score = score;
                best_point.x = new_x;
                best_point.y = new_y;
            }
        }
    }

    // 거리가 1인 곳에서 점수가 없다면 거리가 2인 곳 확인
    if (best_score <= 0) {
        best_score = -1;  // 초기화
        for (int i = -2; i <= 2; i++) {
            for (int j = -2; j <= 2; j++) {
                if (abs(i) + abs(j) == 2) {
                    int new_x = current.x + i;
                    int new_y = current.y + j;

                    if (new_x >= 0 && new_x < ROW && new_y >= 0 && new_y < COL) {
                        int score = map[new_x][new_y].item.score;
                        if (score > best_score) {
                            best_score = score;
                            best_point.x = new_x;
                            best_point.y = new_y;
                        }
                    }
                }
            }
        }
    }

    return best_point;
}

int should_place_bomb(DGIST* dgist, int my_index, int remaining_time) {
    int my_x = dgist->players[my_index].row;
    int my_y = dgist->players[my_index].col;
    int opponent_index = (my_index == 0) ? 1 : 0;
    int opponent_x = dgist->players[opponent_index].row;
    int opponent_y = dgist->players[opponent_index].col;

    if (abs(my_x - opponent_x) + abs(my_y - opponent_y) <= 2) {
        return 1;
    }

    if (dgist->map[my_x][my_y].item.status == item && dgist->map[my_x][my_y].item.score > 1) {
        return 1;
    }

    if (remaining_time < 30) {
        return 1;
    }

    return 0;
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
    int my_index = 1; // 자신의 인덱스를 설정해야 함 (0 또는 1)
    int remaining_time = 120;

    // 현재 위치 설정
    Point current = {4, 4};
    printf("Current location: (%d, %d)\n", current.x, current.y);
    send_action(sock, current.x, current.y, move);

    // 다음 목적지 결정
    Point next = find_next_destination(current, dgist.map);
    printf("Next destination: (%d, %d)\n", next.x, next.y);

    // 서버로부터 DGIST 구조체 수신
    receive_dgist(sock, &dgist);

    // 맵 출력
    printf("Received Map:\n");
    print_received_map(dgist.map);

    // 폭탄 설치 여부 결정
    if (should_place_bomb(&dgist, my_index, remaining_time)) {
        printf("Place a bomb at (%d, %d)\n", dgist.players[my_index].row, dgist.players[my_index].col);
        send_action(sock, dgist.players[my_index].row, dgist.players[my_index].col, setBomb);
    } else {
        printf("Do not place a bomb at (%d, %d)\n", dgist.players[my_index].row, dgist.players[my_index].col);
        send_action(sock, dgist.players[my_index].row, dgist.players[my_index].col, move);
    }

    close(sock);

    return 0;
}
