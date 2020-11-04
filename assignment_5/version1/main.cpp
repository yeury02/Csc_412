#include "program.h"
#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char** argv) 
{
    std::string applicationFolder = argv[1];
	std::string dataFolder = argv[2];
	std::string outputFolder = argv[3];
	std::string jobFile = argv[4];

    // make sure no folder path ends with '/'
    if (applicationFolder.back() == '/') applicationFolder.pop_back();              
    if (dataFolder.back() == '/') dataFolder.pop_back();
    if (outputFolder.back() == '/') outputFolder.pop_back();

    // std::cout << applicationFolder << std::endl; 
    // std::cout << dataFolder << std::endl;
    // std::cout << outputFolder << std::endl;
    // std::cout << jobFile << std::endl;

    JobOutput result = processOneJobFile(applicationFolder, dataFolder, outputFolder, jobFile);

    return result.numTasksCompleted;
}