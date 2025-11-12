#include "states.h"
#include "tile.h"
#include "types.h"
#include "board.h"
#include "group.h"
#include <vector>

static uint64_t state = 88172345361325652ULL;
static uint64_t whiteTurn;
static std::vector<uint64_t> zobristTable[3];
static int size = 0;

static uint64_t xorshift64() {
    uint64_t x = state;
    x ^= x << 13;
    x ^= x >> 7;
    x ^= x << 17;
    state = x;
    return x;
}

static void initTable(int boardSize) {
    for(int i = 0; i < 3; i++) {
        zobristTable[i].resize(boardSize * boardSize);
        for (int j = 0; j < boardSize * boardSize; j++) {
            zobristTable[i][j] = xorshift64();
        }
    }
    whiteTurn = xorshift64();
}

static int getStateIdx(Tile* t) {
    if (t->getColour() == Colour::EMPTY) return 0;
    if (t->getGroup() == nullptr) return 0;
    if (t->getGroup()->isCaptured()) return 0;
    return t->getColour() == Colour::WHITE ? 1 : 2;
}

uint64_t getHash(Board* b, Colour c) {
    int bSize = b->getSize();

    if (bSize != size) {
        size = bSize;
        initTable(size);
    }

    uint64_t result = 0;
    for(int i = 0; i < size; i++) {
        for(int j = 0; j < size; j++) {
            Tile* t = (*b)(i, j);
            result ^= zobristTable[getStateIdx(t)][i * size + j];
        }
    }
    
    if (c == Colour::WHITE) result ^= whiteTurn;

    return result;
}
