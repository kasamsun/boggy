#include "BoggyAI.h"

#include "BoggyApp.h"
#include "BoggyWnd.h"

C_BoggyAI::C_BoggyAI(C_BoggyWnd * pBoggyWnd)
{
	m_pBoggyWnd = pBoggyWnd;

	m_nHowLong = 0;
	m_nBoggyMode = 0;
	m_nPower = 50;
}

C_BoggyAI::~C_BoggyAI()
{
}

int C_BoggyAI::RunComeAndGo()
{
	int direction = 0;
	int action = 0;
	static int x = 0;

	if ( x < 20 ) {
		if ( x >= 9 && x <= 19 ) {
			direction = RIGHT;
			action = STAND;
			m_nPower++;
		} else {
			direction = RIGHT;
			action = RUN;
			m_nPower--;
		}
		x++;
	} else {
		if ( x >= 29 && x <= 39 ) {
			direction = LEFT;
			action = STAND;
			m_nPower++;
		} else {
			direction = LEFT;
			action = RUN;
			m_nPower--;
		}
		x++;
	}

	if ( x == 40 ) {
		x = 0;
	}
	if (m_nPower>50) m_nPower = 50;
	if (m_nPower<-50) m_nPower = -50;
	if (m_nPower<0 && action == RUN) {
		action = RUN_SLOBBER;
	} else if (m_nPower<0 && action == STAND) {
		action = STAND_SLOBBER;
	}
	return direction<<16|action;
}

int C_BoggyAI::RunFollowMouse()
{
	int direction = 0;
	int action = 0;
	POINT lp;
	RECT rc;
	GetCursorPos(&lp);

	GetWindowRect(GetBoggyWnd()->GetHandle(),&rc);
	int bodysize = rc.left-rc.right;

	if ( m_nCurAction>>16 == LEFT ) {
		// go left , if mouse is far on the left
		if ( rc.left > lp.x ) {
			m_nPower--;
			direction = LEFT;
			action = RUN;
		} else {  // turn back if mouse is near behind
			m_nPower++;
			if ( rc.left < lp.x + bodysize ) {
				direction = RIGHT;
				action = STAND;
			} else { // stand still if mouse is near forward
				direction = LEFT;
				action = STAND;
			}
		}
	} else {	// go right, if mouse is far on the right
		if ( rc.right < lp.x ) {
			m_nPower--;
			direction = RIGHT;
			action = RUN;
		} else { // turn back if mouse is near behind
			m_nPower++;
			if ( rc.right > lp.x - bodysize) {
				direction = LEFT;
				action = STAND;
			} else {// stand still if mouse is near forward
				direction = RIGHT;
				action = STAND;
			}
		}
	}
	if (m_nPower>50) m_nPower = 50;
	if (m_nPower<-50) m_nPower = -50;
	if (m_nPower<0 && action == RUN) {
		action = RUN_SLOBBER;
	} else if (m_nPower<0 && action == STAND) {
		action = STAND_SLOBBER;
	}
	return direction<<16|action;
}

int C_BoggyAI::GenNextAction()
{
	if ( m_nHowLong == 0 ) {
		m_nHowLong = RANDOM(50)+10;
		m_nBoggyMode = RANDOM(5);
	} else {
		m_nHowLong--;
	}

	switch(m_nBoggyMode)
	{
	case 0:
		return m_nCurAction = RunComeAndGo();
	case 1:
		return m_nCurAction = RunFollowMouse();
	default:
		return m_nCurAction = RunFollowMouse();
	}
	//return m_nCurAction = RunComeAndGo();
}

