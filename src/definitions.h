#ifndef BPLUSTREE_DEFINITIONS_H
#define BPLUSTREE_DEFINITIONS_H

#include <cstdlib>

#define VERSION "2.0.2"

const size_t DEFAULT_ORDER{ 4 };

// Minimum order is necessarily 3.  We set the maximum
// order arbitrarily.  You may change the maximum order.
const size_t MIN_ORDER{ DEFAULT_ORDER - 1 };
const size_t MAX_ORDER{ 20 };

using Key = int64_t;
using ValueType = int64_t;

#endif


