#include "program.h"
#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>

std::vector<std::string> splitString(std::string line)
{
    // make vector
    std::vector<std::string> myVector;
    std::string wordTracker = "";
    for (int i = 0; i < line.length(); i++)
    {
        if (line[i] == ' ')
        {
            myVector.push_back(wordTracker);
            wordTracker = "";
        }
        wordTracker += line[i];
    }
    // this is necessary because it takes care of the last string of "line"
    myVector.push_back(wordTracker);

    return myVector;
}


JobOutput childProcess(std::string line, std::string applicationFolder, std::string dataFolder, std::string outputFolder, std::string jobFile)
{
    JobOutput result;
    std::string command = line.substr(0, line.find(' '));

    if (command.compare("flipH") == 0 )
    {
        std::vector<std::string> myVector = splitString(line);
        // we need to exec --> execlp

        //	 build path to executable                           / + \0 + flipV
		std::string pathToFlip = applicationFolder + "/flipH"; 

        //	get name of image file from vector
		std::string imageName = myVector[1];
        // std::cout << "Image Name: ";
        // std::cout << imageName<< std::endl;

        //	 build path to image                                                / + \0 
		std::string pathToImage = dataFolder + "/" + imageName;
        // std::cout << "Path to Image: ";
        // std::cout << pathToImage<< std::endl;

        execlp(
			pathToFlip.c_str(),		// path to executable
			"program",				// name of command (what will appear as argv[0])
			pathToImage.c_str(),
			NULL
		);
    }

    else if (command.compare("flipV") == 0 )
    {
        std::vector<std::string> myVector = splitString(line);
        // we need to exec --> execlp

        //	 build path to executable                           / + \0 + flipV
		std::string pathToFlip = applicationFolder + "/flipV"; 

        //	get name of image file from vector
		std::string imageName = myVector[1];

        //	 build path to image                                                / + \0 
		std::string pathToImage = dataFolder + "/" + imageName;  
    }

    else if (command.compare("gray") == 0 )
    {
        std::vector<std::string> myVector = splitString(line);
        // we need to exec --> execlp

        //	 build path to executable                           / + \0 + flipV
		std::string pathToFlip = applicationFolder + "/gray"; 

        //	get name of image file from vector
		std::string imageName = myVector[1];

        //	 build path to image                                                / + \0 
		std::string pathToImage = dataFolder + "/" + imageName;       
    }

    // still need to figure out a way to get each individual value from crop
    else if (command.compare("crop") == 0 )
    {   
        std::vector<std::string> myVector = splitString(line);
        // we need to exec --> execlp

        //	 build path to executable                           / + \0 + flipV
		std::string pathToFlip = applicationFolder + "/flipV"; 

        //	get name of image, x, y, w, h 
		std::string imageName = myVector[1];
        std::cout << "image name: " + imageName << std::endl;

        std::string x = myVector[2];
        std::cout << "Crop X value: " + x << std::endl;
        std::string y = myVector[3];
        std::cout << "Crop y value: " + y << std::endl;
        std::string w = myVector[4];
        std::cout << "Crop w value: " + w << std::endl;
        std::string z = myVector[5];
        std::cout << "Crop z value: " + z << std::endl;

        //	 build path to image                                                / + \0 
		// std::string pathToImage = dataFolder + "/" + imageName;     
    }

    // command must be "rotate"
    else
    {
        std::vector<std::string> myVector = splitString(line);

        std::string rotationMoves = myVector[1];
        std::string imageName = myVector[2];

    }

    return result;
}

JobOutput processOneJobFile(std::string applicationFolder, std::string dataFolder, std::string outputFolder, std::string jobFile)
{
    // instance of struct
    JobOutput result;

    // create an instance of the ifstream class
    std::ifstream inFile;
    inFile.open(jobFile);

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
        //std::cout << line << std::endl;
        int p = fork();   // lab 05
		if (p==0)
        {
            childProcess(line, applicationFolder, dataFolder, outputFolder, jobFile);
        }
        else if (p<0)
		{
			std::cout << "Something went wrong while creating a child" << std::endl;
			exit(1);
		}
    }
    inFile.close();

    //	wait for child processes to terminate
	//		ok may change value if child process encountered problem

    return result;
}

