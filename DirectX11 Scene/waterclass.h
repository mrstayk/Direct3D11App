/*----------------------------------------------------------------*
 | File name: waterclass.h										  |
 | Author: Martin Staykov										  |
 | References:													  |
 | http://www.abertay.ac.uk/ (University of Abertay Dundee) 	  |
 | http://rastertek.com (No longer available. :( )				  |
 |																  |
 | Version:														  |
 | 1.0															  |
 | 8/2014														  |
 *----------------------------------------------------------------*/

#ifndef _WATERCLASS_H_
#define _WATERCLASS_H_

// includes.
#include <d3d11.h>
#include <d3dx10math.h>

// Class includes.
#include "modelclass.h"

// Class name: WaterClass
class WaterClass : public ModelClass // Inherits all properties from ModelClass.
{
public:
	WaterClass();
	WaterClass(const WaterClass&);
	~WaterClass();

	bool Initialize(ID3D11Device*, char*, WCHAR*, bool isTextFile = false);
	void Frame();	
	float GetHeight();
	float GetTranslation();
	
protected:	
	float m_Translation;
	float m_Height;
};

#endif