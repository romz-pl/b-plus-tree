//
//  Node.hpp
//  BPlusTree.2a
//
//  Created by Amittai Aviram on 6/10/16.
//  Copyright © 2016 Amittai Aviram. All rights reserved.
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//  http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.

#pragma once

#include <string>
#include "definitions.h"

// Key used where only the entry's pointer has meaning.
const KeyType DUMMY_KEY{-1};

// Abstract class.
class Node
{
public:
    explicit Node(size_t aOrder);
    explicit Node(size_t aOrder, Node* aParent);
    virtual ~Node();
    size_t order() const;
    Node* parent() const;
    void setParent(Node* aParent);
    bool isRoot() const;
    virtual bool isLeaf() const = 0;
    virtual size_t size() const = 0;
    virtual size_t minSize() const = 0;
    virtual size_t maxSize() const = 0;
    virtual std::string toString(bool aVerbose = false) const = 0;
private:
    const size_t fOrder;
    Node* fParent;
};
