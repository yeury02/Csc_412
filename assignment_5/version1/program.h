#ifndef PROGRAM_H
#define PROGRAM_H
#include <string>
#include <vector>

struct JobOutput {
	bool keepGoing;	        //	no end encountered
	int	numTasksCompleted;
};

JobOutput processOneJobFile(std::string applicationFolder, std::string dataFolder, std::string outputFolder, std::string jobFile);
JobOutput childProcess(std::string line, std::string applicationFolder, std::string dataFolder, std::string outputFolder, std::string jobFile);
std::vector<std::string> splitString(std::string line);

#endif