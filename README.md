# fast_List
>A fast, function-oriented, stack-memory based, doubly-linked list.


Motivation
----
This class was created as a part of my Sr. Design project. I needed to be able to organize arbitrarily sized data in a uniform fashion, but couldn't afford the processing overhead (or memory requirements) of heap allocation. This class implements a quick 'n dirty (i.e. *simple*) version of the `STL::deque`. Instead of allocating memory on the heap, this class is designed to allocate on the stack. This alteration means that memory can be allocated at the initialization phase of my run-time operations, and never dealt with again. Using stack-based allocation also gives me constant run-time complexity for a number of algorithmic operations; that is what is most important.

Design
---
The class is designed to function just like a `STL::deque`. It has all the obvious methods there-of as well as some extra ones I threw in. For example, I wanted the list to be more function-oriented. The built-in `apply` method achieves this end by calling the provided function with each node as argument. The list also provides an index data member for each node in the list, so that the original order can be returned to if desired.

Documentation
---
All methods and classes are documented doxy-style. For further information check out [doxygen](www.doxygen.org).
