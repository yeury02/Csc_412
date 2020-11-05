#include "program.h"
#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include<sys/wait.h>
#include <sys/types.h>

std::vector<std::string> splitString(std::string line)
{
    // make vector
    std::vector<std::string> myVector;
    // to keep track of each word in line
    std::string wordTracker = "";
    for (int i = 0; i < line.length(); i++)
    {
        if (line[i] == ' ')
        {
            myVector.push_back(wordTracker);
            wordTracker = "";
        }
        // handles window line and line ending
        else if (line[i] != '\n' && line[i] != '\r')
        {
            wordTracker += line[i];
        }
    }
    // this is necessary because it takes care of the last string of "line"
    myVector.push_back(wordTracker);

    return myVector;
}


int childProcess(std::string line, std::string applicationFolder, std::string dataFolder, std::string outputFolder, std::string jobFile)
{
    int result = -1;
    // get the command of each line
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

        result = execlp(
			pathToFlip.c_str(),		// path to executable
			"flipH",				// name of command (what will appear as argv[0])
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

        result = execlp(
			pathToFlip.c_str(),		// path to executable
			"flipV",				// name of command (what will appear as argv[0])
			pathToImage.c_str(),
			NULL
		);
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

        result = execlp(
			pathToFlip.c_str(),		// path to executable
			"gray",				    // name of command (what will appear as argv[0])
			pathToImage.c_str(),
			NULL
		);  
    }

    // still need to figure out a way to get each individual value from crop
    else if (command.compare("crop") == 0 )
    {   
        std::vector<std::string> myVector = splitString(line);
        // we need to exec --> execlp

        //	 build path to executable                           / + \0 + flipV
		std::string pathToFlip = applicationFolder + "/crop"; 

        //	get name of image, x, y, w, h 
		std::string imageName = myVector[1];
        //std::cout << "image name: " + imageName << std::endl;

        std::string x = myVector[2];
        //std::cout << "Crop X value: " + x << std::endl;
        std::string y = myVector[3];
        //std::cout << "Crop y value: " + y << std::endl;
        std::string w = myVector[4];
        //std::cout << "Crop w value: " + w << std::endl;
        std::string h = myVector[5];
        //std::cout << "Crop z value: " + z << std::endl;

        //build path to image                                                / + \0 
		std::string pathToImage = dataFolder + "/" + imageName;     

        result = execlp(
			pathToFlip.c_str(),		// path to executable
			"crop",				    // name of command (what will appear as argv[0])
            pathToImage.c_str(),
			x.c_str(),
            y.c_str(),
            w.c_str(),
            h.c_str(),
			NULL
		);
    }

    // command must be "rotate"
    else
    {
        std::vector<std::string> myVector = splitString(line);

        std::string rotationMoves = myVector[1];
        std::string imageName = myVector[2];

         //	 build path to executable                           / + \0 + flipV
		std::string pathToFlip = applicationFolder + "/rotate"; 

        //build path to image                                                / + \0 
		std::string pathToImage = dataFolder + "/" + imageName;

        std::cout << rotationMoves << " " << imageName << " " << pathToFlip << " " << pathToImage << std::endl;

        result = execlp(
			pathToFlip.c_str(),		// path to executable
			"rotate",				    // name of command (what will appear as argv[0])
            rotationMoves.c_str(),
			pathToImage.c_str(),
			NULL
		);  
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
    // to store each line of file
    std::string line;
    // get each line of file, and make sure I am not looking at the command "end"
    while (std::getline(inFile, line) && line != "end")
    {   
        int p = fork();   // lab 05
		if (p==0)
        {
            std::cout << line << std::endl;
            int result = childProcess(line, applicationFolder, dataFolder, outputFolder, jobFile);
            break;
        }
        else if (p<0)
		{
			std::cout << "Something went wrong while creating a child" << std::endl;
			exit(1);
		}
        else
        {
            result.numTasksCompleted++;
        }
        
    }
    inFile.close();

    // takes argument a single string

    int status;
    waitpid(0, &status, 0);
    //	wait for child processes to terminate
	//		ok may change value if child process encountered problem

    return result;
}

