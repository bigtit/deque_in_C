CC=g++
CFLAGS=-g3 -Wall -Wextra -pedantic
LDLIBS=-ldl # dlsym
DEPS=Deque.hpp
OBJ=test.o

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
test: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDLIBS)
clean:
	rm -f *.o
