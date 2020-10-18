all: Arithmetic.o BFile.o LZ77.o main.o main_help.o gen comparator
	g++ Arithmetic.o BFile.o LZ77.o main.o main_help.o -g -o main
	rm *.o
	echo DONE

gen:
g++ -g gen.cpp -o gen

comparator:
g++ -g comparator.cpp -o comparator

Arithmetic.o:
	g++ -c -g Arithmetic.cpp

BFile.o:
	g++ -c -g BFile.cpp

LZ77.o:
	g++ -c -g LZ77.cpp

main.o:
	g++ -c -g main.cpp

main_help.o:
	g++ -c -g main_help.cpp

clean:
	rm -rf tests gen comparator
