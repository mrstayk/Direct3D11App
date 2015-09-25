/*----------------------------------------------------------------*
 | File name: inputclass.h										  |
 | Author: Martin Staykov										  |
 | References:													  |
 | http://www.abertay.ac.uk/ (University of Abertay Dundee) 	  |
 | http://rastertek.com (No longer available. :( )				  |
 |																  |
 | Version:														  |
 | 1.0															  |
 | 8/2014														  |
 *----------------------------------------------------------------*/

#ifndef _INPUTCLASS_H_
#define _INPUTCLASS_H_

// Pre-processing directives.
#define DIRECTINPUT_VERSION 0x0800

// Linking.
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

// Includes.
#include <dinput.h>

// Class name: InputClass
class InputClass
{
public:
	InputClass();
	InputClass(const InputClass&);
	~InputClass();

	bool Initialize(HINSTANCE, HWND, int, int);
	void Shutdown();
	bool Frame();

	void GetMouseLocation(int&, int&);
	void GetMouseChange(int&,int&);
	int  GetMouseScroll();
	bool IsMouseRightPressed();

	bool IsEscapePressed();
	bool IsLeftPressed();
	bool IsRightPressed();
	bool IsUpPressed();
	bool IsDownPressed();
	bool IsAPressed();
	bool IsZPressed();
	bool IsWPressed();
	bool IsBPressed();
	bool IsPJustPressed();
	bool IsPgUpPressed();
	bool IsPgDownPressed();

private:
	bool ReadKeyboard();
	bool ReadMouse();
	void ProcessInput();

private:
	IDirectInput8* m_directInput;
	IDirectInputDevice8* m_keyboard;
	IDirectInputDevice8* m_mouse;
	DIMOUSESTATE m_mouseState;

	unsigned char m_keyboardState[256];
	int m_screenWidth, m_screenHeight;
	int m_mouseX, m_mouseY;

	bool m_prevPressed_P;
};

#endif