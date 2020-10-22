#! /bin/bash
declare  -i  a=0
while [ $a -lt 100 ]
do
B=$(( a %  2 ))
   if [ $B -eq  0 ]
   then
	    echo $a
	fi
a=$a+1
done
