// Filename: waterclass.cpp
#include "waterclass.h"


WaterClass::WaterClass()
{
	/*m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_Texture = 0;*/
}


WaterClass::WaterClass(const WaterClass& other)
{
}


WaterClass::~WaterClass()
{
}

bool WaterClass::Initialize(ID3D11Device* device, char* modelFilename, WCHAR* textureFilename, bool isTextFile)
{
	bool result;

	// Initialize values.
	m_Translation = 0.0f;
	m_Height = 4.75f;

	// Initialize base class functionallity.
	ModelClass::Initialize(device, modelFilename, textureFilename, isTextFile);	

	return true;
}

void WaterClass::Frame()
{
	// Update the position of the water to simulate motion.
	m_Translation += 0.0005f;
	if(m_Translation > 1.0f)
	{
		m_Translation -= 1.0f;
	}

	return;
}

float WaterClass::GetHeight()
{
	return m_Height;
}

float WaterClass::GetTranslation()
{
	return m_Translation;
}