#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ROW 5
#define COL 5

void parse_map(const char *input, int map[ROW][COL]) {
    // 초기화
    memset(map, 0, sizeof(int) * ROW * COL);

    // 맵 데이터 추출
    const char *start = strstr(input, "==========PRINT MAP==========");
    if (!start) {
        printf("Invalid map data\n");
        return;
    }

    while (start) {
        start += strlen("==========PRINT MAP==========\n");
        const char *end = strstr(start, "==========PRINT DONE==========");
        if (!end) {
            printf("Invalid map data\n");
            return;
        }

        // 한 줄씩 읽기
        char line[COL * 2];  // 공백 포함하여 한 줄 최대 크기
        int row = 0;

        while (start < end && row < ROW) {
            // 한 줄 읽기
            int i = 0;
            while (*start != '\n' && start < end && i < sizeof(line) - 1) {
                line[i++] = *start++;
            }
            line[i] = '\0';
            start++;  // 다음 줄로 이동

            // 빈 줄 건너뛰기
            if (strlen(line) == 0) {
                continue;
            }

            printf("line: %s\n", line);  // 디버깅용 출력

            // 라인에서 맵 데이터 추출
            int col = 0;
            for (int j = 0; j < strlen(line) && col < COL; j++) {
                if (line[j] == ' ') {
                    continue;
                } else if (line[j] == '-') {
                    map[row][col] = 0;  // 빈 공간
                } else {
                    map[row][col] = line[j] - '0';  // 숫자 변환
                }
                col++;
            }
            row++;
        }

        start = strstr(start, "==========PRINT MAP==========");
    }
}

void print_map(int map[ROW][COL]) {
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            if (map[i][j] == 0) {
                printf("- ");
            } else {
                printf("%d ", map[i][j]);
            }
        }
        printf("\n");
    }
}

int main() {
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

    return 0;
}
