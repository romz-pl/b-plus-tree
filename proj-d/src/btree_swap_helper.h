#ifndef ROMZ_BTREE_SWAP_HELPER_H
#define ROMZ_BTREE_SWAP_HELPER_H

#include <utility>

namespace btree
{

//
// Inside a btree method, if we just call swap(), it will choose the
// btree::swap method, which we don't want. And we can't say ::swap
// because then MSVC won't pickup any std::swap() implementations. We
// can't just use std::swap() directly because then we don't get the
// specialization for types outside the std namespace. So the solution
// is to have a special swap helper function whose name doesn't
// collide with other swap functions defined by the btree classes.
//
template< typename T >
inline void btree_swap_helper( T &a, T &b )
{
    using std::swap;
    swap( a, b );
}

}

#endif
