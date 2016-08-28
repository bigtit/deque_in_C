// use C style
#ifndef __MYDEQUE__
#define __MYDEQUE__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define DATA_SIZE (1<<16)
#define ITS_SIZE (1<<10)
#define TAB_SIZE (1<<10) /* max capacity of deque is 1<<(16+10) */
/* TODO
 * make the size of its and trace_table dynamic if necessary
 * may use some vars tables to track the size of its and trace_table
 * like the implementation of Deque_##T itself
 */

#define Deque_DEFINE(T)  \
struct Deque_##T; \
struct Deque_##T##_Iterator; \
typedef bool (*less_func_##T)(const T &, const T &); \
\
int Deque_##T##_size(Deque_##T * c); \
bool Deque_##T##_empty(Deque_##T * c); \
T & Deque_##T##_at(Deque_##T * c, int idx); \
T & Deque_##T##_front(Deque_##T * c); \
T & Deque_##T##_back(Deque_##T * c); \
Deque_##T##_Iterator& Deque_##T##_begin(Deque_##T * c); \
Deque_##T##_Iterator& Deque_##T##_end(Deque_##T * c); \
void Deque_##T##_push_back(Deque_##T * c, T m); \
void Deque_##T##_push_front(Deque_##T * c, T m); \
T Deque_##T##_pop_back(Deque_##T * c); \
T Deque_##T##_pop_front(Deque_##T * c); \
void Deque_##T##_clear(Deque_##T * c); \
void Deque_##T##_dtor(Deque_##T * c); \
 \
Deque_##T##_Iterator* it_get(Deque_##T * c, int idx); \
 \
 \
struct Deque_##T##_Iterator { \
  struct Deque_##T * deq; \
  int idx; \
 \
  void (*inc)(Deque_##T##_Iterator*); \
  void (*dec)(Deque_##T##_Iterator*); \
 \
  T & (*deref)(Deque_##T##_Iterator*); \
}; \
struct Deque_##T { \
  T * data; /* the 1st data array == trace_table[0], \
               preserved from previous version with only one data array, can be discarded since we have trace_table */ \
  char type_name[sizeof("deque_"#T)]; \
\
  /* like STL, the _end points to the place after the last element */ \
  int _beg, _end, _begi, _endi; \
  T ** trace_table; \
\
  Deque_##T##_Iterator** its; \
  int itn; \
 \
  less_func_##T lf; \
 \
  int (*size)(Deque_##T *); \
  bool (*empty)(Deque_##T *); \
 \
  Deque_##T##_Iterator& (*begin)(Deque_##T *); \
  Deque_##T##_Iterator& (*end)(Deque_##T *); \
 \
  T & (*back)(Deque_##T *); \
  T & (*front)(Deque_##T *); \
  T & (*at)(Deque_##T *, int); \
 \
  void (*push_back)(Deque_##T *, T); \
  void (*push_front)(Deque_##T *, T); \
  T (*pop_back)(Deque_##T *); \
  T (*pop_front)(Deque_##T *); \
 \
  void (*clear)(Deque_##T *); \
  void (*dtor)(Deque_##T *); \
}; \
 \
bool Deque_##T##_equal(Deque_##T & c1, Deque_##T & c2) { \
  if(c1.size(&c1)!=c2.size(&c2)) return false; \
  if(c1.lf != c2.lf) return false; \
  int s = c1.size(&c1); \
\
  for(int i=0; i<s; ++i) \
    if((c1.lf)(c1.at(&c1, i), c2.at(&c1, i)) || \
        (c1.lf)(c2.at(&c2, i), c1.at(&c1, i))) \
      return false; \
  return true; \
} \
\
void Deque_##T##_ctor(Deque_##T * c, less_func_##T lf) { \
  c->_beg = c->_end = DATA_SIZE>>1; /* just for convinience cuz balanced push and pop operations are assumed */ \
  c->_begi = c->_endi = 0; \
  strcpy(c->type_name, "Deque_"#T); \
  c->data = (T *)malloc(DATA_SIZE*sizeof(T)); \
  c->trace_table = (T **)malloc(TAB_SIZE*sizeof(T *)); \
  c->its = (Deque_##T##_Iterator **)malloc(ITS_SIZE*sizeof(Deque_##T##_Iterator *)); \
  memset(c->data, 0, DATA_SIZE*sizeof(T)); \
  memset(c->trace_table, 0, TAB_SIZE*sizeof(T *)); \
  memset(c->its, 0, ITS_SIZE*sizeof(Deque_##T##_Iterator **)); \
  c->trace_table[0] = c->data; \
  c->itn = 0; \
\
  c->lf = lf; \
  c->size = Deque_##T##_size; \
  c->empty = Deque_##T##_empty; \
  c->begin = Deque_##T##_begin; \
  c->end = Deque_##T##_end; \
  c->back = Deque_##T##_back; \
  c->front = Deque_##T##_front; \
  c->at = Deque_##T##_at; \
  c->push_back = Deque_##T##_push_back; \
  c->push_front = Deque_##T##_push_front; \
  c->pop_back = Deque_##T##_pop_back; \
  c->pop_front = Deque_##T##_pop_front; \
  c->clear = Deque_##T##_clear; \
  c->dtor = Deque_##T##_dtor; \
} \
void Deque_##T##_clear(Deque_##T *) { \
\
} \
\
void Deque_##T##_dtor(Deque_##T * c) { \
  /* there's no constructor and destructor for iterator struct */ \
  /* keep track of them manually */ \
  if(c->its) { \
    for(int i=0; i<ITS_SIZE; ++i) { \
      if(c->its[i]) free(c->its[i]); \
    } \
    free(c->its); \
  } \
  if(c->trace_table) { \
    for(int i=0; i<TAB_SIZE; ++i) { \
      if(c->trace_table[i]) free(c->trace_table[i]); \
    } \
    free(c->trace_table); \
  } \
} \
 \
\
int Deque_##T##_size(Deque_##T * c) { \
  return c->_end - c->_beg + \
    ((c->_endi%2?(-1-c->_endi/2):c->_endi/2) - (c->_begi%2?(-1-c->_begi/2):c->_begi/2))*DATA_SIZE; \
} \
bool Deque_##T##_empty(Deque_##T * c) { \
  /* not use size() cuz this way is simpler */ \
  return c->_beg == c->_end && c->_begi == c->_endi; \
} \
T & Deque_##T##_at(Deque_##T * c, int idx) { \
  /* odd indices for negative, which means _begi or _endi start before 0 the init data array */ \
  /* even indices for positive, which means _begi _endi start after 0 the init data array */ \
  /* so there's a map between indices of trace_table and "actual" indices we need to calculate positions */ \
  int t = (idx+c->_beg)%DATA_SIZE; \
  int ta = (idx+c->_beg)/DATA_SIZE; \
  int tb = c->_begi%2?(-1-c->_begi/2):c->_begi/2 + ta; \
  int tidx = tb>=0?2*tb:-1-2*tb; \
  return c->trace_table[tidx][t]; \
} \
T & Deque_##T##_front(Deque_##T * c) { \
  return c->trace_table[c->_begi][c->_beg]; \
} \
T & Deque_##T##_back(Deque_##T * c) { \
  return c->trace_table[c->_endi][c->_end-1]; \
} \
/* there's no constructor and destructor for iterator struct */ \
/* keep track of them manually */ \
Deque_##T##_Iterator& Deque_##T##_begin(Deque_##T * c) { \
  auto it = it_get(c, 0); \
  return *it; \
} \
Deque_##T##_Iterator& Deque_##T##_end(Deque_##T * c) { \
  auto it = it_get(c, c->size(c)); \
  return *it; \
} \
 \
T * Deque_##T##_new_data_array() { \
  T * p = (T*)malloc(DATA_SIZE*sizeof(T)); \
  assert(p!=nullptr); \
  return p; \
} \
\
void Deque_##T##_push_back(Deque_##T * c, T m) { \
  if(c->_end == DATA_SIZE) { \
    c->_end = 1; \
    if(c->_endi==1) c->_endi = 0; /* 0 and 1 are two special cases */ \
    else if(c->_endi%2)c->_endi -= 2; \
    else c->_endi += 2; \
    /* reuse existed data arrays */ \
    if(!c->trace_table[c->_endi]) c->trace_table[c->_endi] = Deque_##T##_new_data_array(); \
  } \
  else ++c->_end; \
  c->back(c) = m; \
} \
void Deque_##T##_push_front(Deque_##T * c, T m) { \
  if(!c->_beg) { \
    c->_beg = DATA_SIZE-1; \
    if(c->_begi==0) c->_begi = 1; \
    else if(c->_begi%2) c->_begi += 2; \
    else c->_begi -= 2; \
    if(!c->trace_table[c->_begi]) c->trace_table[c->_begi] = Deque_##T##_new_data_array(); \
  } \
  else --c->_beg; \
  c->front(c) = m; \
} \
/* no ref should be returned by pop functions */ \
T Deque_##T##_pop_back(Deque_##T * c) { \
  T sav = c->back(c); \
  if(c->_end == 1) { \
    c->_end = DATA_SIZE; \
    if(c->_endi==0) c->_endi = 1; \
    else if(c->_endi%2) c->_endi += 2; \
    else c->_endi -= 2; \
  } \
  else --c->_end; \
\
  return sav; \
} \
T Deque_##T##_pop_front(Deque_##T * c) { \
  T sav = c->front(c); \
  if(c->_beg == DATA_SIZE-1) { \
    c->_beg = 0; \
    if(c->_begi==1) c->_begi = 0; \
    else if(c->_begi%2) c->_begi -= 2; \
    else c->_begi += 2; \
  } \
  else ++c->_beg; \
  return sav; \
} \
 \
 \
\
/* nothing special needed in iterator operations */ \
void Deque_##T##_Iterator_inc(Deque_##T##_Iterator* it) { \
  ++it->idx; \
} \
void Deque_##T##_Iterator_dec(Deque_##T##_Iterator* it) { \
  --it->idx; \
} \
T & Deque_##T##_Iterator_deref(Deque_##T##_Iterator* it) { \
  return it->deq->at(it->deq, it->idx); \
} \
\
bool Deque_##T##_Iterator_equal(Deque_##T##_Iterator& it1, Deque_##T##_Iterator& it2) { \
  if(it1.idx!=it2.idx || it1.deq!=it2.deq) return false; \
  return true; \
} \
\
\
\
Deque_##T##_Iterator* it_get(Deque_##T * c, int idx) { \
  int i; \
  Deque_##T##_Iterator* it; \
  for(i=0; i<ITS_SIZE; ++i) { \
    if(!c->its[(i+c->itn)%ITS_SIZE]){ \
      it = (Deque_##T##_Iterator*)malloc(sizeof(Deque_##T##_Iterator)); \
      c->its[(i+c->itn)%ITS_SIZE] = it; \
      ++c->itn; \
      break; \
    } \
  } \
\
  /* TODO \
   * reuse existed iterators \
   * may incur undefined behaviors if the iterator overwrited is being used */ \
  if(i==ITS_SIZE) { \
    c->its[c->itn]->idx = idx; \
    ++c->itn; \
    return c->its[c->itn]; \
  } \
  if(!it) return nullptr; \
  it->deq = c; \
  it->idx = idx; \
  it->inc = Deque_##T##_Iterator_inc; \
  it->dec = Deque_##T##_Iterator_dec; \
  it->deref = Deque_##T##_Iterator_deref; \
  return it; \
}

#endif
