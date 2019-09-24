#include "BoggyAI.h"

#include "BoggyApp.h"
#include "BoggyWnd.h"

C_BoggyAI::C_BoggyAI(C_BoggyWnd * pBoggyWnd)
{
	m_pBoggyWnd = pBoggyWnd;

	m_nHowLong = 0;
	m_nBoggyMode = 0;
}

C_BoggyAI::~C_BoggyAI()
{
}

int C_BoggyAI::RunComeAndGo()
{
	int retval=0;
	static x = 0;

	if ( x < 20 )
	{
		if ( x >= 9 && x <= 19 )
			retval = RIGHT<<16|STAND;
		else
			retval = RIGHT<<16|RUN;
		x++;
	}
	else
	{
		if ( x >= 29 && x <= 39 )
			retval = LEFT<<16|STAND;
		else
			retval = LEFT<<16|RUN_SLOBBER;
		x++;
	}

	if ( x == 40 ) x = 0;
	return retval;
}

int C_BoggyAI::RunFollowMouse()
{
	POINT lp;
	RECT rc;
	GetCursorPos(&lp);

	GetWindowRect(GetBoggyWnd()->GetHandle(),&rc);
	int bodysize = rc.left-rc.right;
	if ( m_nCurAction>>16 == LEFT )
	{	// go left , if mouse is far on the left
		if ( rc.left > lp.x )
			return LEFT<<16|RUN;
		else  // turn back if mouse is near behind
			if ( rc.left < lp.x + bodysize )
				return RIGHT<<16|STAND;
			else // stand still if mouse is near forward
				return LEFT<<16|STAND;
	}
	else
	{	// go right, if mouse is far on the right
		if ( rc.right < lp.x )
			return RIGHT<<16|RUN;
		else // turn back if mouse is near behind
			if ( rc.right > lp.x - bodysize)
				return LEFT<<16|STAND;
			else // stand still if mouse is near forward
				return RIGHT<<16|STAND;
	}
}

int C_BoggyAI::GenNextAction()
{
	if ( m_nHowLong == 0 )
	{
		m_nHowLong = RANDOM(50)+10;
		m_nBoggyMode = RANDOM(2);
	}
	else
		m_nHowLong--;
/*
	switch(m_nBoggyMode)
	{
	case 0:
		return m_nCurAction = RunComeAndGo();
	case 1:
		return m_nCurAction = RunFollowMouse();
	default:
		return m_nCurAction = RunComeAndGo();
	}*/
	return m_nCurAction = RunFollowMouse();
}

