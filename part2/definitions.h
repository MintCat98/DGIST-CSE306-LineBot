#include <stdio.h>
#include <pthread.h>

#define ROW 5
#define COL 5
#define CLIENTS 2

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 12345

struct QRCodeInfo {
    int x;
    int y;
    char data[128];
};

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

int create_socket();
void send_action(int sock, int x, int y, enum Action action);
void receive_dgist(int sock, DGIST* dgist);
void detectQRCode(struct QRCodeInfo *qr_info, int *qr_detected);

extern DGIST global_dgist;
extern pthread_mutex_t dgist_mutex;
extern Point current;
extern Point next;
