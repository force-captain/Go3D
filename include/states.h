#pragma once

#include <cstdint>
#include "types.h"

class Board;

uint64_t getHash(Board* b, Colour c);
