#ifndef _BOGGYAPP_H_
#define _BOGGYAPP_H_

/////////// global define ///////////////////////////////
enum DIRECTION
{
	LEFT=0,
	RIGHT=1,
	MIDDLE=2
 };
enum DISPLAYMODE
{
	SINGLE_SIZE=1,
	DOUBLE_SIZE=2,
	TRIPLE_SIZE=3
};
#define NUM_OF_ACTION 4
enum ACTION
{
	STAND=0,
	STAND_SLOBBER=1,
	RUN=2,
	RUN_SLOBBER=3
};
enum MOVE_FLAG
{
	MOVE,
	STILL,
	NOTHING
};

#define RANDOM(_k_)		(int)( rand()%(_k_) )

#define RANDOMIZE()     srand((unsigned) time(NULL))

#define TRANSPARENT_COLOR RGB(0,128,128)
#define IDTIMER_UPDATE	1000
#define IDTIMER_RUN		1001
#define TIMER_UPDATE	50
#define TIMER_RUN		50

////////////////////////////////////////////////////////
#endif