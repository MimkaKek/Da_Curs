#!/bin/bash
#Руководство:
#1) для запуска вводите в терминал bash for_test.sh
#2) после первого echo можно вводить числа
# отрицательные числа удаляют сгенерированные ЭТИМ скиптом тесты
# 0 запускает проверку с этими же тестами
# положительные числа создают новые тестовые файлы с заданным числом символов

echo "Input somthing less then 0 for deleting test files, 0 for execution with same tests files or something else to resize test files"

read action

if [ "$action" -gt  0 ]
then
	rm "test01" "test02" "test11" "test12" "test21" "test22" "test31" "test32" "test41" "test42" "test51" "test52"
	rm "test02.gz" "test12.gz" "test22.gz" "test32.gz" "test42.gz" "test52.gz" 
	#генерация тестовых файлов
	./gen "$action"	>> "test01"
	./gen "$action" >> "test11"
	./gen "$action" >> "test21"
	./gen "$action" >> "test31"
	./gen "$action"	>> "test41"
	./gen "$action" >> "test51"
	echo "end gen"
	#создание доп копий тестовых файлов
	cp "test01" "test02"
	cp "test11" "test12"
	cp "test21" "test22"
	cp "test31" "test32"
	cp "test41" "test42"
	cp "test51" "test52"
	echo "end cp"
	#архивация
	./main -1 test02
	./main	  test12
	./main -9 test22
	./main -1 test32
	./main 	  test42
	./main -9 test52
	echo "end compr"
	#разархивация
	./main -d test02.gz 
	./main -d test12.gz 
	./main -d test22.gz 
	./main -d test32.gz 
	./main -d test42.gz 
	./main -d test52.gz
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
	cp "test01" "test02"
	cp "test11" "test12"
	cp "test21" "test22"
	cp "test31" "test32"
	cp "test41" "test42"
	cp "test51" "test52"
	echo "end cp"
	./main -1 test02
	./main	  test12
	./main -9 test22
	./main -1 test32
	./main 	  test42
	./main -9 test52
	echo "end compr"
	#разархивация
	./main -d test02.gz 
	./main -d test12.gz 
	./main -d test22.gz 
	./main -d test32.gz 
	./main -d test42.gz 
	./main -d test52.gz
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
	rm "test01" "test02" "test11" "test12" "test21" "test22" "test31" "test32" "test41" "test42" "test51" "test52"
	rm "test02.gz" "test12.gz" "test22.gz" "test32.gz" "test42.gz" "test52.gz"
fi
