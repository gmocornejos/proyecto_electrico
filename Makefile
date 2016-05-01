INCLUDE = ./include
TEST = ./test
SRC = ./src

CFLAGS = -I $(INCLUDE)
LIBS = -lm
CC = gcc

_SRC = bvh_joint.o bvh_load.o
_SRC_DEPS = vector.h bvh_header.h

src : $(patsubst %, $(SRC)/%, $(_SRC))
	$(CC) -c -o $^ $(CFLAGS)
	

vector_test: $(TEST)/vector_test.c $(INCLUDE)/vector.h
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm $(SRC)/*.o


