#!/bin/bash
declare -i d
func()
{
	d=$1$2$3
	return $d
}
func  $1 $2  $3
echo $?
