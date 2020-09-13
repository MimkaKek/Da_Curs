#!/bin/bash
#Руководство:
#1) для запуска вводите в терминал bash for_test.sh
#2) после первого echo можно вводить числа
# отрицательные числа удаляют сгенерированные ЭТИМ скиптом тесты
# 0 запускает проверку с этими же тестами
# положительные числа создают новые тестовые файлы с заданным числом символов
# при 0 можно ввести проверку на ключ -с

echo "Input somthing less then 0 for deleting test files, 0 for execution with same tests files or something else to resize test files"

read action

if [ "$action" -gt  0 ]
then
	rm -rf tests
	mkdir tests
	#генерация тестовых файлов
	./gen "$action"	>> "tests/test01"
	./gen "$action" >> "tests/test11"
	./gen "$action" >> "tests/test21"
	./gen "$action" >> "tests/test31"
	./gen "$action"	>> "tests/test41"
	./gen "$action" >> "tests/test51"
	./gen "$action" >> "tests/test61"
	./gen "$action" >> "tests/test71"
	./gen "$action" >> "tests/test81"
	./gen "$action" >> "tests/test91"
	echo "end gen"
	##архивация и создание доп копий тестовых файлов
	./main  -1k 	tests/test01
	./main	-k		tests/test11
	./main  -9k 	tests/test21
	./main  -1k  	tests/test31
	./main 	-k  	tests/test41
	./main  -9k 	tests/test51
	./main  -1k  	tests/test61
	./main  -k  	tests/test71
	./main  -9k 	tests/test81
	./main  -1k 	tests/test91
	echo "end compr"
	#разархивация
	./main -dc tests/test01.gz >> tests/test02
	./main -dc tests/test11.gz >> tests/test12
	./main -dc tests/test21.gz >> tests/test22
	./main -dc tests/test31.gz >> tests/test32
	./main -dc tests/test41.gz >> tests/test42
	./main -dc tests/test51.gz >> tests/test52
	./main -dc tests/test61.gz >> tests/test62
	./main -dc tests/test71.gz >> tests/test72
	./main -dc tests/test81.gz >> tests/test82
	./main -dc tests/test91.gz >> tests/test92
	echo "end decom"
	#проверка совпадения результатов
	./comparator
	rc=$?
	if [ "$rc" -eq 0 ]
	then
		echo "YOU HAVE DONE THIS CORRECTLY"
	else
		echo "YOU HAVE ERRORS IN ALGORITHM"
	fi
elif [ "$action" -eq 0 ]
then
	echo "want -c - print 1"
	read key
	cp "tests/test01" "tests/test02"
	cp "tests/test11" "tests/test12"
	cp "tests/test21" "tests/test22"
	cp "tests/test31" "tests/test32"
	cp "tests/test41" "tests/test42"
	cp "tests/test51" "tests/test52"
	cp "tests/test61" "tests/test62"
	cp "tests/test71" "tests/test72"
	cp "tests/test81" "tests/test82"
	cp "tests/test91" "tests/test92"
	echo "end cp"
	./main -1 tests/test02
	./main 	  tests/test12
	./main -9 tests/test22
	./main -1 tests/test32
	./main 	  tests/test42
	./main -9 tests/test52
	./main -1 tests/test62
	./main    tests/test72
	./main -9 tests/test82
	./main -1 tests/test92
	echo "end compr"
	#разархивация
	if [ "$key" -eq 1 ]
	then
		./main -dc tests/test02.gz >> tests/test02
		./main -dc tests/test12.gz >> tests/test12
		./main -dc tests/test22.gz >> tests/test22
		./main -dc tests/test32.gz >> tests/test32
		./main -dc tests/test42.gz >> tests/test42
		./main -dc tests/test52.gz >> tests/test52
		./main -dc tests/test62.gz >> tests/test62
		./main -dc tests/test72.gz >> tests/test72
		./main -dc tests/test82.gz >> tests/test82
		./main -dc tests/test92.gz >> tests/test92
	else
		./main -d tests/test02.gz 
		./main -d tests/test12.gz 
		./main -d tests/test22.gz 
		./main -d tests/test32.gz 
		./main -d tests/test42.gz 
		./main -d tests/test52.gz
		./main -d tests/test62.gz
		./main -d tests/test72.gz
		./main -d tests/test82.gz
		./main -d tests/test92.gz
	fi
	echo "end decom"
	#проверка совпадения результатов
	./comparator
	rc=$?
	if [ "$rc" -eq 0 ]
	then
		echo "YOU HAVE DONE THIS CORRECTLY"
	else
		echo "YOU HAVE ERRORS IN ALGORITHM"
	fi
else
	rm -rf tests
fi
