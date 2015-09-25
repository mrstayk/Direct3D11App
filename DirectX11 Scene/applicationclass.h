/*----------------------------------------------------------------*
 | File name: applicationclass.h								  |
 | Author: Martin Staykov										  |
 | References:													  |
 | http://www.abertay.ac.uk/ (University of Abertay Dundee) 	  |
 | http://rastertek.com (No longer available. :( )				  |
 |																  |
 | Version:														  |
 | 1.0															  |
 | 8/2014														  |
 *----------------------------------------------------------------*/

#ifndef _APPLICATIONCLASS_H_
#define _APPLICATIONCLASS_H_

// Global/constant variables.
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

// Define the colour we will be clearing the scene background to.
#define SCREEN_BG_R		0.5f	// Red.
#define SCREEN_BG_G		0.5f	// Green.
#define SCREEN_BG_B		0.5f	// Blue.
#define SCREEN_BG_A		1.0f	// Alpha.

// Class includes.
#include "inputclass.h"
#include "d3dclass.h"
#include "cameraclass.h"
#include "terrainclass.h"
#include "timerclass.h"
#include "positionclass.h"
#include "fpsclass.h"
#include "cpuclass.h"
#include "fontshaderclass.h"
#include "textclass.h"
#include "lightshaderclass.h"
#include "lightclass.h"
#include "particleshaderclass.h"
#include "particlesystemclass.h"
#include "rendertextureclass.h"
#include "reflectionshaderclass.h"
#include "watershaderclass.h"
#include "waterclass.h"
#include "glowshaderclass.h"
#include "textureshaderclass.h"
#include "horizontalblurshaderclass.h"
#include "verticalblurshaderclass.h"
#include "orthowindowclass.h"

// Class name: ApplicationClass
class ApplicationClass
{
public:
	ApplicationClass();
	ApplicationClass(const ApplicationClass&);
	~ApplicationClass();

	bool Initialize(HINSTANCE, HWND, int, int);
	void Shutdown();
	bool Frame();

private:
	bool HandleInput(float);
	bool RenderRefractionToTexture();
	bool RenderReflectionToTexture();
	bool RenderScene();
	bool InitializeShaders(HWND hwnd);
	bool InitializeBlur(HWND hwnd, int screenWidth, int screenHeight);

	bool RenderSceneToTexture();
	bool DownSampleTexture();
	bool RenderHorizontalBlurToTexture();
	bool RenderVerticalBlurToTexture();
	bool UpSampleTexture();
	bool Render2DTextureScene();
	bool RenderBlur();

private:
	// Various toggles for the application.
	struct Toggles
	{
		bool BlurOn;
		bool WireframeOn;
		bool pause;
	} AppToggles;

	InputClass* m_Input;
	D3DClass* m_Direct3D;
	CameraClass* m_Camera;
	TerrainClass* m_Terrain;
	TimerClass* m_Timer;
	PositionClass* m_Position;
	FpsClass* m_Fps;
	CpuClass* m_Cpu;
	FontShaderClass* m_FontShader;
	TextClass* m_Text;
	LightShaderClass* m_LightShader;
	LightClass* m_Light;
	ParticleShaderClass* m_ParticleShader;
	ParticleSystemClass* m_ParticleSystem;
	GlowShaderClass* m_GlowShader;
	WaterClass* m_Water;
	ReflectionShaderClass* m_ReflectionShader;
	WaterShaderClass* m_WaterShader;
	ModelClass *m_SunModel;
	RenderTextureClass *m_RefractionTexture, *m_ReflectionTexture;
	
	TextureShaderClass* m_TextureShader;
	HorizontalBlurShaderClass* m_HorizontalBlurShader;
	VerticalBlurShaderClass* m_VerticalBlurShader;
	RenderTextureClass *m_RenderTexture, *m_DownSampleTexure, *m_HorizontalBlurTexture, *m_VerticalBlurTexture, *m_UpSampleTexure;
	OrthoWindowClass *m_SmallWindow, *m_FullScreenWindow;

	// This matrix holds the position of the sun for when we render its reflection in the water.
	D3DXMATRIX sunWorldMatrix;
};

#endif