TARGET  =main
CC      =g++
CFLAGS  =-pedantic -Wall -Werror -Wno-sign-compare -Wno-long-long -lm -c -g
SRC     =ACC.cpp BFile.cpp LZW.cpp Prefix.cpp main.cpp main_help.cpp
OBJS    =$(SRC:.cpp=.o)

.PHONY: all clean clean_tests clean_all

all: $(TARGET) gen comparator
	@echo DONE

$(TARGET): $(OBJS)
	$(CC) $(DEBUG) $^ -o $@
gen: gen.o
	$(CC) $(DEBUG) $^ -o $@
comparator: comparator.o BFile.o
	$(CC) $(DEBUG) $^ -o $@
	
%.o: %.cpp
	$(CC) $(CFLAGS) $(DEBUG) $<

clean:
	rm -f *.o $(TARGET) gen comparator
clean_tests:
	rm -rf tests
clean_all:
	rm -rf *.o tests $(TARGET) gen comparator
