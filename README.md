[![Build Status](https://travis-ci.org/romz-pl/b-plus-tree.svg?branch=master)](https://travis-ci.org/romz-pl/b-plus-tree)


# Memory based B+ tree in C++ 

## Definition of the B-tree

Let _h >= 0_ be an integer, and  _k_ a natural number. A directed tree _T_
is in the class _(k, h)_ of B-trees if _T_ is either empty (i.e. h=0) or has the following
properties:

* Each path from the root to any leaf has the same length _h_, also called the
    height of _T_, i.e. _h_ is eequal to the number of nodes in path.

* Each node except the root and the leaves has at least _(k + 1)_ sons.
    The root is a leaf or has at least two sons.

* Each node has at most _(2k + 1)_ sons. 


## Properties of the B-tree

* Each page holds between _k_ and _2k_ keys (index elements) except the root
    page which may hold between _1_ and _2k_ keys.

* Let the number of keys on a page _P_, which is not a leaf, be _L_.
    Then _P_ has _L+1_ sons.

* Within each page _P_ the keys are sequential in increasing order:
    _x<sub>1</sub>, x<sub>2</sub>, ... x<sub>L</sub>_, where _k <= L <= 2k_ 
    except for the root page for which _1 <= L <= 2k_.

* Furthermore, _P_ contains _(L + 1)_ pointers _P<sub>0</sub>, P<sub>1</sub> ... P<sub>L</sub>_ to the sons of _P_.
    On leaf pages these pointers are undefined (i.e. null pointers).

* Let _P(p<sub>i</sub>)_ be the page to which _P<sub>i</sub>_ points, let _K(P<sub>i</sub>)_ be the set of keys on
    the pages of that maximal subtree of which _P(P<sub>i</sub>)_ is the root. Then for the B-trees
    considered here the following conditions shall always hold:
    1) for each y in K(p<sub>0</sub>)   y < x<sub>i</sub>
    2) for each y in K(p<sub>i</sub>)  x<sub>i</sub> < y < x<sub>i+1</sub>  for i = 1,2,...,L-1
    3) for each y in K(p<sub>L</sub>)  x<sub>L</sub> < y



## References, books

* A. Silberschatz, H.F. Korth, S. Sudarshan, _"Database System Concepts"_, 6th ed. (McGraw Hill, 2011)

* R. Ramakrishnan, J. Gehrke, _"Database Management Systems "_, 3rd ed. (McGraw Hill, 2003)

* A. Drozdek, D.L. Simon _"Data Structures in C"_, (PWS, 1995)

## References, articles

* R. Bayer E.M. McCreight, _"Organization and maintenance of large ordered indexes"_, Acta Informatica, Volume 1, Issue 3, pp. 173–189, Year 1972

* R. Bayer, _"Symmetric binary B-Trees: Data structure and maintenance algorithms"_, Acta Informatica, Volume 1, Issue 4, pp. 290–306, Year 1972

* R. Bayer, K. Unterauer, _"Prefix B-trees"_, ACM Transactions on Database Systems, Volume 2, Issue 1, pp. 11-26, Year 1977

* D. Comer, _"Ubiquitous B-Tree"_, ACM Computing Surveys, Volume 11, Issue 2, Pages 121-137, Year 1979 

* J. Zhou _"A B+-tree Index fot the Know-It-All Database Framework"_, 2003, 
https://spectrum.library.concordia.ca/2344/1/MQ83927.pdf


## Similar projects in C++: 

* cpp-btree: https://code.google.com/archive/p/cpp-btree/

* STX: http://panthema.net/2007/stx-btree/

* From Wikipedia: http://www.amittai.com/prose/bplustree_cpp.html (**Caution!** The code has the bug in deletion procedure!)


### Author
* Zbigniew Romanowski
* e-mail: romz@wp.pl


