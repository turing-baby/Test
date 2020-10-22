#! /bin/bash
read -p "请输入要打印正方形的边长：" n
for i in `seq 1 $n`
do
for((j=1;j<=$n;j++))
do
echo -n "* "
done
echo
done