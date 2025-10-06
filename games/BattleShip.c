#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 10
#define NUM_SHIPS 5

typedef enum { EMPTY, SHIP, HIT, MISS } Cell;

typedef struct {
    int x, y;
    int length;
    int horizontal; // 1 = horizontal, 0 = vertical
    int hits;
} Ship;

void initBoard(Cell board[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            board[i][j] = EMPTY;
}

void printBoard(Cell board[SIZE][SIZE], int hideShips) {
    printf("  ");
    for (int i = 0; i < SIZE; i++) printf("%d ", i);
    printf("\n");
    for (int i = 0; i < SIZE; i++) {
        printf("%d ", i);
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == SHIP && hideShips) printf(". ");
            else if (board[i][j] == SHIP) printf("S ");
            else if (board[i][j] == HIT) printf("X ");
            else if (board[i][j] == MISS) printf("O ");
            else printf(". ");
        }
        printf("\n");
    }
}

int canPlaceShip(Cell board[SIZE][SIZE], Ship ship) {
    int dx = ship.horizontal ? 1 : 0;
    int dy = ship.horizontal ? 0 : 1;
    for (int i = 0; i < ship.length; i++) {
        int nx = ship.x + i * dx;
        int ny = ship.y + i * dy;
        if (nx >= SIZE || ny >= SIZE || board[ny][nx] != EMPTY) return 0;
    }
    return 1;
}

void placeShip(Cell board[SIZE][SIZE], Ship ship) {
    int dx = ship.horizontal ? 1 : 0;
    int dy = ship.horizontal ? 0 : 1;
    for (int i = 0; i < ship.length; i++) {
        board[ship.y + i*dy][ship.x + i*dx] = SHIP;
    }
}

void randomPlaceShips(Cell board[SIZE][SIZE], Ship ships[NUM_SHIPS]) {
    int lengths[NUM_SHIPS] = {5, 4, 3, 3, 2};
    for (int i = 0; i < NUM_SHIPS; i++) {
        Ship ship;
        ship.length = lengths[i];
        ship.hits = 0;
        do {
            ship.horizontal = rand() % 2;
            ship.x = rand() % SIZE;
            ship.y = rand() % SIZE;
        } while (!canPlaceShip(board, ship));
        ships[i] = ship;
        placeShip(board, ship);
    }
}

int takeTurn(Cell board[SIZE][SIZE], Ship ships[NUM_SHIPS]) {
    int x, y;
    printf("Enter coordinates to attack (x y): ");
    scanf("%d %d", &x, &y);
    if (x < 0 || x >= SIZE || y < 0 || y >= SIZE) {
        printf("Invalid coordinates!\n");
        return 0;
    }
    if (board[y][x] == SHIP) {
        board[y][x] = HIT;
        for (int i = 0; i < NUM_SHIPS; i++) {
            Ship *s = &ships[i];
            int dx = s->horizontal ? 1 : 0;
            int dy = s->horizontal ? 0 : 1;
            for (int j = 0; j < s->length; j++) {
                if (s->x + j*dx == x && s->y + j*dy == y) {
                    s->hits++;
                    if (s->hits == s->length) {
                        printf("🎯 You sunk a ship of length %d!\n", s->length);
                    } else {
                        printf("Hit!\n");
                    }
                    break;
                }
            }
        }
        return 1;
    } else if (board[y][x] == EMPTY) {
        board[y][x] = MISS;
        printf("Miss!\n");
        return 0;
    } else {
        printf("Already targeted!\n");
        return 0;
    }
}

int allShipsSunk(Ship ships[NUM_SHIPS]) {
    for (int i = 0; i < NUM_SHIPS; i++)
        if (ships[i].hits < ships[i].length) return 0;
    return 1;
}

void computerTurn(Cell board[SIZE][SIZE], Ship ships[NUM_SHIPS]) {
    int x, y;
    do {
        x = rand() % SIZE;
        y = rand() % SIZE;
    } while (board[y][x] == HIT || board[y][x] == MISS);

    printf("Computer attacks (%d,%d): ", x, y);
    if (board[y][x] == SHIP) {
        board[y][x] = HIT;
        printf("Hit!\n");
        for (int i = 0; i < NUM_SHIPS; i++) {
            Ship *s = &ships[i];
            int dx = s->horizontal ? 1 : 0;
            int dy = s->horizontal ? 0 : 1;
            for (int j = 0; j < s->length; j++) {
                if (s->x + j*dx == x && s->y + j*dy == y) {
                    s->hits++;
                    if (s->hits == s->length) printf("Computer sunk your ship of length %d!\n", s->length);
                    break;
                }
            }
        }
    } else {
        board[y][x] = MISS;
        printf("Miss!\n");
    }
}

int main() {
    srand(time(0));

    Cell playerBoard[SIZE][SIZE], computerBoard[SIZE][SIZE];
    Ship playerShips[NUM_SHIPS], computerShips[NUM_SHIPS];

    initBoard(playerBoard);
    initBoard(computerBoard);

    printf("Placing your ships randomly...\n");
    randomPlaceShips(playerBoard, playerShips);

    printf("Placing computer ships...\n");
    randomPlaceShips(computerBoard, computerShips);

    while (1) {
        printf("\nYour Board:\n");
        printBoard(playerBoard, 0);
        printf("\nComputer Board:\n");
        printBoard(computerBoard, 1);

        // Player turn
        takeTurn(computerBoard, computerShips);
        if (allShipsSunk(computerShips)) {
            printf("🏆 You win!\n");
            break;
        }

        // Computer turn
        computerTurn(playerBoard, playerShips);
        if (allShipsSunk(playerShips)) {
            printf("💀 Computer wins!\n");
            break;
        }
    }

    return 0;
}
