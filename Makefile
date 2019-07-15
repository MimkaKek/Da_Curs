all: main_help.o main.o
	g++ main_help.o main.o -o main
	rm *.o
main.o:
	g++ -c -g main.cpp
main_help.o:
	g++ -c -g main_help.cpp
#clear
c:
	rm *.o main
