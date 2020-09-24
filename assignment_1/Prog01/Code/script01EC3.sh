#!/bin/bash

SRC="prog01.c"
CC=gcc
$CC $SRC -o $1
MAXARG=2

if [[ $#-1 -eq $MAXARG ]]
then
    ./$1 $2 $3
else
    echo "usage: ./script01EC3.sh arg1 arg2"
fi