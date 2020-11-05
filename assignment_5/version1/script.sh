#!/bin/bash
INCLUDE=-I../ImageLibrary/include

g++ -c -std=c++11 -fPIC $INCLUDE ../ImageLibrary/src/ImageIO_TGA.cpp -o ImageIO_TGA.o
g++ -c -std=c++11 -fPIC $INCLUDE ../ImageLibrary/src/ImageIO.cpp -o ImageIO.o
g++ -c -std=c++11 -fPIC $INCLUDE ../ImageLibrary/src/RasterImage.cpp -o RasterImage.o
g++ -std=c++11 -c -fPIC $INCLUDE ../ImageLibrary/src/utilities.cpp -o utilities.o
ar rcs libImageLibrary_static.a ImageIO_TGA.o ImageIO.o RasterImage.o utilities.o
g++ -shared -std=c++11 ImageIO_TGA.o ImageIO.o RasterImage.o utilities.o -o libImageLibrary_shared.so

# These programs depend on static library
# g++ -std=c++11 $INCLUDE ../ImageLibrary/applications/crop.cpp -o crop -lImageLibrary_static -L./
# g++ -std=c++11 $INCLUDE ../ImageLibrary/applications/rotate.cpp -o rotate -lImageLibrary_static -L./
# g++ -std=c++11 $INCLUDE ../ImageLibrary/applications/flipH.cpp -o flipH -lImageLibrary_static -L./
# g++ -std=c++11 $INCLUDE ../ImageLibrary/applications/flipV.cpp -o flipV -lImageLibrary_static -L./
# g++ -std=c++11 $INCLUDE ../ImageLibrary/applications/gray.cpp -o gray -lImageLibrary_static -L./

list="crop rotate flipH flipV gray" 

for file in $list 
do
    g++ -std=c++11 $INCLUDE ../ImageLibrary/applications/$file.cpp -o ${file}_static -lImageLibrary_static -L./
    g++ -std=c++11 $INCLUDE ../ImageLibrary/applications/$file.cpp -o ${file}_shared -lImageLibrary_shared -L./
    echo "built: $file"
done


# mv -v crop rotate flipH flipV gray ../output/