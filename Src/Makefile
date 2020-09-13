TARGET  =main
CC      =g++
CFLAGS  =-pedantic -Wall -Werror -Wno-sign-compare -Wno-long-long -lm -c -g
SRC     =ACC.cpp BFile.cpp LZW.cpp TPrefix.cpp main.cpp main_help.cpp
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
	rm *.o $(TARGET) gen comparator
clean_tests:
	rm -r tests
clean_all:
	rm -r *.o tests $(TARGET) gen comparator
