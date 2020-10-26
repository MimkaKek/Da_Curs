all: Arithmetic.o LZ77.o main.o preprocessing.o gen comparator
	g++ Arithmetic.o LZ77.o main.o preprocessing.o -g -o main
	rm *.o
	echo DONE

gen:
	g++ -g gen.cpp -o gen

comparator: BFile.o
	g++ -g BFile.o comparator.cpp -o comparator

Arithmetic.o:
	g++ -c -g Arithmetic.cpp

BFile.o:
	g++ -c -g BFile.cpp

LZ77.o:
	g++ -c -g LZ77.cpp

main.o:
	g++ -c -g main.cpp

preprocessing.o:
	g++ -c -g preprocessing.cpp

clean:
	rm -rf tests gen comparator
