#pragma once

#include "labster/core/slice.h"
#include <stdint.h>

typedef uint32_t Utf8Char;

Utf8Char utf8Next(Slice slice, size_t *position);
