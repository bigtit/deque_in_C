Kaiming Fang
kfang2@binghamton.edu
B00671111



# Some Notes

#### My archive includes: Makefile, Deque.hpp, README.md, README.txt

------
### 2016-08-26

It seems that there's no way to get rid of deque object as a parameter in functions of struct 
itself.

I use one array to store user data(such as MyClass), and put two indices(_beg and _end) to trace the head and tail of deque, which simplify most of the operations and shorten the codes, while introducing several unmaintainable problems:
 * The capacity of the array to store user data is fixed so that a waste or lack of space might happen when encounter a specific situation.
 * no bounds checking for arrays incurs undefined behaviors.

A linked list could be better in these issues, but the performance might be worse because of the search time for a specific node.

The implementation of deque in STL seems to be using a 2-level array.

The capacity of single array is not enough for performance test of int type.

------

### 2016-08-27

A possible way to solve not-enough-space problem:
 * malloc small arrays storing user data, build a table to trace them, a small array is not generated until it is needed.
 * use 2 additional ints(_begi and _endi) to trace the index of first and last data array in 
trace table.
 * generate new small array when there's no enough space, update the trace table and 4 indices.
 * free the first or last data array when data is popped and bound checking returns true, update trace table and 4 indices.
 * use odd and even indices in trace table to distinguish front and back arrays so that we can easily find them.
 * Structure of trace_table indices:
  ...|--2n+1--|...|--7--|--5--|--3--|--1--|--0--|--2--|--4--|...|--2m--|...
 The diagram above shows the memory structure a user expects about the deque, the numbers inside indicate the indices of trace_table.

Discard the free steps in pop_front, pop_back and it_get to reuse existed memory space, reducing redundant allocation operations.

------

### 2016-08-28

The performance on a VirtualBox VM with 2GB ram and 1 core is not bad and the result is the same as what has been given in correct result file except for the last line that differs in allocation and memory using numbers.

To lower the allocation number I tried to increase the `DATA_SIZE` so that one data array could hold more user data to reduce the frequency of allocations, while the number of allocations influences the time costs of overheads in each allocation operation.

*valgrind report*
There's stll a small block of memory space in use at exit according to valgrind report, and it seems to be *a known bug from GCC 5.1(https://gcc.gnu.org/bugzilla/show_bug.cgi?id=64535)*.

I changed the `DATA_SIZE` to 1<<14 with the TAB_SIZE to 1<<12, and found that the running time is almost the same (3.588s VS 3.584).

I guess the time costs of overheads in allocation operations is fairly small compared with other operations such as push or pop.

------

Add implementation of clear().

------

**The result of diff my_output corrent_output:**
  108c108
  < 579 allocations totalling 128666200 bytes
  ---
  > 60 allocations totalling 142680112 bytes

**The result of valgrind:**
  valgrind ./test --leak-check=full

  ==2681== Memcheck, a memory error detector
  ==2681== Copyright (C) 2002-2015, and GNU GPL'd, by Julian Seward et al.
  ==2681== Using Valgrind-3.11.0 and LibVEX; rerun with -h for copyright info
  ==2681== Command: ./test --leak-check=full
  ==2681==
  ==2681==
  ==2681== HEAP SUMMARY:
  ==2681==     in use at exit: 72,704 bytes in 1 blocks
  ==2681==   total heap usage: 579 allocs, 578 frees, 128,666,200 bytes allocated
  ==2681==
  ==2681== LEAK SUMMARY:
  ==2681==    definitely lost: 0 bytes in 0 blocks
  ==2681==    indirectly lost: 0 bytes in 0 blocks
  ==2681==      possibly lost: 0 bytes in 0 blocks
  ==2681==    still reachable: 72,704 bytes in 1 blocks
  ==2681==         suppressed: 0 bytes in 0 blocks
  ==2681== Rerun with --leak-check=full to see details of leaked memory
  ==2681==
  ==2681== For counts of detected and suppressed errors, rerun with: -v
  ==2681== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)

**Build environment:**
  4.7.2-1-ARCH x86_64 GNU/Linux
  gcc version 6.1.1 20160802 (GCC)

