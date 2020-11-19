//
//  gl_frontEnd.h
//  GL threads
//
//  Created by Jean-Yves Hervé on 2017-04-24.
//	Updated 2020-11-06

#ifndef GL_FRONT_END_H
#define GL_FRONT_END_H


//------------------------------------------------------------------------------
//	Find out whether we are on Linux or macOS (sorry, Windows people)
//	and load the OpenGL & glut headers.
//	For the macOS, lets us choose which glut to use
//------------------------------------------------------------------------------
#if (defined(__dest_os) && (__dest_os == __mac_os )) || \
	defined(__APPLE_CPP__) || defined(__APPLE_CC__)
	//	Either use the Apple-provided---but deprecated---glut
	//	or the third-party freeglut install
	#if 1
		#include <GLUT/GLUT.h>
		//	Here ask Xcode/clang++ to suppress "deprecated" warnings
		#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
	#else
		#include <GL/freeglut.h>
		#include <GL/gl.h>
	#endif
#elif defined(linux)
	#include <GL/glut.h>
#else
	#error unknown OS
#endif


//-----------------------------------------------------------------------------
//	Custom data types
//-----------------------------------------------------------------------------


typedef enum ColorLabel {
	BLACK_COL = 0,
	WHITE_COL,
	BLUE_COL,
	GREEN_COL,
	YELLOW_COL,
	RED_COL,
	//
	NB_COLORS
} ColorLabel;

using PaneIndex = enum
{
	GRID_PANE,
	STATE_PANE,
	//
	NUM_PANES
};


//	Rules of the automaton (in C, it's a lot more complicated than in
//	C++/Java/Python/Swift to define an easy-to-initialize data type storing
//	arrays of numbers.  So, in this program I hard-code my rules
#define GAME_OF_LIFE_RULE	1
#define CORAL_GROWTH_RULE	2
#define AMOEBA_RULE			3
#define MAZE_RULE			4


//-----------------------------------------------------------------------------
//	Function prototypes
//-----------------------------------------------------------------------------

void drawGrid(unsigned int** grid, unsigned short numRows, unsigned short numCols);
void drawState(unsigned short numLiveThreads);
void initializeFrontEnd(int argc, const char** argv, void (*gridCB)(void), void (*stateCB)(void));
void toggleDrawGridLines(void);

//	Functions implemented in main.cpp but called byt the glut callback functions
void resetGrid(void);
void oneGeneration(void);
void myKeyboardFunc(unsigned char c, int x, int y);
void myTimerFunc(int val);
void myDisplayFunc(void);

#endif // GL_FRONT_END_H

