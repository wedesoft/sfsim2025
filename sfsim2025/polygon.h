#pragma once
#include <stdbool.h>
#include "coordinate.h"
#include "list.h"


list_t convex_hull(list_t points);

bool inside(list_t points, coordinate_t point);

list_t intersection(list_t a, list_t b);
