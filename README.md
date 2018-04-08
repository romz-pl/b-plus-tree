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

* R. Bayer E.M. McCreight, _"Organization and maintenance of large ordered indexes"_, Acta Informatica, Volume 1, Issue 3, pp. 173–189, Year 1972, [PDF](http://www.inf.fu-berlin.de/lehre/SS10/DBS-Intro/Reader/BayerBTree-72.pdf)

* R. Bayer, _"Symmetric binary B-Trees: Data structure and maintenance algorithms"_, Acta Informatica, Volume 1, Issue 4, pp. 290–306, Year 1972, [Abstract](https://link.springer.com/article/10.1007/BF00289509)

* R. Bayer, K. Unterauer, _"Prefix B-trees"_, ACM Transactions on Database Systems, Volume 2, Issue 1, pp. 11-26, Year 1977, [Abstract](https://dl.acm.org/citation.cfm?id=320530&dl=ACM&coll=DL)

* D. Comer, _"Ubiquitous B-Tree"_, ACM Computing Surveys, Volume 11, Issue 2, Pages 121-137, Year 1979, [PDF](http://www.ezdoum.com/upload/14/20020512204603/TheUbiquitousB-Tree.pdf) 

* J. Zhou _"A B+-tree Index fot the Know-It-All Database Framework"_, 2003, 
[PDF](https://spectrum.library.concordia.ca/2344/1/MQ83927.pdf)

 * D. Lomet, _"The Evolution of Effective B-tree: Page Organization and Techniques: A Personal Account"_, SIGMOD Record, Volume 30, No. 3, 2001, [PDF](https://www.microsoft.com/en-us/research/wp-content/uploads/2016/02/p64-lomet.pdf)
 
 * R. ORLANDIC, H.M. MAHMOUD _"STORAGE OVERHEAD OF O-TREES, B-TREES AND PREFIX B-TREES: A COMPARATIVE ANALYSIS"_, International
Journal of Foundations of Computer Science, [PDF](http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.133.9412&rep=rep1&type=pdf)

 * G. Graefe, _"Write-Optimized B-Trees "_, Proceedings of the 30th VLDB Conference, 2004, [PDF](http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.90.4825&rep=rep1&type=pdf)
 
 * G. Graefe, _"A Survey of B-Tree Logging and Recovery Technique"_, ACM Transactions on Database Systems, Vol. 35, No. 3, Article 16, 2012, [PDF](https://people.eecs.berkeley.edu/~kubitron/cs262/handouts/papers/a1-graefe.pdf)
 
 * G. Graefe, _"Modern B-Tree Techniques"_, Foundations and Trends in Databases, Vol. 3, No. 4, pp. 203–402, Year 2010, [PDF](https://pdfs.semanticscholar.org/0b19/f413ffb5bc68b43f3bd05a97c282a7c6d6ab.pdf)
 
 * G. Graefe, _"A Survey of B-Tree Locking Technique"_, ACM Transactions on Database Systems, Vol. 35, No. 3, Article 16, Year 2010, [PDF](http://people.eecs.berkeley.edu/~kubitron/courses/cs262a-S16/handouts/papers/a16-graefe.pdf)
 
 * P. Wang, _"An In-Depth Analysis of Concurrent B-tree Algorithms"_, Massachusetts Institute of Technology Report MIT/LCS/TR-496, Year 1991, [PDF](http://www.dtic.mil/dtic/tr/fulltext/u2/a232287.pdf)
 
 * Y.-S. KWONG,D. WOOD, _"A New Method for Concurrency in B-Trees"_, IEEE TRANSACTIONS ON SOFTWARE ENGINEERING, VOL. SE-8, NO. 3,
pp. 211-222, Year 1982, [PDF](https://www.computer.org/csdl/trans/ts/1982/03/01702938.pdf) 


## Similar projects in C++: 

* [cpp-btree](https://code.google.com/archive/p/cpp-btree/)

* [STX](http://panthema.net/2007/stx-btree/)

* From Wikipedia: [B+Tree](http://www.amittai.com/prose/bplustree_cpp.html) (**Caution!** The code has the bug in deletion procedure!)


### Author
* Zbigniew Romanowski
* e-mail: romz@wp.pl


