#!/bin/bash

#the number of arguments is not 2
if [ "$#" = "2" ]
    then
        dirPath=$1
        fileExt=$2
        lastDir=$(ls -1 $dirPath | grep '.*\.'$fileExt'$')
        # gives me a count of how many files it found
        filesFound=$(ls -l $dirPath | grep '.*\.'$fileExt'$' | wc -l)
        zeroFile="0"

        echo "Looking for files with extension .$fileExt in folder"
        echo "     $dirPath:"

        # checks if one more than one file
        if (( $filesFound > 1 ));
            then
                echo "$filesFound files found:"
            else
                echo "$filesFound file found:"
        fi

        if [ "$filesFound" != "$zeroFile" ]
            then
                # loops thru the files in that directories that match the extension
                for file in $lastDir;
                    do
                        echo "     $file"
                    done
        else 
            echo "No file found."
            
        fi
else
    echo "Usage: ./script02.sh <path> <extension>"
    exit
fi