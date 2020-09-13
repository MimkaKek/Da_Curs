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
	rm "test01" "test02" "test11" "test12" "test21" "test22" "test31" "test32" "test41" "test42" "test51" "test52" "test61" "test62" "test71" "test72" "test81" "test82" "test91" "test92"
	rm "test02.gz" "test12.gz" "test22.gz" "test32.gz" "test42.gz" "test52.gz" "test62.gz" "test72.gz" "test82.gz" "test92.gz" "test01.gz" "test11.gz" "test21.gz" "test31.gz" "test41.gz" "test51.gz" "test61.gz" "test71.gz" "test81.gz" "test91.gz"
	#генерация тестовых файлов
	./gen "$action"	>> "test01"
	./gen "$action" >> "test11"
	./gen "$action" >> "test21"
	./gen "$action" >> "test31"
	./gen "$action"	>> "test41"
	./gen "$action" >> "test51"
	./gen "$action" >> "test61"
	./gen "$action" >> "test71"
	./gen "$action" >> "test81"
	./gen "$action" >> "test91"
	echo "end gen"
	##архивация и создание доп копий тестовых файлов
	./main  -1k 	test01
	./main	-k		test11
	./main  -9k 	test21
	./main  -1k  	test31
	./main 	-k  	test41
	./main  -9k 	test51
	./main  -1k  	test61
	./main  -k  	test71
	./main  -9k 	test81
	./main  -1k 	test91
	echo "end compr"
	#разархивация
	./main -dc test01.gz >> test02
	./main -dc test11.gz >> test12
	./main -dc test21.gz >> test22
	./main -dc test31.gz >> test32
	./main -dc test41.gz >> test42
	./main -dc test51.gz >> test52
	./main -dc test61.gz >> test62
	./main -dc test71.gz >> test72
	./main -dc test81.gz >> test82
	./main -dc test91.gz >> test92
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
	cp "test01" "test02"
	cp "test11" "test12"
	cp "test21" "test22"
	cp "test31" "test32"
	cp "test41" "test42"
	cp "test51" "test52"
	cp "test61" "test62"
	cp "test71" "test72"
	cp "test81" "test82"
	cp "test91" "test92"
	echo "end cp"
	./main -1 test02
	./main 	  test12
	./main -9 test22
	./main -1 test32
	./main 	  test42
	./main -9 test52
	./main -1 test62
	./main    test72
	./main -9 test82
	./main -1 test92
	echo "end compr"
	#разархивация
	if [ "$key" -eq 1 ]
	then
		./main -dc test02.gz >> test02
		./main -dc test12.gz >> test12
		./main -dc test22.gz >> test22
		./main -dc test32.gz >> test32
		./main -dc test42.gz >> test42
		./main -dc test52.gz >> test52
		./main -dc test62.gz >> test62
		./main -dc test72.gz >> test72
		./main -dc test82.gz >> test82
		./main -dc test92.gz >> test92
	else
		./main -d test02.gz 
		./main -d test12.gz 
		./main -d test22.gz 
		./main -d test32.gz 
		./main -d test42.gz 
		./main -d test52.gz
		./main -d test62.gz
		./main -d test72.gz
		./main -d test82.gz
		./main -d test92.gz
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
	rm "test01" "test02" "test11" "test12" "test21" "test22" "test31" "test32" "test41" "test42" "test51" "test52" "test61" "test62" "test71" "test72" "test81" "test82" "test91" "test92"
	rm "test02.gz" "test12.gz" "test22.gz" "test32.gz" "test42.gz" "test52.gz" "test62.gz" "test72.gz" "test82.gz" "test92.gz" "test01.gz" "test11.gz" "test21.gz" "test31.gz" "test41.gz" "test51.gz" "test61.gz" "test71.gz" "test81.gz" "test91.gz"
fi
