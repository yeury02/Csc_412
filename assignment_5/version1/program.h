#ifndef PROGRAM_H
#define PROGRAM_H
#include <string>

struct JobOutput {
	bool keepGoing;	        //	no end encountered
	int	numTasksCompleted;
};



int processOneJobFile(std::string filePath);

#endif