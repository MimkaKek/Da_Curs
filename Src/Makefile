TARGET  =main
CC      =g++
CFLAGS  =-pedantic -Wall -Werror -Wno-sign-compare -Wno-long-long -lm -c
SRC     =ACC.cpp BFile.cpp LZW.cpp TPrefix.cpp main.cpp main_help.cpp
OBJS    =$(SRC:.cpp=.o)

.PHONY: all clean clean_all

all: $(TARGET) gen comparator 
	rm *.o
	echo DONE

$(TARGET): $(OBJS)
	$(CC) $(DEBUG) $^ -o $@
gen:
	g++ gen.cpp -o gen
comparator: comparator.o BFile.o
	g++ comparator.o BFile.o -o comparator
	
%.o: %.cpp
	$(CC) $(CFLAGS) $(DEBUG) $<

clean:
	rm *.o $(TARGET)
clean_tests:
	rm test*
clean_all:
	rm *.o test* $(TARGET) gen comparator
