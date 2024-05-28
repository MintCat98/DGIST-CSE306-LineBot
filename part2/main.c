#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include "definitions.h"

// C++에서 정의된 함수 선언
#ifdef __cplusplus
extern "C" {
#endif
    void detectQRCode(struct QRCodeInfo *qr_info, bool *qr_detected);
#ifdef __cplusplus
}
#endif

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

void* qr_thread(void* arg) {
    struct QRCodeInfo qr_info;
    bool qr_detected = false;

    printf("qr thread started!\n");
    
    while (true) {
        /*
        detectQRCode(&qr_info, &qr_detected);
        
        if (qr_detected) {
            printf("QR Code Detected:\n");
            printf("Current location: (%d, %d)\n", qr_info.x, qr_info.y);
            printf("QR Code Data: %s\n", qr_info.data);
            qr_detected = false; // Reset the flag for the next detection
        }

        usleep(100000); // 0.1초 대기
        */
       sleep(10);
       break;
    }
    
    printf("qr thread ended!\n");
    return NULL;
}

void* server_thread(void* arg) {
    int sock = create_socket();
    DGIST dgist;
    int my_index = 1; // ? 필요한지 아닌지 모름
    int remaining_time = 120;

    // 현재 위치 설정
    Point current = {dgist.players[my_index].row, dgist.players[my_index].col};
    printf("Current location: (%d, %d)\n", current.x, current.y);
    send_action(sock, current.x, current.y, move);

    // 서버로부터 DGIST 구조체 수신
    receive_dgist(sock, &dgist);

    // 다음 목적지 결정
    Point next = find_next_destination(current, dgist.map);
    printf("Next destination: (%d, %d)\n", next.x, next.y);

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

    return NULL;
}

int main() {
    pthread_t qr_tid, server_tid;

    // QR 코드 인식 스레드 시작
    if (pthread_create(&qr_tid, NULL, qr_thread, NULL) != 0) {
        perror("Failed to create QR thread");
        exit(1);
    }

    // 서버 통신 스레드 시작
    if (pthread_create(&server_tid, NULL, server_thread, NULL) != 0) {
        perror("Failed to create server thread");
        exit(1);
    }

    // 스레드 종료 대기
    pthread_join(qr_tid, NULL);
    pthread_join(server_tid, NULL);

    return 0;
}