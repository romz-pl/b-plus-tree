#ifndef ROMZ_BTREE_EMPTY_BASE_HANDLE_H
#define ROMZ_BTREE_EMPTY_BASE_HANDLE_H


namespace btree
{

//
// A helper class to get the empty base class optimization for 0-size
// allocators. Base is internal_allocator_type.
// (e.g. empty_base_handle<internal_allocator_type, node_type*>). If Base is
// 0-size, the compiler doesn't have to reserve any space for it and
// sizeof(empty_base_handle) will simply be sizeof(Data). Google [empty base
// class optimization] for more details.
//
template< typename Base, typename Data >
struct empty_base_handle : public Base
{
    empty_base_handle( const Base &b, const Data &d )
        : Base( b )
        , data( d )
    {
    }

    Data data;
};

}

#endif
