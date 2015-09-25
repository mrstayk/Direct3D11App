/*----------------------------------------------------------------*
 | File name: systemclass.h										  |
 | Author: Martin Staykov										  |
 | References:													  |
 | http://www.abertay.ac.uk/ (University of Abertay Dundee) 	  |
 | http://rastertek.com (No longer available. :( )				  |
 |																  |
 | Version:														  |
 | 1.0															  |
 | 8/2014														  |
 *----------------------------------------------------------------*/

#ifndef _SYSTEMCLASS_H_
#define _SYSTEMCLASS_H_

// Pre-processing directives
#define WIN32_LEAN_AND_MEAN

// Include.
#include <windows.h>

// Class include.
#include "applicationclass.h"

// Class name: SystemClass
class SystemClass
{
public:
	SystemClass();
	SystemClass(const SystemClass&);
	~SystemClass();

	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	void InitializeWindows(int&, int&);
	void ShutdownWindows();

private:
	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;
	ApplicationClass* m_Application;
};

// Function prototypes.
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// Globals.
static SystemClass* ApplicationHandle = 0;

#endif