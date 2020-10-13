#!/bin/bash

# if not the right amount of arguments are passed in
if [ "$#" != "4" ]
then
	echo "usage: ./run.sh <C-Program> <Images> <Patterns> <Output>"
	exit
fi

# made a directory
mkdir copies
pats=$(ls $3)
echo "Starts"

for d in $pats;
do
    bytes=$(hexdump -C $3/$d | grep -o " [0-9a-z][0-9a-z]")
	for byte in $bytes;do
		if [ $byte != "0d" ]
		then
			printf '\x'$byte'' >> copies/$d
		fi
	done
	$(cp copies/$d $3/$d)
done

rm -rf copies
gcc -g $1.c -o $1
for pat in $pats;do
	./$1 $3/$pat $2 $4
done