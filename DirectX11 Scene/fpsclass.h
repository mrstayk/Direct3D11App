/*----------------------------------------------------------------*
 | File name: fpsclass.h										  |
 | Amended by: Martin Staykov									  |
 | References:													  |
 | http://www.abertay.ac.uk/ (University of Abertay Dundee) 	  |
 | http://rastertek.com (No longer available. :( )				  |
 |																  |
 | Version:														  |
 | 1.0															  |
 | 8/2014														  |
 *----------------------------------------------------------------*/

#ifndef _FPSCLASS_H_
#define _FPSCLASS_H_

// Linking.
#pragma comment(lib, "winmm.lib")

// Class includes.
#include <windows.h>
#include <mmsystem.h>

// Class name: FpsClass
class FpsClass
{
public:
	FpsClass();
	FpsClass(const FpsClass&);
	~FpsClass();

	void Initialize();
	void Frame();
	int GetFps();

private:
	int m_fps, m_count;
	unsigned long m_startTime;
};

#endif