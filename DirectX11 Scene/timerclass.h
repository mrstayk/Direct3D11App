/*----------------------------------------------------------------*
 | File name: timerclass.h										  |
 | Amended by: Martin Staykov									  |
 | References:													  |
 | http://www.abertay.ac.uk/ (University of Abertay Dundee) 	  |
 | http://rastertek.com (No longer available. :( )				  |
 |																  |
 | Version:														  |
 | 1.0															  |
 | 8/2014														  |
 *----------------------------------------------------------------*/

#ifndef _TIMERCLASS_H_
#define _TIMERCLASS_H_

// Includes
#include <windows.h>

// Class name: TimerClass
class TimerClass
{
public:
	TimerClass();
	TimerClass(const TimerClass&);
	~TimerClass();

	bool Initialize();
	void Frame();
	float GetTime();

private:
	INT64 m_frequency;
	float m_ticksPerMs;
	INT64 m_startTime;
	float m_frameTime;
};

#endif