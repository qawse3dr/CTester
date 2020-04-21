CC = gcc

#DIR used
BIN = ./bin/
BUILD = ./build/
SRC = ./src/
INCLUDE = ./include/
TEST = ./test/

CFLAGS = -g -std=c11 -I$(INCLUDE)
OBJ = $(BIN)ctester.o

#Builds .so and places it in build
build : $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -shared -fpic -o $(BUILD)libctester.so

#installs to /usr/lib/
#might need super user
install: build
	cp $(BUILD)libctester.so /usr/lib/
	cp $(INCLUDE)ctester.h /usr/include/

$(BIN)ctester.o: $(SRC)ctester.c $(INCLUDE)ctester.h
	$(CC) $(CFLAGS) -fpic -c $(SRC)ctester.c -o $@

#testfiles
test: build $(BIN)test.o
	$(CC) $(CFLAGS) $(BIN)test.o -L$(BUILD) -lctester -o $(BUILD)$@

	#run program
	LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$(BUILD) $(BUILD)test

#testfiles memory test
memtest: build $(BIN)test.o
	#set library paths for c compiler
	export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$(BUILD);
	$(CC) $(CFLAGS) $(BIN)test.o -L$(BUILD) -lctester -o $(BUILD)$@

	#run program
	LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$(BUILD) valgrind  --leak-check=full $(BUILD)memtest

$(BIN)test.o: $(TEST)tester.c
	$(CC) $(CFLAGS) -c $(TEST)tester.c -o $@
license:
	cat LICENSE

clean:
	rm -rf $(BIN)* $(BUILD)*
