[![Build Status](https://travis-ci.org/romz-pl/b-plus-tree.svg?branch=master)](https://travis-ci.org/romz-pl/b-plus-tree)


# Memory based B+ tree in C++ 


This is refactored version of the Amittai's code 
http://www.amittai.com/prose/bplustree_cpp.html

I have found the link to Amittai's code (on October 2017) in wikipedia's article describing to B+tree https://en.wikipedia.org/wiki/B%2B_tree. The link is in section _Implementation_ and it is called _Interactive B+ Tree Implementation in C++_.

**Caution!** The current implementation have the bug in deletion procedure. See Issues. 


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


### Author
* Zbigniew Romanowski
* e-mail: romz@wp.pl


