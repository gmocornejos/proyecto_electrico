INCLUDE = ./include
TEST = ./test
SRC = ./src
EXMPLS = ./examples

CFLAGS = -I $(INCLUDE)
LIBS = -lm
CC = gcc

SRC_OBJ = $(patsubst %, %.o, $(basename $(wildcard $(SRC)/*)))
SRC_DEPS = $(wildcard $(INCLUDE)/*)
TEST_FILES = $(basename $(wildcard $(TEST)/*))
EXAMPLES = $(basename $(wildcard $(EXMPLS)/*))

$(SRC)/%.o : $(SRC)/%.c $(SRC_DEPS) 
	$(CC) -c -o $@ $< $(CFLAGS)

libmas.a : $(SRC_OBJ)
	ar rcs $@ $^

test : libmas.a
	for file in $(TEST_FILES); \
	do \
		$(CC) -static $$file.c $^ $(CFLAGS) $(LIBS) -o $$file ; \
	done

examples: libmas.a
	for file in $(EXAMPLES); \
	do \
		$(CC) -static $$file.c $^ $(CFLAGS) $(LIBS) -o $$file; \
	done
	
clean:
	rm $(SRC)/*.o

clean_bin:
	rm *.a
	rm $(TEST_FILES) $(EXAMPLES)

