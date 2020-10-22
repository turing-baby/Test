#! /bin/bash
read -p "" n
m=$(bc <<< "sqrt($n)")
i=true
for k in `seq 2 $n`
do
    g=$( expr  $n  %  $k)
    if [ $g -eq  0 ]
    then
          i=false
          break
     fi
done
if [ $i == true -a $n -ne 1 ]
then
     echo "$n 是"
else
      echo "$n 不是"
fi

