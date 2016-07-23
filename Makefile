INCLUDE = ./include
TEST = ./test
SRC = ./src

CFLAGS = -g -I $(INCLUDE)
LIBS = -lm
CC = gcc

_SRC_OBJ = motion.o bvh_joint.o bvh_load.o kinematics.o space_temporal.o
_SRC_DEPS = vector.h bvh_header.h dictionary.h motion.h kinematics.h space_temporal.h

SRC_OBJ = $(patsubst %, $(SRC)/%, $(_SRC_OBJ))
SRC_DEPS = $(patsubst %, $(INCLUDE)/%, $(_SRC_DEPS))

$(SRC)/%.o : $(SRC)/%.c $(SRC_DEPS) 
	$(CC) -c -o $@ $< $(CFLAGS)	

vector_test: $(TEST)/vector_test.o $(INCLUDE)/vector.h
	$(CC) -o $@ $^ $(CFLAGS)

dic_test: $(TEST)/dictionary_test.o $(INCLUDE)/dictionary.h
	$(CC) -o $@ $^ $(CFLAGS)

bvh_test : $(SRC_OBJ) $(TEST)/bvh_test.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

kinematics_test : $(SRC_OBJ) $(TEST)/kinematics_test.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

planes_test: $(SRC_OBJ) $(TEST)/planes_test.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

angle_test : $(SRC_OBJ) $(TEST)/angle_test.c
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

step_detect : $(SRC_OBJ) $(TEST)/step_detect.c
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

space_temporal : $(SRC_OBJ) $(TEST)/space_temporal.c
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

egocentric : $(SRC_OBJ) $(TEST)/egocentric_test.c
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

mariela : $(SRC_OBJ) $(TEST)/mariela.c
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

clean:
	rm $(SRC)/*.o
	rm $(TEST)/*.o
