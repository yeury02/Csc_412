#!/bin/bash

# if the correct of arguments are not passed in
if [ "$#" = "2" ]
    then
        # variable to track the max index
        max_distributor=0
        # for every file in dataSets
        for file in $(ls "$1")
        do
            # command to find index 
            # got it from StackOverFlow
            proc_index=$(grep -o '^[0-9]*' "$1/$file")
            # checks which index is bigger
            if [ $proc_index -gt $max_distributor ];
            then    
                max_distributor=$proc_index
            fi
        done

    # got from the web
    # + 1 because distributors start with 0
    distributors=$( expr $max_distributor + 1 )

    # first argument is the number of child processes to be created
    ./progV1 $distributors $1 $2

# in case I do not have enough arguments
# this error message will be printed out
else
    echo "Usage: $0 <dataSets> <outputFile>" 
    exit
fi