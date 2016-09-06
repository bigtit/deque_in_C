CC=g++
CFLAGS=-std=c++11 -Wall -Wextra -pedantic
LDLIBS=-ldl # dlsym
DEPS=Deque.hpp
OBJ=test.o

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
test: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDLIBS)
clean:
	rm -f *.o
