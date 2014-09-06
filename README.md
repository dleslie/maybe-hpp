maybe-hpp
=========

Provides a container for returning and passing values that may or may not exist. The container is unique, where the values are swapped rather than copied on assignment, and memory management for non-immediate types are handled internally.


```
make && ./test
gcc -std=c++11 test.cpp -lstdc++ -o test
Size of maybe<int>: 8
Size of maybe<foo>: 8
Testing has_value:
Does no_int have a value? 0
Does has_int have a value? 1
foo created
Does no_foo have a value? 0
Does has_foo have a value? 1
foo destroyed

Testing copy:
Does nothing_a have a value? 0
Does nothing_b have a value? 0
Does int_a have a value? 0
Does int_b have a value? 1
Value of int_b: 1
foo created
Does foo_a have a value? 0
Does foo_b have a value? 1
Value of foo_b: 1
foo destroyed

```
