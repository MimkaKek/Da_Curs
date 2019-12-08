#Makefile
all: main_help.o main.o TPrefix.o LZW.o BFile.o gen comparator ACC.o
	g++ main_help.o main.o TPrefix.o LZW.o BFile.o ACC.o -o main
	rm *.o
	echo DONE
gen:
	g++ gen.cpp -o gen
comparator: comparator.o BFile.o
	g++ comparator.o BFile.o -o comparator
comparator.o:
	g++ -c -g comparator.cpp
main.o:
	g++ -c -g main.cpp
main_help.o:
	g++ -c -g main_help.cpp
TPrefix.o:
	g++ -c -g TPrefix.cpp
LZW.o:
	g++ -c -g LZW.cpp
BFile.o:
	g++ -c -g BFile.cpp
ACC.o:
	g++ -c -g ACC.cpp
#TODO Включу ваши алги как они будут готовы
LZ77.o:
	g++ -c -g LZ77.cpp
#clear
c:
	rm *.o main
ca:
	rm *.o main gen comparator
