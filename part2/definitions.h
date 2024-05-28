#include <stdio.h>

#define ROW 5
#define COL 5
#define CLIENTS 2

#define SERVER_IP "127.0.0.1"


enum Status {
    nothing,
    item,
    trap
};

typedef struct {
    int x;
    int y;
} Point;

typedef struct {
    int socket;
    struct sockaddr_in address;
    int row;
    int col;
    int score;
    int bomb;
} client_info;

typedef struct {
    enum Status status;
    int score;
} Item;

typedef struct {
    int row;
    int col;
    Item item;
} Node;

typedef struct {
    client_info players[CLIENTS];
    Node map[ROW][COL];
} DGIST;

enum Action {
    move,
    setBomb
};

typedef struct {
    int row;
    int col;
    enum Action action;
} ClientAction;