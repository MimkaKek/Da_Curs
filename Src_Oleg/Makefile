all: comparator gen ACC.o BFile.o main.o interface.o LZ77.o
	g++ ACC.o BFile.o main.o interface.o LZ77.o -g -o main
	rm *.o
	echo DONE
gen:
	g++ -g gen.cpp -o gen
comparator: BFile.o
	g++ -g BFile.o comparator.cpp -o comparator
ACC.o:
	g++ -c -g ACC.cpp
BFile.o:
	g++ -c -g BFile.cpp
LZ77.o:
	g++ -c -g LZ77.cpp
main.o:
	g++ -c -g main.cpp
interface.o:
	g++ -c -g interface.cpp
clean:
	rm -rf tests gen comparator *.o
