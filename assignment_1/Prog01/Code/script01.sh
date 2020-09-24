#!/bin/bash

SRC="prog01.c"
CC=gcc
$CC $SRC -o $1

./$1
./$1 one 
./$1 one two
./$1 one two three
./$1 one two three four

 
