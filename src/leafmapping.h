#ifndef BPLUSTREE_LEAFMAPPING_H
#define BPLUSTREE_LEAFMAPPING_H

#include "key.h"
#include "record.h"

class LeafMapping
{
public:
    LeafMapping( const Key& key, Record* record );

public:
    Key m_key;

    Record* m_record;
};

#endif
