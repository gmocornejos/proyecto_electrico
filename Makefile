INCLUDE = ./include
TEST = ./test
SRC = ./src

CFLAGS = -I $(INCLUDE)
LIBS = -lm
CC = gcc

_SRC_OBJ = bvh_joint.o bvh_load.o
_SRC_DEPS = vector.h bvh_header.h

SRC_OBJ = $(patsubst %, $(SRC)/%, $(_SRC_OBJ))
SRC_DEPS = $(patsubst %, $(INCLUDE)/%, $(_SRC_DEPS))

$(SRC)/%.o : $(SRC)/%.c $(SRC_DEPS) 
	$(CC) -c -o $@ $< $(CFLAGS)	

vector_test: $(TEST)/vector_test.o $(INCLUDE)/vector.h
	$(CC) -o $@ $^ $(CFLAGS)

bvh_test : $(SRC_OBJ) $(TEST)/bvh_test.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

clean:
	rm $(SRC)/*.o
	rm $(TEST)/*.o


