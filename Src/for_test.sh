#!/bin/bash

#генерация тестовых файлов
./gen 1	>> "test01"
./gen 10 >> "test11"
./gen 100 >> "test21"
./gen 1000 >> "test31"
./gen 10000	>> "test41"
./gen 100000 >> "test51"
./gen 1000000 >> "test61"
echo "end gen"
#создание доп копий тестовых файлов
cp "test01" "test02"
cp "test11" "test12"
cp "test21" "test22"
cp "test31" "test32"
cp "test41" "test42"
cp "test51" "test52"
cp "test61" "test62"
echo "end cp"
#архивация
./main -1 test02
./main	  test12
./main -9 test22
./main -1 test32
./main 	  test42
./main -9 test52
./main -1 test62
echo "end compr"
#разархивация
./main -d test02.gz test12.gz test22.gz test32.gz test42.gz test52.gz test62.gz
#проверка совпадения результатов
./comparator
