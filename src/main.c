#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <time.h>
#include <math.h>
#include <wiringPi.h>
#include "car_control.h"
#include "car_tracking.h"
#include "definitions.h"

// C++에서 정의된 함수 선언
#ifdef __cplusplus
extern "C" {
#endif
    //void detectQRCode(struct QRCodeInfo *qr_info, bool *qr_detected);
#ifdef __cplusplus
}
#endif

// 글로벌 변수와 뮤텍스 정의
DGIST global_dgist;
int sock;
pthread_mutex_t dgist_mutex = PTHREAD_MUTEX_INITIALIZER;
Robot robot;
Point previous;

int COMMAND;
int nQR;
int myIndex;
int algorithm;

// 디버깅용
void print_received_map(Node map[ROW][COL]) {
    for (int i = ROW-1; i >= 0; i--) {
        for (int j = 0; j < COL; j++) {
            int status = map[j][i].item.status;
            if (status == 1) {
                printf("%d ", map[j][i].item.score);
            } else if (status == 0) {
                printf("- ");
            } else if (status == 2) {
                printf("x ");
            }
        }
        printf("\n");
    }
    printf("\n");
}

//디버깅용
void directionPrint() {
    if (robot.direction == 0) {
        printf("robot.direction: 동\n");
    } else if (robot.direction == 1) {
        printf("robot.direction: 서\n");
    } else if (robot.direction == 2) {
        printf("robot.direction: 남\n");
    } else if (robot.direction == 3) {
        printf("robot.direction: 북\n");
    }
}

void printCommand(int cmd) {
    if (cmd == 1) printf("직진\n");
    else if (cmd == 2) printf("좌회전\n");
    else if (cmd == 3) printf("우회전\n");
    return;
}

Point find_next_destination(Node map[ROW][COL]) {
    int directions[3][2];

    // 점수가 같을 때 우선 순위: 앞, 오른쪽, 왼쪽
    switch (robot.direction) {
        case NORTH:
            directions[0][0] = 0; directions[0][1] = 1; // 앞으로
            directions[1][0] = 1;  directions[1][1] = 0; // 오른쪽
            directions[2][0] = -1;  directions[2][1] = 0; // 왼쪽
            break;
        case SOUTH:
            directions[0][0] = 0;  directions[0][1] = -1; // 앞으로
            directions[1][0] = -1; directions[1][1] = 0; // 오른쪽
            directions[2][0] = 1;  directions[2][1] = 0; // 왼쪽
            break;
        case EAST:
            directions[0][0] = 1;  directions[0][1] = 0; // 앞으로
            directions[1][0] = 0;  directions[1][1] = -1; // 오른쪽
            directions[2][0] = 0;  directions[2][1] = 1; // 왼쪽
            break;
        case WEST:
            directions[0][0] = -1;  directions[0][1] = 0; // 앞으로
            directions[1][0] = 0;  directions[1][1] = 1; // 오른쪽
            directions[2][0] = 0; directions[2][1] = -1; // 왼쪽
            break;
    }

    Point best_point = {robot.x, robot.y};
    int best_score = -9; // TODO: 폭탄의 최저 점수여야 함.

    //Manual movement (4번째 QR을 인식하기 전까지는 앞으로만 가게 함 => 폭탄 (1,0)(3,0) 또는 (1,4),(3,4)에 설치 가능)
    /*
    if (nQR < 4) {
        best_point.x = robot.x + directions[0][0];
        best_point.y = robot.y + directions[0][1];
        return best_point;
    }
    */

    for (int i = 0; i < 3; i++) {
        int new_x = robot.x + directions[i][0];
        int new_y = robot.y + directions[i][1];

        if (new_x >= 0 && new_x < ROW && new_y >= 0 && new_y < COL) {
            int score;
            int status = map[new_x][new_y].item.status;
            if (status == 1) {
                score = map[new_x][new_y].item.score;
            } else if (status == 0) {
                score = 0;
            } else if (status == 2) {
                score = -2;
            }

            if (score > best_score) {
                best_score = score;
                best_point.x = new_x;
                best_point.y = new_y;
            }
        }
        printf("\n");
    }
    return best_point;
}

int (*findWays(Direction direction))[2] {
    static int ways[3][2];

    if (direction == NORTH) {
        ways[0][0] = 0; ways[0][1] = 1; // 앞으로
        ways[1][0] = 1; ways[1][1] = 0; // 오른쪽
        ways[2][0] = -1; ways[2][1] = 0; // 왼쪽
    } else if (direction == SOUTH) {
        ways[0][0] = 0; ways[0][1] = -1; // 앞으로
        ways[1][0] = -1; ways[1][1] = 0; // 오른쪽
        ways[2][0] = 1; ways[2][1] = 0; // 왼쪽
    } else if (direction == EAST) {
        ways[0][0] = 1; ways[0][1] = 0; // 앞으로
        ways[1][0] = 0; ways[1][1] = -1; // 오른쪽
        ways[2][0] = 0; ways[2][1] = 1; // 왼쪽
    } else if (direction == WEST) {
        ways[0][0] = -1; ways[0][1] = 0; // 앞으로
        ways[1][0] = 0; ways[1][1] = 1; // 오른쪽
        ways[2][0] = 0; ways[2][1] = -1; // 왼쪽
    }

    return ways;
}

Direction update_direction(int action, Direction direction) {
    int currDirection = direction;

    if (action == 2) { // 왼쪽으로 회전
        if (currDirection == NORTH) {
            return WEST;
        } else if (currDirection == SOUTH) {
            return EAST;
        } else if (currDirection == EAST) {
            return NORTH;
        } else if (currDirection == WEST) {
            return SOUTH;
        }
    } else if (action == 3) { // 오른쪽으로 회전
        if (currDirection == NORTH) {
            return EAST;
        } else if (currDirection == SOUTH) {
            return WEST;
        } else if (currDirection == EAST) {
            return SOUTH;
        } else if (currDirection == WEST) {
            return NORTH;
        }
    } else {
        return currDirection;
    }
}

Direction set_direction(int x, int y) {
    Direction newDir;

    if (x > previous.x) {
        newDir = EAST;
    } else if (x < previous.x) {
        newDir = WEST;
    } else if (y > previous.y) {
        newDir = NORTH;
    } else if (y < previous.y) {
        newDir = SOUTH;
    }

    // 이전 위치를 현재 위치로 업데이트
    previous.x = x;
    previous.y = y;

    return newDir;
}

int distance(int x, int y, int new_x, int new_y) {
    return abs(new_x - x) + abs(new_y - y);
}

Point find_next_destination2(Node map[ROW][COL]) {
    Point best_point = {robot.x, robot.y};
    int best_score = -9;

    int (*directions1)[2] = findWays(robot.direction);

    for (int i = 0; i < 3; i++) {
        int new_x = robot.x + directions1[i][0];
        int new_y = robot.y + directions1[i][1];

        if (new_x >= 0 && new_x < ROW && new_y >= 0 && new_y < COL) {
            int score1 = 0;
            int status = map[new_x][new_y].item.status;
            if (status == 1) {
                score1 = map[new_x][new_y].item.score;
            } else if (status == 2) {
                score1 = -8;
            }

            Direction newDirection = update_direction(i+1, robot.direction);
            int (*directions2)[2] = findWays(newDirection);

            for (int j = 0; j < 3; j++) {
                int new_x2 = new_x + directions2[j][0];
                int new_y2 = new_y + directions2[j][1];

                if (new_x2 >= 0 && new_x2 < ROW && new_y2 >= 0 && new_y2 < COL) {
                    // 적이 나보다 해당 포인트에 먼저 도착하는 경우 제외
                    int oppDistance = distance(global_dgist.players[2].col, global_dgist.players[2].row, new_x2, new_y2);
                    if (oppDistance <= 2) continue;

                    int status2 = map[new_x2][new_y2].item.status;
                    int score2 = 0;
                    if (status2 == 1) {
                        score2 = map[new_x2][new_y2].item.score;
                    } else if (status2 == 2) {
                        score2 == -8;
                    }

                    int total_score = score1 + score2;

                    if (total_score > best_score) {
                        best_score = total_score;
                        best_point.x = new_x;
                        best_point.y = new_y;
                    }
                }
            }

            if (score1 > best_score) {
                best_score = score1;
                best_point.x = new_x;
                best_point.y = new_y;
            }
        }
    }
    return best_point;
}

// 로봇의 이동 명령을 결정하는 함수
int decide_movement(Point destination) {
    // 목적지가 로봇의 현재 위치에 상대적으로 어디에 있는지 계산
    int dx = destination.x - robot.x; // 동서 방향
    int dy = destination.y - robot.y; // 남북 방향
    int command;

    Direction currDirection = robot.direction;

    if (currDirection == NORTH) {
        if (dy > 0 && dx == 0) return 1;
        if (dx < 0 && dy == 0) return 2; 
        if (dx > 0 && dy == 0) return 3; 
    } else if (currDirection == SOUTH) {
        if (dy < 0 && dx == 0) return 1; 
        if (dx > 0 && dy == 0) return 2; 
        if (dx < 0 && dy == 0) return 3; 
    } else if (currDirection == EAST) {
        if (dx > 0 && dy == 0) return 1;
        if (dy < 0 && dx == 0) return 2;
        if (dy > 0 && dx == 0) return 3; 
    } else if (currDirection == WEST) {
        if (dx < 0 && dy == 0) return 1; 
        if (dy > 0 && dx == 0) return 2; 
        if (dy < 0 && dx == 0) return 3; 
    } else {
        return 0;
    }
}

int should_place_bomb(DGIST* dgist) {
    // 첫 QR 인식 후 90초가 지난 시점부터 폭탄 설치
    static time_t start_time = 0;
    static int counting = 0;
    
    if (nQR > 0) {
        if (!counting) {
            start_time = time(NULL);
            counting = 1;
        }
        else {
            time_t current_time = time(NULL);
            double elapsed_time = difftime(current_time, start_time);

            if (elapsed_time >= 90) return 1; 
        }
    }

    // (1,0), (3,0), (1,4), (3,4)일 경우에 폭탄 설치
    if ((robot.x == 1 && robot.y == 0) ||
        (robot.x == 3 && robot.y == 0) ||
        (robot.x == 1 && robot.y == 4) ||
        (robot.x == 3 && robot.y == 4)) {
        return 1;
    }

    // 적과의 거리가 2 이하일 경우에 폭탄 설치
    /*
    int opponent_index = (my_index == 0) ? 1 : 0;
    int opponent_x = dgist->players[opponent_index].col;
    int opponent_y = dgist->players[opponent_index].row;

    if (sqrt(pow(robot.x - opponent_x, 2) + pow(robot.y - opponent_y, 2)) <= 2) {
        return 1;
    }
    */

    return 0;
}

void* qr_thread(void* arg) {
    struct QRCodeInfo qr_info;
    int qr_detected = 0;
    nQR = 0;
    time_t start_time = *(time_t*)arg;

    camSetup();

    //디버깅용
    printf("초기 ");
    directionPrint();

    while (1) {
        detectQRCode(&qr_info, &qr_detected);
        
        if (qr_detected) {

            if (qr_info.x == previous.x && qr_info.y == previous.y) {
                printf("중복 qr detected (%d, %d)\n", qr_info.x, qr_info.y);
                qr_detected = 0;
                continue;
            }

            nQR += 1;
            robot.x = qr_info.x;
            robot.y = qr_info.y;
            printf("Current location: (%d, %d)\n", robot.x, robot.y);
            
            printf("direction update 전: ");
            directionPrint();
            robot.direction = set_direction(robot.x, robot.y);
            printf("direction update 후: ");
            directionPrint();

            qr_detected = 0; 

            // 뮤텍스를 사용하여 글로벌 데이터 접근
            pthread_mutex_lock(&dgist_mutex);

            // 폭탄 설치 여부 결정
            if (should_place_bomb(&global_dgist)) {
                printf("Place a bomb at (%d, %d)\n", robot.x, robot.y);
                send_action(sock, robot.x, robot.y, setBomb);
            } else {
                printf("Do not place a bomb at (%d, %d)\n", robot.x, robot.y);
                send_action(sock, robot.x, robot.y, move);
            }

            // 다음 목적지 선택
            if (algorithm == 1) {
                Point next = find_next_destination(global_dgist.map);
            } else if (algorithm == 2) {
                Point next = find_next_destination2(global_dgist.map);
            }
            
            printf("Next destination: (%d, %d)\n", next.x, next.y);
            int cmd = decide_movement(next);
            COMMAND = cmd;
            printCommand(COMMAND);
            
            pthread_mutex_unlock(&dgist_mutex);
        }

        usleep(100000); // 0.1초 대기
    }

    return NULL;
}

void* server_thread(void* arg) {
    DGIST dgist;

    while (1) { // 변경된 부분
        // 서버로부터 DGIST 구조체 수신
        receive_dgist(sock, &dgist);
        
        // dgist.players[index] index 확인
        if (nQR == 1) {
            myIndex = (dgist.players[0].row == robot.y) ? 0 : 1;
            printf("myIndex: %d\n", myIndex);
        }

        // 뮤텍스를 사용하여 글로벌 데이터 업데이트
        pthread_mutex_lock(&dgist_mutex);
        global_dgist = dgist;
        print_received_map(global_dgist.map);
        pthread_mutex_unlock(&dgist_mutex);

        usleep(100000); // 0.1초 대기
    }

    return NULL;
}

void* raspbot_thread(void *arg) {
    setup();

    // 초기 command 설정 - 직진
    // COMMAND = 1;

    tracking_function();
}

int main(int argc, char *argv[]) {
    if (argc != 6) {
        fprintf(stderr, "Usage: %s <server_ip> <server_port> <robot index> <initial direction> <algorithm>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *server_ip = argv[1];
    int server_port = atoi(argv[2]);
    int robot_index = atoi(argv[3]); 
    robot.direction = atoi(argv[4]);
    algorithm = atoi(argv[5]);

    pthread_t qr_tid, server_tid, raspbot_tid;
    time_t start_time;
    time(&start_time);

    // 소켓 생성
    sock = create_socket(server_ip, server_port);

    // 시작 위치(robot_index)에 따라 초기 상태 설정
    if (robot_index == 1) {
        robot.x = 0;
        robot.y = 0;
        if (robot.direction == NORTH) {
            previous.x = 0;
            previous.y = -1;
        } else if (robot.direction == EAST) {
            previous.x = -1;
            previous.y = 0;
        } else {
            fprintf(stderr, "Invalid initial direction\n");
            exit(EXIT_FAILURE);
        }
    } else {
        robot.x = 4;
        robot.y = 4;
        if (robot.direction == SOUTH) {
            previous.x = 4;
            previous.y = 5;
        } else if (robot.direction == WEST) {
            previous.x = 5;
            previous.y = 4;
        } else {
            fprintf(stderr, "Invalid initial direction\n");
            exit(EXIT_FAILURE);
        }
    }

    // Raspbot 스레드 시작
    if (pthread_create(&raspbot_tid, NULL, raspbot_thread, NULL) != 0) {
        perror("Failed to create raspbot thread");
        exit(1);
    }
    
    // QR 코드 인식 스레드 시작
    if (pthread_create(&qr_tid, NULL, qr_thread, &start_time) != 0) {
        perror("Failed to create QR thread");
        exit(1);
    }

    // 서버 통신 스레드 시작
    if (pthread_create(&server_tid, NULL, server_thread, NULL) != 0) {
        perror("Failed to create server thread");
        exit(1);
    }

    // 스레드 종료 대기
    pthread_join(raspbot_tid, NULL);
    pthread_join(qr_tid, NULL);
    pthread_join(server_tid, NULL);

    // 메인 함수 종료 시 소켓 닫기
    close(sock);
    printf("Socket closed in main.\n");

    return 0;
}
