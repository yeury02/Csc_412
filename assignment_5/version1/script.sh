#!/bin/bash
INCLUDE=-I../ImageLibrary/include

g++ -c -fPIC $INCLUDE ..ImageLibrary/src/ImageIO_TGA.cpp -o ImageIO_TGA.o
g++ -c -fPIC $INCLUDE ..ImageLibrary/src/ImageIO.cpp -o ImageIO.o
g++ -c -fPIC $INCLUDE ..ImageLibrary/src/RasterImage.cpp -o RasterImage.o
g++ -c -fPIC $INCLUDE ..ImageLibrary/src/utilities.cpp -o utilities.o
ar rcs libImageLibrary_static.a ImageIO_TGA.o ImageIO.o RasterImage.o utilities.o

g++ $INCLUDE ../ImageLibrary/applications/crop.cpp -o crop -libImageLibrary_static -L./
g++ $INCLUDE ../ImageLibrary/applications/rotate.cpp -o rotate -libImageLibrary_static -L./
g++ $INCLUDE ../ImageLibrary/applications/flipH.cpp -o flipH -libImageLibrary_static -L./
g++ $INCLUDE ../ImageLibrary/applications/flipV.cpp -o flipV -libImageLibrary_static -L./
g++ $INCLUDE ../ImageLibrary/applications/gray.cpp -o gray -libImageLibrary_static -L./