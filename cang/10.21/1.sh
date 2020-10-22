#！/bin/bash
A=喻
B=英建
declare -i  C=13  #declare -i ，前缀意思为定义整形数据
declare -i  D=12
declare -i  E=$C+$D
echo  $A$B
echo   $E
H=$1$2$3$4$5$6$7$8$9   #$1代表   ./运行脚本加入的参数  $1 == argv[1]
echo   A=$1$2$3$4$5$6$7$8$9
echo   $H
echo `ls`   #反引号，tab键上面，将输出的字符串已命令形式拼接打印出来
echo  '$A$B `ls` '  #单引号内的东西不能调用，只以字符串输出
case $1 in 
1)echo "星期一";;
2)echo "星期二";;
3)echo "星期三";;
4)echo "星期四";;
5)echo "星期五";;
6)echo "星期六";;
7)echo "星期日";;
*)echo "输入错误"
esac
if [ $1 = 1 ]
then
echo "星期一"
elif [ $1 = 2 ]
then
echo "星期二"
elif [ $1 = 3 ]
then
echo "星期三"
elif [ $1 = 4 ]
then
echo "星期四"
elif [ $1 = 5 ]
then
echo "星期五"
elif [ $1 = 6 ]
then
echo "星期六"
elif [ $1 = 7 ]
then
echo "星期日"
else
echo "输入错误"
fi
