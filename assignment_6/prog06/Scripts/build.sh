#!/bin/bash

# build version 1
cd ../Version1
g++ main.cpp gl_frontEnd.cpp -lGL -lglut -lpthread -o cell 

# build version 2
cd ../Version2
g++ main.cpp gl_frontEnd.cpp -lGL -lglut -lpthread -o cell 