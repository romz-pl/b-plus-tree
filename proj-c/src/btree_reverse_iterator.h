#ifndef ROMZ_BTREE_REVERSE_ITERATOR_H
#define ROMZ_BTREE_REVERSE_ITERATOR_H



/// STL-like mutable reverse iterator object for B+ tree items. The
/// iterator points to a specific slot number in a leaf.


template< typename _Key,
          typename _Data,
          typename _Value,
          typename _Compare,
          typename _Traits,
          bool _Duplicates,
          typename _Alloc,
          bool _UsedAsSet >
class btree_iterator;

template< typename _Key,
          typename _Data,
          typename _Value,
          typename _Compare,
          typename _Traits,
          bool _Duplicates,
          typename _Alloc,
          bool _UsedAsSet >
class btree_reverse_iterator
{
public:
    // *** Types

    /// The key type of the btree. Returned by key().
    typedef _Key key_type;

    /// The data type of the btree. Returned by data().
    typedef _Data data_type;

    /// The value type of the btree. Returned by operator*().
    typedef _Value value_type;

    /// The pair type of the btree.
    typedef std::pair<key_type, data_type> pair_type;

    /// Reference to the value_type. STL required.
    typedef value_type& reference;

    /// Pointer to the value_type. STL required.
    typedef value_type* pointer;

    /// STL-magic iterator category
    typedef std::bidirectional_iterator_tag iterator_category;

    /// STL-magic
    typedef ptrdiff_t difference_type;

    /// Size type used to count keys
    typedef size_t size_type;

    typedef btree_leaf_node< _Key, _Data, _Value, _Traits, _Alloc, _UsedAsSet > leaf_node;

    typedef btree_pair_to_value<_Key, _Data, value_type, pair_type> pair_to_value_type;

    static const bool used_as_set = _UsedAsSet;

private:
    // *** Members

    /// The currently referenced leaf node of the tree
    leaf_node * currnode;

    /// One slot past the current key/data slot referenced.
    size_type currslot;

    /// Friendly to the const_iterator, so it may access the two data items
    /// directly
    friend class btree_iterator<_Key, _Data, _Value, _Compare, _Traits, _Duplicates, _Alloc, _UsedAsSet>;

    /// Also friendly to the const_iterator, so it may access the two data
    /// items directly
    friend class const_iterator;

    /// Also friendly to the const_iterator, so it may access the two data
    /// items directly
    friend class const_reverse_iterator;

    /// Evil! A temporary value_type to STL-correctly deliver operator* and
    /// operator->
    mutable value_type temp_value;

    // The macro BTREE_FRIENDS can be used by outside class to access the B+
    // tree internals. This was added for wxBTreeDemo to be able to draw the
    // tree.
    // BTREE_FRIENDS

public:
    // *** Methods

    /// Default-Constructor of a reverse iterator
    inline btree_reverse_iterator()
        : currnode(nullptr), currslot(0)
    { }

    /// Initializing-Constructor of a mutable reverse iterator
    inline btree_reverse_iterator(leaf_node* l, size_type s)
        : currnode(l), currslot(s)
    { }

    /// Copy-constructor from a mutable iterator
    inline btree_reverse_iterator(const btree_iterator< _Key,
                                  _Data,
                                  _Value,
                                  _Compare,
                                  _Traits,
                                  _Duplicates,
                                  _Alloc,
                                  _UsedAsSet>& it) // NOLINT
        : currnode(it.currnode), currslot(it.currslot)
    { }

    /// Dereference the iterator, this is not a value_type& because key and
    /// value are not stored together
    inline reference operator * () const
    {
        BTREE_ASSERT(currslot > 0);
        temp_value = pair_to_value_type()(pair_type(key(), data()));
        return temp_value;
    }

    /// Dereference the iterator. Do not use this if possible, use key()
    /// and data() instead. The B+ tree does not stored key and data
    /// together.
    inline pointer operator -> () const
    {
        BTREE_ASSERT(currslot > 0);
        temp_value = pair_to_value_type()(pair_type(key(), data()));
        return &temp_value;
    }

    /// Key of the current slot
    inline const key_type & key() const
    {
        BTREE_ASSERT(currslot > 0);
        return currnode->slotkey[currslot - 1];
    }

    /// Writable reference to the current data object
    inline data_type & data() const
    {
        BTREE_ASSERT(currslot > 0);
        return currnode->slotdata[used_as_set ? 0 : currslot - 1];
    }

    /// Prefix++ advance the iterator to the next slot
    inline btree_reverse_iterator& operator ++ ()
    {
        if (currslot > 1) {
            --currslot;
        }
        else if (currnode->prevleaf != nullptr) {
            currnode = currnode->prevleaf;
            currslot = currnode->slotuse;
        }
        else {
            // this is begin() == rend()
            currslot = 0;
        }

        return *this;
    }

    /// Postfix++ advance the iterator to the next slot
    inline btree_reverse_iterator operator ++ (int)
    {
        btree_reverse_iterator tmp = *this;   // copy ourselves

        if (currslot > 1) {
            --currslot;
        }
        else if (currnode->prevleaf != nullptr) {
            currnode = currnode->prevleaf;
            currslot = currnode->slotuse;
        }
        else {
            // this is begin() == rend()
            currslot = 0;
        }

        return tmp;
    }

    /// Prefix-- backstep the iterator to the last slot
    inline btree_reverse_iterator& operator -- ()
    {
        if (currslot < currnode->slotuse) {
            ++currslot;
        }
        else if (currnode->nextleaf != nullptr) {
            currnode = currnode->nextleaf;
            currslot = 1;
        }
        else {
            // this is end() == rbegin()
            currslot = currnode->slotuse;
        }

        return *this;
    }

    /// Postfix-- backstep the iterator to the last slot
    inline btree_reverse_iterator operator -- (int)
    {
        btree_reverse_iterator tmp = *this;   // copy ourselves

        if (currslot < currnode->slotuse) {
            ++currslot;
        }
        else if (currnode->nextleaf != nullptr) {
            currnode = currnode->nextleaf;
            currslot = 1;
        }
        else {
            // this is end() == rbegin()
            currslot = currnode->slotuse;
        }

        return tmp;
    }

    /// Equality of iterators
    inline bool operator == (const btree_reverse_iterator& x) const
    {
        return (x.currnode == currnode) && (x.currslot == currslot);
    }

    /// Inequality of iterators
    inline bool operator != (const btree_reverse_iterator& x) const
    {
        return (x.currnode != currnode) || (x.currslot != currslot);
    }
};


#endif
