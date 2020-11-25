//
//  main.c
//  Cellular Automaton
//
//  Created by Jean-Yves Hervé on 2018-04-09.
//	C++ version 2020-11-06

 /*-------------------------------------------------------------------------+
 |	A graphic front end for a grid+state simulation.						|
 |																			|
 |	This application simply creates a glut window with a pane to display	|
 |	a colored grid and the other to display some state information.			|
 |	Sets up callback functions to handle menu, mouse and keyboard events.	|
 |	Normally, you shouldn't have to touch anything in this code, unless		|
 |	you want to change some of the things displayed, add menus, etc.		|
 |	Only mess with this after everything else works and making a backup		|
 |	copy of your project.  OpenGL & glut are tricky and it's really easy	|
 |	to break everything with a single line of code.							|
 |																			|
 |	Current keyboard controls:												|
 |																			|
 |		- 'ESC' --> exit the application									|
 |		- space bar --> resets the grid										|
 |																			|
 |		- 'c' --> toggle color mode on/off									|
 |		- 'b' --> toggles color mode off/on									|
 |		- 'l' --> toggles on/off grid line rendering						|
 |																			|
 |		- '+' --> increase simulation speed									|
 |		- '-' --> reduce simulation speed									|
 |																			|
 |		- '1' --> apply Rule 1 (Conway's classical Game of Life: B3/S23)	|
 |		- '2' --> apply Rule 2 (Coral: B3/S45678)							|
 |		- '3' --> apply Rule 3 (Amoeba: B357/S1358)							|
 |		- '4' --> apply Rule 4 (Maze: B3/S12345)							|
 |																			|
 +-------------------------------------------------------------------------*/

#include <iostream>
#include <ctime>
#include <unistd.h>
#include <pthread.h>
#include <sstream>
//
#include "gl_frontEnd.h"

using namespace std;

#if 0
//==================================================================================
#pragma mark -
#pragma mark Custom data types
//==================================================================================
#endif

typedef struct ThreadInfo
{
	//	you probably want these
	pthread_t threadID;
	int threadIndex;
	//
	//	whatever other input or output data may be needed
	unsigned int startRow, endRow;
	pthread_mutex_t lock;
} ThreadInfo;


#if 0
//==================================================================================
#pragma mark -
#pragma mark Function prototypes
//==================================================================================
#endif

void displayGridPane(void);
void displayStatePane(void);
void initializeApplication(void);
void cleanupAndquit(void);
void* createThreadsFunc(void* arg);
void* computationThreadFunc(void* arg);
void swapGrids(void);
void distributeRows(void);
unsigned int cellNewState(unsigned int i, unsigned int j);


//==================================================================================
//	Precompiler #define to let us specify how things should be handled at the
//	border of the frame
//==================================================================================

#if 0
//==================================================================================
#pragma mark -
#pragma mark Simulation mode:  behavior at edges of frame
//==================================================================================
#endif

#define FRAME_DEAD		0	//	cell borders are kept dead
#define FRAME_RANDOM	1	//	new random values are generated at each generation
#define FRAME_CLIPPED	2	//	same rule as elsewhere, with clipping to stay within bounds
#define FRAME_WRAP		3	//	same rule as elsewhere, with wrapping around at edges

//	Pick one value for FRAME_BEHAVIOR
#define FRAME_BEHAVIOR	FRAME_DEAD

#if 0
//==================================================================================
#pragma mark -
#pragma mark Application-level global variables
//==================================================================================
#endif

//	Don't touch
extern int gMainWindow, gSubwindow[2];

//	The state grid and its dimensions.  We use two copies of the grid:
//		- currentGrid is the one displayed in the graphic front end
//		- nextGrid is the grid that stores the next generation of cell
//			states, as computed by our threads.
unsigned int* currentGrid;
unsigned int* nextGrid;
unsigned int** currentGrid2D;
unsigned int** nextGrid2D;

//	Piece of advice, whenever you do a grid-based (e.g. image processing),
//	implementastion, you should always try to run your code with a
//	non-square grid to spot accidental row-col inversion bugs.
//	When this is possible, of course (e.g. makes no sense for a chess program).

//const unsigned int NUM_ROWS = 400, NUM_COLS = 420;


// these are my arguments
unsigned int numCols, numRows, maxNumThreads;
unsigned int sleepTime = 200000;
unsigned int threatCount = 0;
// to prevent other threads from altering the count
pthread_mutex_t lockCount;
bool run = true;

ThreadInfo* info;

//	the number of live computation threads (that haven't terminated yet)
unsigned short numLiveThreads = 0;

unsigned int rule = GAME_OF_LIFE_RULE;

unsigned int colorMode = 0;

unsigned int generation = 0;


//------------------------------
//	Threads and synchronization
//	Reminder of all declarations and function calls
//------------------------------
//pthread_mutex_t myLock;
//pthread_mutex_init(&myLock, NULL);
//int err = pthread_create(pthread_t*, NULL, threadFunc, ThreadInfo*);
//int pthread_join(pthread_t , void**);
//pthread_mutex_lock(&myLock);
//pthread_mutex_unlock(&myLock);

#if 0
//==================================================================================
#pragma mark -
#pragma mark Computation functions
//==================================================================================
#endif


//------------------------------------------------------------------------
//	You shouldn't have to change anything in the main function
//------------------------------------------------------------------------
int main(int argc, const char* argv[])
{
	// error checking for not enough arguments
	if ( argc != 4 )
	{
		cout << "You must pass 3 arguments. Number of rows, columns, and maximum number of threads" << endl;
	}
	// right amount of arguments are passed in
	else 
	{
		// get arguments from command line
		numCols = stoul(argv[1]); // number of columns
		numRows = stoul(argv[2]); // number of rows
		maxNumThreads = stoul(argv[3]); // number of threads

		//cout << numCols << " " << numRows << " " << maxNumThreads << endl;
		// check all the arguments are positive intergers
		// cols and rows are bigger than 5
		if (numCols > 5 && numRows > 5 && maxNumThreads >= 0 )
		{
			if ( maxNumThreads < numRows)
			{
				cout << "met all the conditions" << endl;
				//	This takes care of initializing glut and the GUI.
				//	You shouldn’t have to touch this
				initializeFrontEnd(argc, argv, displayGridPane, displayStatePane);
				
				//	Now we can do application-level initialization
				initializeApplication();

				// create main threat simulation
				pthread_t simulation;
				pthread_create(&simulation, NULL, createThreadsFunc, NULL);

				//	Now would be the place & time to create mutex locks and threads

				//	Now we enter the main loop of the program and to a large extend
				//	"lose control" over its execution.  The callback functions that 
				//	we set up earlier will be called when the corresponding event
				//	occurs
				glutMainLoop();
				
				//	In fact this code is never reached because we only leave the glut main
				//	loop through an exit call.
				//	Free allocated resource before leaving (not absolutely needed, but
				//	just nicer.  Also, if you crash there, you know something is wrong
				//	in your code.
				free(currentGrid2D);
				free(currentGrid);
					
				//	This will never be executed (the exit point will be in one of the
				//	call back functions).
				return 0;
			}
			else
			{
				cout << "number of rows is smaller than or equal to the maximum number of threads. Not Acceptable!" << endl;
			}
		}
		else 
		{
			cout << "All values must be positive. columns and rows must be greater than 5" << endl;
		}
	}
}

//==================================================================================
//
//	This is a part that you have to edit and add to.
//
//==================================================================================

void initializeApplication(void)
{
    //--------------------
    //  Allocate 1D grids
    //--------------------
    currentGrid = new unsigned int[numRows*numCols];
    nextGrid = new unsigned int[numRows*numCols];

    //---------------------------------------------
    //  Scaffold 2D arrays on top of the 1D arrays
    //---------------------------------------------
    currentGrid2D = new unsigned int*[numRows];
    nextGrid2D = new unsigned int*[numRows];
    currentGrid2D[0] = currentGrid;
    nextGrid2D[0] = nextGrid;
    for (unsigned int i=1; i<numRows; i++)
    {
        currentGrid2D[i] = currentGrid2D[i-1] + numCols;
        nextGrid2D[i] = nextGrid2D[i-1] + numCols;
    }
	
	//---------------------------------------------------------------
	//	All the code below to be replaced/removed
	//	I initialize the grid's pixels to have something to look at
	//---------------------------------------------------------------
	//	Yes, I am using the C random generator after ranting in class that the C random
	//	generator was junk.  Here I am not using it to produce "serious" data (as in a
	//	simulation), only some color, in meant-to-be-thrown-away code
	
	//	seed the pseudo-random generator
	srand((unsigned int) time(NULL));
	
	resetGrid();
}

//---------------------------------------------------------------------
//	You will need to implement/modify the two functions below
//---------------------------------------------------------------------

void* createThreadsFunc(void* arg)
{
	(void) arg;

	pthread_mutex_init(&lockCount,nullptr);
	distributeRows();
	for (int k = 0; k < maxNumThreads; k++)
	{
        pthread_mutex_init(&info[k].lock,nullptr);
        pthread_mutex_lock(&(info[k].lock)); // acquire
    }
    for (int k = 0; k < maxNumThreads; k++)
	{
        pthread_create(&(info[k].threadID),nullptr,computationThreadFunc,info+k);
	}
	for (int k = 0; k < maxNumThreads; k++)
	{
		pthread_mutex_unlock(&(info[k]).lock);
	}
	return NULL;
}


// //	I have decided to go for maximum modularity and readability, at the
// //	cost of some performance.  This may seem contradictory with the
// //	very purpose of multi-threading our application.  I won't deny it.
// //	My justification here is that this is very much an educational exercise,
// //	my objective being for you to understand and master the mechanisms of
// //	multithreading and synchronization with mutex.  After you get there,
// //	you can micro-optimi1ze your synchronized multithreaded apps to your
// //	heart's content.
// void oneGeneration(void)
// {
// 	distributeRows();
// 	// create threads
// 	for (int k=0; k < maxNumThreads; k++)
// 	{
// 		//	initialize ThreadInfo struct for thread k
// 		pthread_create(&(info[k].threadID),nullptr,computationThreadFunc,info+k);
// 	}
// 	//	wait for threads to finish (join)
// 	for (int k=0; k < maxNumThreads; k++)
// 	{
// 		//	create thread k
// 		pthread_join(info[k].threadID,nullptr);
// 	}
// 	// free memory once everythig is done
// 	delete []info;
// }

void* computationThreadFunc(void* arg)
{
	//	cast argument to proper type
	ThreadInfo* data = static_cast<ThreadInfo*>(arg);
	// C-style
	//ThreadInfo* data = (ThreadInfo*) arg;
	pthread_mutex_lock(&(data->lock)); // acquire
	{
		stringstream sstr;
		sstr << "\t+ Thread " << data->threadIndex << " launched\n";
		sstr << "\t ----> lock acquired\n";
		cout << sstr.str() << flush;
	}

	int numRowsProcessed = data->endRow - data->startRow;
	//bool keepGoing = true;
	while (run)
	{
		for (unsigned int i = data->startRow; i <= data->endRow; i++)
		{
			for (unsigned int j=0; j<numCols; j++)
			{
				unsigned int newState = cellNewState(i, j);

				//	In black and white mode, only alive/dead matters
				//	Dead is dead in any mode
				if (colorMode == 0 || newState == 0)
				{
					nextGrid2D[i][j] = newState;
				}
				//	in color mode, color reflext the "age" of a live cell
				else
				{
					//	Any cell that has not yet reached the "very old cell"
					//	stage simply got one generation older
					if (currentGrid2D[i][j] < NB_COLORS-1)
						nextGrid2D[i][j] = currentGrid2D[i][j] + 1;
					//	An old cell remains old until it dies
					else
						nextGrid2D[i][j] = currentGrid2D[i][j];
				}
			}
		}
		threatCount++;
		// if on last thread, swap grids
		if (threatCount == numRowsProcessed)
		{
			stringstream sstr;
			// test
			sstr << "\t |\t count is " << threatCount << "\n";
			threatCount = 0;
			swapGrids();
			generation++;
			pthread_mutex_unlock(&(data->lock)); // release locks
			// test
			sstr << "\t | last thread " << data->threadIndex << "\n";
			sstr << "\t --<- lock released\n";
			//threatCount << sstr.str() << flush;
		}
		else 
		{
			usleep(sleepTime);
			pthread_mutex_unlock(&lockCount); // release
		}
	}
	return NULL;
}

void distributeRows (void)
{
	int rowsToDistribute = numRows / maxNumThreads;
	int remainder = numRows / maxNumThreads;
	int start = 0;
	int end = rowsToDistribute - 1;

	// global ThreadInfo + number of threads
	info = new ThreadInfo [maxNumThreads];
	for (int k = 0; k < maxNumThreads; k++)
	{
		//	initialize ThreadInfo struct for thread k
        info[k].threadIndex = k;
        // define the start and end rows
        if (k < remainder)
        {
            end++;
        }
        info[k].startRow = start;
        info[k].endRow = end;
        start = end + 1;
        end = end + rowsToDistribute;
	}
}

//	This is the function that determines how a cell update its state
//	based on that of its neighbors.
//	1. As repeated in the comments below, this is a "didactic" implementation,
//	rather than one optimized for speed.  In particular, I refer explicitly
//	to the S/B elements of the "rule" in place.
//	2. This implentation allows for different behaviors along the edges of the
//	grid (no change, keep border dead, clipping, wrap around). All these
//	variants are used for simulations in research applications.
unsigned int cellNewState(unsigned int i, unsigned int j)
{
	//	First count the number of neighbors that are alive
	//----------------------------------------------------
	//	Again, this implementation makes no pretense at being the most efficient.
	//	I am just trying to keep things modular and somewhat readable
	int count = 0;

	//	Away from the border, we simply count how many among the cell's
	//	eight neighbors are alive (cell state > 0)
	if (i>0 && i<numRows-1 && j>0 && j<numCols-1)
	{
		//	remember that in C, (x == val) is either 1 or 0
		count = (currentGrid2D[i-1][j-1] != 0) +
				(currentGrid2D[i-1][j] != 0) +
				(currentGrid2D[i-1][j+1] != 0)  +
				(currentGrid2D[i][j-1] != 0)  +
				(currentGrid2D[i][j+1] != 0)  +
				(currentGrid2D[i+1][j-1] != 0)  +
				(currentGrid2D[i+1][j] != 0)  +
				(currentGrid2D[i+1][j+1] != 0);
	}
	//	on the border of the frame...
	else
	{
		#if FRAME_BEHAVIOR == FRAME_DEAD
		
			//	Hack to force death of a cell
			count = -1;
		
		#elif FRAME_BEHAVIOR == FRAME_RANDOM
		
			count = rand() % 9;
		
		#elif FRAME_BEHAVIOR == FRAME_CLIPPED
	
			if (i>0)
			{
				if (j>0 && currentGrid2D[i-1][j-1] != 0)
					count++;
				if (currentGrid2D[i-1][j] != 0)
					count++;
				if (j<numCols-1 && currentGrid2D[i-1][j+1] != 0)
					count++;
			}

			if (j>0 && currentGrid2D[i][j-1] != 0)
				count++;
			if (j<numCols-1 && currentGrid2D[i][j+1] != 0)
				count++;

			if (i<numRows-1)
			{
				if (j>0 && currentGrid2D[i+1][j-1] != 0)
					count++;
				if (currentGrid2D[i+1][j] != 0)
					count++;
				if (j<numCols-1 && currentGrid2D[i+1][j+1] != 0)
					count++;
			}
			
	
		#elif FRAME_BEHAVIOR == FRAME_WRAPPED
	
			unsigned int 	iM1 = (i+numRows-1)%numRows,
							iP1 = (i+1)%numRows,
							jM1 = (j+numCols-1)%numCols,
							jP1 = (j+1)%numCols;
			count = currentGrid2D[iM1][jM1] != 0 +
					currentGrid2D[iM1][j] != 0 +
					currentGrid2D[iM1][jP1] != 0  +
					currentGrid2D[i][jM1] != 0  +
					currentGrid2D[i][jP1] != 0  +
					currentGrid2D[iP1][jM1] != 0  +
					currentGrid2D[iP1][j] != 0  +
					currentGrid2D[iP1][jP1] != 0 ;

		#else
			#error undefined frame behavior
		#endif
		
	}	//	end of else case (on border)
	
	//	Next apply the cellular automaton rule
	//----------------------------------------------------
	//	by default, the grid square is going to be empty/dead
	unsigned int newState = 0;
	
	//	unless....
	
	switch (rule)
	{
		//	Rule 1 (Conway's classical Game of Life: B3/S23)
		case GAME_OF_LIFE_RULE:

			//	if the cell is currently occupied by a live cell, look at "Stay alive rule"
			if (currentGrid2D[i][j] != 0)
			{
				if (count == 3 || count == 2)
					newState = 1;
			}
			//	if the grid square is currently empty, look at the "Birth of a new cell" rule
			else
			{
				if (count == 3)
					newState = 1;
			}
			break;
	
		//	Rule 2 (Coral Growth: B3/S45678)
		case CORAL_GROWTH_RULE:

			//	if the cell is currently occupied by a live cell, look at "Stay alive rule"
			if (currentGrid2D[i][j] != 0)
			{
				if (count > 3)
					newState = 1;
			}
			//	if the grid square is currently empty, look at the "Birth of a new cell" rule
			else
			{
				if (count == 3)
					newState = 1;
			}
			break;
			
		//	Rule 3 (Amoeba: B357/S1358)
		case AMOEBA_RULE:

			//	if the cell is currently occupied by a live cell, look at "Stay alive rule"
			if (currentGrid2D[i][j] != 0)
			{
				if (count == 1 || count == 3 || count == 5 || count == 8)
					newState = 1;
			}
			//	if the grid square is currently empty, look at the "Birth of a new cell" rule
			else
			{
				if (count == 3 || count == 5 || count == 7)
					newState = 1;
			}
			break;
		
		//	Rule 4 (Maze: B3/S12345)							|
		case MAZE_RULE:

			//	if the cell is currently occupied by a live cell, look at "Stay alive rule"
			if (currentGrid2D[i][j] != 0)
			{
				if (count >= 1 && count <= 5)
					newState = 1;
			}
			//	if the grid square is currently empty, look at the "Birth of a new cell" rule
			else
			{
				if (count == 3)
					newState = 1;
			}
			break;

			break;
		
		default:
			cout << "Invalid rule number" << endl;
			exit(5);
	}

	return newState;
}

void cleanupAndquit(void)
{
	//	join the threads
	cout << "\t simulation terminated" << endl;
	run = false;
	for (int k = 0; k < maxNumThreads; k++)
	{
        pthread_join(info[k].threadID,nullptr);
        stringstream sstr;

		cout << sstr.str() << flush;
	}
	//sstr << threatCount << "threads joined\n";
    delete []info;
	//	In fact this code is never reached because we only leave the glut main
	//	loop through an exit call.
	//	Free allocated resource before leaving (not absolutely needed, but
	//	just nicer.  Also, if you crash there, you know something is wrong
	//	in your code.
	free(currentGrid2D);
	free(currentGrid);

	cout << "end." << endl;
	
	//	free the grids

	exit(0);
}



#if 0
#pragma mark -
#pragma mark GUI functions
#endif

//==================================================================================
//	GUI functions
//	These are the functions that tie the simulation with the rendering.
//	Some parts are "don't touch."  Other parts need your intervention
//	to make sure that access to critical section is properly synchronized
//==================================================================================


void displayGridPane(void)
{
	//	This is OpenGL/glut magic.  Don't touch
	glutSetWindow(gSubwindow[GRID_PANE]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//---------------------------------------------------------
	//	This is the call that makes OpenGL render the grid.
	//
	//---------------------------------------------------------
	drawGrid(currentGrid2D, numRows, numCols);
	
	//	This is OpenGL/glut magic.  Don't touch
	glutSwapBuffers();
	
	glutSetWindow(gMainWindow);
}

void displayStatePane(void)
{
	//	This is OpenGL/glut magic.  Don't touch
	glutSetWindow(gSubwindow[STATE_PANE]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//---------------------------------------------------------
	//	This is the call that makes OpenGL render information
	//	about the state of the simulation.
	//	You may want to add stuff to display
	//---------------------------------------------------------
	drawState(numLiveThreads);
	
	//	This is OpenGL/glut magic.  Don't touch
	glutSwapBuffers();
	glutSetWindow(gMainWindow);
}


//	This callback function is called when a keyboard event occurs
//
void myKeyboardFunc(unsigned char c, int x, int y)
{
	(void) x; (void) y;
	
	switch (c)
	{
		//	'ESC' --> exit the application
		case 27:
			cleanupAndquit();
			break;

		//	spacebar --> resets the grid
		case ' ':
			resetGrid();
			break;

		//	'+' --> increase simulation speed
		case '+':
			break;

		//	'-' --> reduce simulation speed
		case '-':
			break;

		//	'1' --> apply Rule 1 (Game of Life: B23/S3)
		case '1':
			rule = GAME_OF_LIFE_RULE;
			break;

		//	'2' --> apply Rule 2 (Coral: B3_S45678)
		case '2':
			rule = CORAL_GROWTH_RULE;
			break;

		//	'3' --> apply Rule 3 (Amoeba: B357/S1358)
		case '3':
			rule = AMOEBA_RULE;
			break;

		//	'4' --> apply Rule 4 (Maze: B3/S12345)
		case '4':
			rule = MAZE_RULE;
			break;

		//	'c' --> toggles on/off color mode
		//	'b' --> toggles off/on color mode
		case 'c':
		case 'b':
			colorMode = !colorMode;
			break;

		//	'l' --> toggles on/off grid line rendering
		case 'l':
			toggleDrawGridLines();
			break;
			//	',' --> slowdown
			case ',':
				sleepTime = 11*sleepTime/10;
				break;
			//	'.' --> speedup
			case '.':
				if (sleepTime > 10000)
					sleepTime = 9*sleepTime/10;

		default:
			break;
	}
	
	glutSetWindow(gMainWindow);
	glutPostRedisplay();
}

void myTimerFunc(int value)
{
	//	value not used.  Warning suppression
	(void) value;
	
    //  possibly I do something to update the state information displayed
    //	in the "state" pane
	
	//==============================================
	//	This call must **DEFINITELY** go away.
	//	(when you add proper threading)
	//==============================================
    // threadFunc(NULL);
	
	//	This is not the way it should be done, but it seems that Apple is
	//	not happy with having marked glut as deprecated.  They are doing
	//	things to make it break
    //glutPostRedisplay();
    myDisplayFunc();
    
	//	And finally I perform the rendering
	glutTimerFunc(15, myTimerFunc, 0);
}

//---------------------------------------------------------------------
//	You shouldn't need to touch the functions below
//---------------------------------------------------------------------

void resetGrid(void)
{
	for (unsigned int i=0; i<numRows; i++)
	{
		for (unsigned int j=0; j<numCols; j++)
		{
			nextGrid2D[i][j] = rand() % 2;
		}
	}
	swapGrids();
}

//	This function swaps the current and next grids, as well as their
//	companion 2D grid.  Note that we only swap the "top" layer of
//	the 2D grids.
void swapGrids(void)
{
	//	swap grids
	unsigned int* tempGrid;
	unsigned int** tempGrid2D;
	
	tempGrid = currentGrid;
	currentGrid = nextGrid;
	nextGrid = tempGrid;
	//
	tempGrid2D = currentGrid2D;
	currentGrid2D = nextGrid2D;
	nextGrid2D = tempGrid2D;
}