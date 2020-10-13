#!/bin/bash

# TA Chris helped me complete this assignment during his office hours
# he said to mention this on top of my bash script

# defines all my arguments
EXEC=$1
IMG=$2
PAT=$3
OUTPUT=$4

# as long as I have all the correct arguments
if [ "$#" = "4" ]
	then
		# list pattern files
		Patterns=$(ls $PAT)
		# made a directory
		# got this idea from TA Chris
		# create a temporary directory to make modifications
		mkdir temp

		# for every file
		for pat in $Patterns;
		do
			# found this line online!
			tmp=$(hexdump -C $PAT/$pat | grep -o " [0-9a-z][0-9a-z]")
			for t in $tmp;
			do	
				# checking
				if [ $t != "0d" ]
				then
					# output
					printf '\x'$t'' >> temp/$pat
				fi
			done
			# got this from online as well!
			$(cp temp/$pat $PAT/$pat)
		done

	rm -rf temp
	# build program
	gcc -g $EXEC.c -o EXEC
	for m in $Patterns;
	do
		# execute it
		./$EXEC $PAT/$pat $IMG $OUTPUT
	done

# in case I do not have enough arguments
# this error message will be printed out
else
	echo "usage: $0 <C-Program-Executable> <Images> <Patterns> <Output>"
fi