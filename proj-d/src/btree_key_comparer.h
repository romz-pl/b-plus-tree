#ifndef ROMZ_BTREE_KEY_COMPARER_H
#define ROMZ_BTREE_KEY_COMPARER_H


namespace btree
{

//
// A helper class that allows a compare-to functor to behave like a plain
// compare functor. This specialization is used when we do not have a
// compare-to functor.
//
template< typename Key, typename Compare, bool HaveCompareTo >
struct btree_key_comparer
{
    btree_key_comparer() { }
    btree_key_comparer( Compare c ) : comp( c ) { }

    static bool bool_compare( const Compare &comp, const Key &x, const Key &y )
    {
        return comp( x, y );
    }

    bool operator()( const Key &x, const Key &y ) const
    {
        return bool_compare( comp, x, y );
    }

    Compare comp;
};

//
// A specialization of btree_key_comparer when a compare-to functor is
// present. We need a plain (boolean) comparison in some parts of the btree
// code, such as insert-with-hint.
//
template< typename Key, typename Compare >
struct btree_key_comparer< Key, Compare, true >
{
    btree_key_comparer() { }
    btree_key_comparer( Compare c ) : comp( c ) { }

    static bool bool_compare( const Compare &comp, const Key &x, const Key &y )
    {
        return comp(x, y) < 0;
    }

    bool operator()( const Key &x, const Key &y ) const
    {
        return bool_compare( comp, x, y );
    }

    Compare comp;
};

}

#endif

