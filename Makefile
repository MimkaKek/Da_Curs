all: main_help.o main.o gen
	g++ main_help.o main.o -o main
	rm *.o
	echo DONE
gen:
	g++ gen.cpp -o gen
main.o:
	g++ -c -g main.cpp
main_help.o:
	g++ -c -g main_help.cpp
TPrefix.o:
	g++ -c -g TPrefix.cpp
LZW.o:
	g++ -c -g LZW.cpp
#clear
c:
	rm *.o main gen
