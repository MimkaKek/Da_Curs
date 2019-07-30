all: main_help.o main.o TPrefix.o LZW.o BFile.o gen
	g++ main_help.o main.o TPrefix.o LZW.o BFile.o -o main
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
BFile.o:
	g++ -c -g BFile.cpp
#Включу ваши алги как они будут готовы
Arithmetic.o:
	g++ -c -g Arithmetic.cpp
LZ77.o:
	g++ -c -g LZ77.cpp
#clear
c:
	rm *.o main gen
