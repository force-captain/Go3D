#pragma once

#include <cstdint>
#include "types.hpp"

class Board;

uint64_t getHash(Board* b, Colour c);
