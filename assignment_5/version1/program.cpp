#include "program.h"
#include <iostream>
#include <fstream>
#include <string>

int processOneJobFile(std::string filePath)
{
    // create an instance of the ifstream class
    std::ifstream inFile;
    inFile.open(filePath);

    // check for Error
    // check if file exist
    if (inFile.fail())
    {
        std::cerr << "Error opening file" << std::endl;
        exit(1); // terminate with error
    }

    std::string line;
    while (std::getline(inFile, line) && line != "end")
    {   
        // get the first command "I am the best" 
        std::cout << line << std::endl;

        // extract the first word of each line (command)
        std::string command = line.substr(0, line.find(' '));
        std::cout << command << std::endl;


        // make this a function
        if (command.compare("flipH") == 0 )
	    {
            std::cout << "True" << std::endl;      
        }

        else if (command.compare("flipV") == 0 )
	    {
            std::cout << "True" << std::endl;      
        }

        else if (command.compare("gray") == 0 )
	    {
            std::cout << "True" << std::endl;      
        }

        else if (command.compare("crop") == 0 )
	    {
            std::cout << "True" << std::endl;      
        }

        // command must be "rotate"
        else
        {
            std::cout << "True" << std::endl; 
        }



    }
    inFile.close();
    // if it opens properly


    // go until you find the string "end"

    // look at the first task... Ex) rotate, flipH,etc
    return 0;
}

