// Filename: applicationclass.cpp
#include "applicationclass.h"

ApplicationClass::ApplicationClass()
{
	m_Input = 0;
	m_Direct3D = 0;
	m_Camera = 0;
	m_Terrain = 0;
	m_Timer = 0;
	m_Position = 0;
	m_Fps = 0;
	m_Cpu = 0;
	m_FontShader = 0;
	m_Text = 0;
	m_LightShader = 0;
	m_Light = 0;
	m_ParticleShader = 0;
	m_ParticleSystem = 0;
	m_RefractionTexture = 0;
	m_ReflectionTexture = 0;
	m_ReflectionShader = 0;
	m_WaterShader = 0;
	m_SunModel = 0;
	m_Water = 0;
	m_GlowShader = 0;

	m_TextureShader = 0;
	m_HorizontalBlurShader = 0;
	m_VerticalBlurShader = 0;
	m_RenderTexture = 0;
	m_DownSampleTexure = 0;
	m_HorizontalBlurTexture = 0;
	m_VerticalBlurTexture = 0;
	m_UpSampleTexure = 0;
	m_SmallWindow = 0;
	m_FullScreenWindow = 0;

	AppToggles.BlurOn = false;
	AppToggles.WireframeOn = false;
	AppToggles.pause = false;	
}

ApplicationClass::ApplicationClass(const ApplicationClass& other)
{
}

ApplicationClass::~ApplicationClass()
{
}

bool ApplicationClass::Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight)
{
	bool result;
	float cameraX, cameraY, cameraZ;
	char videoCard[128];
	int videoMemory;

	// Create the input object. The input object will be used to handle reading the keyboard and mouse input from the user.
	m_Input = new InputClass;
	if(!m_Input)
	{
		return false;
	}

	// Initialize the input object.
	result = m_Input->Initialize(hinstance, hwnd, screenWidth, screenHeight);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the input object.", L"Error", MB_OK);
		return false;
	}

	// Create the Direct3D object.
	m_Direct3D = new D3DClass;
	if(!m_Direct3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize DirectX 11.", L"Error", MB_OK);
		return false;
	}

	// Initialize the world matrix of the sun object.
	m_Direct3D->GetWorldMatrix(sunWorldMatrix);

	// Create the camera object.
	m_Camera = new CameraClass;
	if(!m_Camera)
	{
		return false;
	}

	// Initialize a base view matrix with the camera for 2D user interface rendering.
	m_Camera->SetPosition(0.0f, 0.0f, -1.0f);
	m_Camera->Render();

	// Create the base view matrix at this point, to be used later for the blur render-to-texture.
	m_Camera->RenderBaseViewMatrix();

	// Set the initial position of the camera.
	cameraX = 50.0f;
	cameraY = 48.0;//2.0f;
	cameraZ = -7.0f;
	m_Camera->SetPosition(cameraX, cameraY, cameraZ);

	// Create the position object.
	m_Position = new PositionClass;
	if(!m_Position)
	{
		return false;
	}

	// Set the initial position of the viewer to the same as the initial camera position.
	m_Position->SetPosition(cameraX, cameraY, cameraZ);

	// Create the terrain object.
	m_Terrain = new TerrainClass;
	if(!m_Terrain)
	{
		return false;
	}

	// Initialize the terrain object.
	result = m_Terrain->Initialize(m_Direct3D->GetDevice(), "data/hmrn+2.bmp", L"data/drit.dds");
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the terrain object.", L"Error", MB_OK);
		return false;
	}

	// Create the timer object.
	m_Timer = new TimerClass;
	if(!m_Timer)
	{
		return false;
	}

	// Initialize the timer object.
	result = m_Timer->Initialize();
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the timer object.", L"Error", MB_OK);
		return false;
	}	

	// Create the fps object.
	m_Fps = new FpsClass;
	if(!m_Fps)
	{
		return false;
	}

	// Initialize the fps object.
	m_Fps->Initialize();

	// Create the cpu object.
	m_Cpu = new CpuClass;
	if(!m_Cpu)
	{
		return false;
	}

	// Initialize the cpu object.
	m_Cpu->Initialize();	

	// Create the text object.
	m_Text = new TextClass;
	if(!m_Text)
	{
		return false;
	}

	// Initialize the text object.
	result = m_Text->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, m_Camera->GetBaseViewMatrix());
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
		return false;
	}

	// Retrieve the video card information.
	m_Direct3D->GetVideoCardInfo(videoCard, videoMemory);

	// Set the video card information in the text object.
	result = m_Text->SetVideoCardInfo(videoCard, videoMemory, m_Direct3D->GetDeviceContext());
	if(!result)
	{
		MessageBox(hwnd, L"Could not set video card info in the text object.", L"Error", MB_OK);
		return false;
	}	

	// Create the light object.
	m_Light = new LightClass;
	if(!m_Light)
	{
		return false;
	}

	// Initialize the light object.
	m_Light->SetAmbientColor(0.3f, 0.3f, 0.3f, 1.0f);
	m_Light->SetDiffuseColor(0.8f, 0.8f, 0.8f, 1.0f);
	m_Light->SetDirection(-2.0f, -1.0f, 0.0f);	

	// Create the particle system object.
	m_ParticleSystem = new ParticleSystemClass;
	if(!m_ParticleSystem)
	{
		return false;
	}

	// Initialize the particle system object.
	result = m_ParticleSystem->Initialize(m_Direct3D->GetDevice(), L"data/snowflake.dds");
	if(!result)
	{
		return false;
	}

	// Create the refraction render to texture object.
	m_RefractionTexture = new RenderTextureClass;
	if(!m_RefractionTexture)
	{
		return false;
	}

	// Initialize the refraction render to texture object.
	result = m_RefractionTexture->Initialize(m_Direct3D->GetDevice(), screenWidth, screenHeight);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the refraction render to texture object.", L"Error", MB_OK);
		return false;
	}

	// Create the reflection render to texture object.
	m_ReflectionTexture = new RenderTextureClass;
	if(!m_ReflectionTexture)
	{
		return false;
	}

	// Initialize the reflection render to texture object.
	result = m_ReflectionTexture->Initialize(m_Direct3D->GetDevice(), screenWidth, screenHeight);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the reflection render to texture object.", L"Error", MB_OK);
		return false;
	}	

	// Create the model object.
	m_SunModel = new ModelClass;
	if(!m_SunModel)
	{
		return false;
	}

	// Initialize the model object.
	result = m_SunModel->Initialize(m_Direct3D->GetDevice(), "data/Sphere.obj", L"data/sun.dds");
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Create the water object.
	m_Water = new WaterClass;
	if(!m_Water)
	{
		return false;
	}

	// Initialize the water object.
	m_Water->Initialize(m_Direct3D->GetDevice(), "data/water.txt", L"data/water01.dds", true);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the water object.", L"Error", MB_OK);
		return false;
	}

	// Create the objects related to the blur effect.
	InitializeBlur(hwnd, screenWidth, screenHeight);

	// Create the shader objects.
	InitializeShaders(hwnd);

	return true;
}

bool ApplicationClass::InitializeBlur(HWND hwnd, int screenWidth, int screenHeight)
{
	bool result;

	// Render to texture window downsampling values.
	int downSampleWidth = screenWidth / 2.0f;
	int downSampleHeight = screenHeight / 2.0f;

	// Create the texture shader object.
	m_TextureShader = new TextureShaderClass;
	if(!m_TextureShader)
	{
		return false;
	}

	// Initialize the texture shader object.
	result = m_TextureShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the horizontal blur shader object.
	m_HorizontalBlurShader = new HorizontalBlurShaderClass;
	if(!m_HorizontalBlurShader)
	{
		return false;
	}

	// Initialize the horizontal blur shader object.
	result = m_HorizontalBlurShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the horizontal blur shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the vertical blur shader object.
	m_VerticalBlurShader = new VerticalBlurShaderClass;
	if(!m_VerticalBlurShader)
	{
		return false;
	}

	// Initialize the vertical blur shader object.
	result = m_VerticalBlurShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the vertical blur shader object.", L"Error", MB_OK);
		return false;
	}
	
	// Create the render to texture object.
	m_RenderTexture = new RenderTextureClass;
	if(!m_RenderTexture)
	{
		return false;
	}

	// Initialize the render to texture object.
	result = m_RenderTexture->InitializeWithViewport(m_Direct3D->GetDevice(), screenWidth, screenHeight, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the render to texture object.", L"Error", MB_OK);
		return false;
	}

	// Create the down sample render to texture object.
	m_DownSampleTexure = new RenderTextureClass;
	if(!m_DownSampleTexure)
	{
		return false;
	}

	// Initialize the down sample render to texture object.
	result = m_DownSampleTexure->InitializeWithViewport(m_Direct3D->GetDevice(), downSampleWidth, downSampleHeight, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the down sample render to texture object.", L"Error", MB_OK);
		return false;
	}

	// Create the horizontal blur render to texture object.
	m_HorizontalBlurTexture = new RenderTextureClass;
	if(!m_HorizontalBlurTexture)
	{
		return false;
	}

	// Initialize the horizontal blur render to texture object.
	result = m_HorizontalBlurTexture->InitializeWithViewport(m_Direct3D->GetDevice(), downSampleWidth, downSampleHeight, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the horizontal blur render to texture object.", L"Error", MB_OK);
		return false;
	}

	// Create the vertical blur render to texture object.
	m_VerticalBlurTexture = new RenderTextureClass;
	if(!m_VerticalBlurTexture)
	{
		return false;
	}

	// Initialize the vertical blur render to texture object.
	result = m_VerticalBlurTexture->InitializeWithViewport(m_Direct3D->GetDevice(), downSampleWidth, downSampleHeight, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the vertical blur render to texture object.", L"Error", MB_OK);
		return false;
	}

	// Create the up sample render to texture object.
	m_UpSampleTexure = new RenderTextureClass;
	if(!m_UpSampleTexure)
	{
		return false;
	}

	// Initialize the up sample render to texture object.
	result = m_UpSampleTexure->InitializeWithViewport(m_Direct3D->GetDevice(), screenWidth, screenHeight, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the up sample render to texture object.", L"Error", MB_OK);
		return false;
	}
	
	// Create the small ortho window object.
	m_SmallWindow = new OrthoWindowClass;
	if(!m_SmallWindow)
	{
		return false;
	}

	// Initialize the small ortho window object.
	result = m_SmallWindow->Initialize(m_Direct3D->GetDevice(), downSampleWidth, downSampleHeight);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the small ortho window object.", L"Error", MB_OK);
		return false;
	}

	// Create the full screen ortho window object.
	m_FullScreenWindow = new OrthoWindowClass;
	if(!m_FullScreenWindow)
	{
		return false;
	}

	// Initialize the full screen ortho window object.
	result = m_FullScreenWindow->Initialize(m_Direct3D->GetDevice(), screenWidth, screenHeight);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the full screen ortho window object.", L"Error", MB_OK);
		return false;
	}
}

bool ApplicationClass::InitializeShaders(HWND hwnd)
{
	bool result;

	// Create the font shader object.
	m_FontShader = new FontShaderClass;
	if(!m_FontShader)
	{
		return false;
	}

	// Initialize the font shader object.
	result = m_FontShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the font shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the light shader object.
	m_LightShader = new LightShaderClass;
	if(!m_LightShader)
	{
		return false;
	}

	// Initialize the light shader object.
	result = m_LightShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the terrain shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the light shader for the sun model.
	m_GlowShader = new GlowShaderClass;
	if(!m_GlowShader)
	{
		return false;
	}

	// Initialize it.
	result = m_GlowShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the glow shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the particle shader object.
	m_ParticleShader = new ParticleShaderClass;
	if(!m_ParticleShader)
	{
		return false;
	}

	// Initialize the particle shader object.
	result = m_ParticleShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the particle shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the reflection shader object.
	m_ReflectionShader = new ReflectionShaderClass;
	if(!m_ReflectionShader)
	{
		return false;
	}

	// Initialize the reflection shader object.
	result = m_ReflectionShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the refraction shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the water shader object.
	m_WaterShader = new WaterShaderClass;
	if(!m_WaterShader)
	{
		return false;
	}

	// Initialize the water shader object.
	result = m_WaterShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the water shader object.", L"Error", MB_OK);
		return false;
	}
}

void ApplicationClass::Shutdown()
{
	// Release the water object.
	if(m_Water)
	{
		m_Water->Shutdown();
		delete m_Water;
		m_Water = 0;
	}
	// Release the model object.
	if(m_SunModel)
	{
		m_SunModel->Shutdown();
		delete m_SunModel;
		m_SunModel = 0;
	}

	// Release the water shader object.
	if(m_WaterShader)
	{
		m_WaterShader->Shutdown();
		delete m_WaterShader;
		m_WaterShader = 0;
	}

	// Release the reflection shader object.
	if(m_ReflectionShader)
	{
		m_ReflectionShader->Shutdown();
		delete m_ReflectionShader;
		m_ReflectionShader = 0;
	}

	// Release the reflection render to texture object.
	if(m_ReflectionTexture)
	{
		m_ReflectionTexture->Shutdown();
		delete m_ReflectionTexture;
		m_ReflectionTexture = 0;
	}

	// Release the refraction render to texture object.
	if(m_RefractionTexture)
	{
		m_RefractionTexture->Shutdown();
		delete m_RefractionTexture;
		m_RefractionTexture = 0;
	}

	// Release the particle system object.
	if(m_ParticleSystem)
	{
		m_ParticleSystem->Shutdown();
		delete m_ParticleSystem;
		m_ParticleSystem = 0;
	}

	// Release the particle shader object.
	if(m_ParticleShader)
	{
		m_ParticleShader->Shutdown();
		delete m_ParticleShader;
		m_ParticleShader = 0;
	}

	// Release the light object.
	if(m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// Release the light shader object.
	if(m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}

	// Release the glow shader object.
	if(m_GlowShader)
	{
		m_GlowShader->Shutdown();
		delete m_GlowShader;
		m_GlowShader = 0;
	}

	// Release the text object.
	if(m_Text)
	{
		m_Text->Shutdown();
		delete m_Text;
		m_Text = 0;
	}

	// Release the font shader object.
	if(m_FontShader)
	{
		m_FontShader->Shutdown();
		delete m_FontShader;
		m_FontShader = 0;
	}

	// Release the cpu object.
	if(m_Cpu)
	{
		m_Cpu->Shutdown();
		delete m_Cpu;
		m_Cpu = 0;
	}

	// Release the fps object.
	if(m_Fps)
	{
		delete m_Fps;
		m_Fps = 0;
	}

	// Release the position object.
	if(m_Position)
	{
		delete m_Position;
		m_Position = 0;
	}

	// Release the timer object.
	if(m_Timer)
	{
		delete m_Timer;
		m_Timer = 0;
	}

	// Release the terrain object.
	if(m_Terrain)
	{
		m_Terrain->Shutdown();
		delete m_Terrain;
		m_Terrain = 0;
	}

	// Release the camera object.
	if(m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the Direct3D object.
	if(m_Direct3D)
	{
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = 0;
	}

	// Release the input object.
	if(m_Input)
	{
		m_Input->Shutdown();
		delete m_Input;
		m_Input = 0;
	}

	// Release the full screen ortho window object.
	if(m_FullScreenWindow)
	{
		m_FullScreenWindow->Shutdown();
		delete m_FullScreenWindow;
		m_FullScreenWindow = 0;
	}

	// Release the small ortho window object.
	if(m_SmallWindow)
	{
		m_SmallWindow->Shutdown();
		delete m_SmallWindow;
		m_SmallWindow = 0;
	}
	
	// Release the up sample render to texture object.
	if(m_UpSampleTexure)
	{
		m_UpSampleTexure->Shutdown();
		delete m_UpSampleTexure;
		m_UpSampleTexure = 0;
	}

	// Release the vertical blur render to texture object.
	if(m_VerticalBlurTexture)
	{
		m_VerticalBlurTexture->Shutdown();
		delete m_VerticalBlurTexture;
		m_VerticalBlurTexture = 0;
	}

	// Release the horizontal blur render to texture object.
	if(m_HorizontalBlurTexture)
	{
		m_HorizontalBlurTexture->Shutdown();
		delete m_HorizontalBlurTexture;
		m_HorizontalBlurTexture = 0;
	}

	// Release the down sample render to texture object.
	if(m_DownSampleTexure)
	{
		m_DownSampleTexure->Shutdown();
		delete m_DownSampleTexure;
		m_DownSampleTexure = 0;
	}
	
	// Release the render to texture object.
	if(m_RenderTexture)
	{
		m_RenderTexture->Shutdown();
		delete m_RenderTexture;
		m_RenderTexture = 0;
	}
	
	// Release the vertical blur shader object.
	if(m_VerticalBlurShader)
	{
		m_VerticalBlurShader->Shutdown();
		delete m_VerticalBlurShader;
		m_VerticalBlurShader = 0;
	}

	// Release the horizontal blur shader object.
	if(m_HorizontalBlurShader)
	{
		m_HorizontalBlurShader->Shutdown();
		delete m_HorizontalBlurShader;
		m_HorizontalBlurShader = 0;
	}

	// Release the texture shader object.
	if(m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	return;
}


bool ApplicationClass::Frame()
{
	bool result;
	
	// Update the system stats.
	m_Timer->Frame();
	m_Fps->Frame();
	m_Cpu->Frame();

	// Read the user input.
	result = m_Input->Frame();
	if(!result)
	{
		return false;
	}

	// Then handle it. 
	result = HandleInput(m_Timer->GetTime());
	if(!result)
	{
		return false;
	}	

	// Update the FPS value in the text object.
	result = m_Text->SetFps(m_Fps->GetFps(), m_Direct3D->GetDeviceContext());
	if(!result)
	{
		return false;
	}
	
	// Update the CPU usage value in the text object.
	result = m_Text->SetCpu(m_Cpu->GetCpuPercentage(), m_Direct3D->GetDeviceContext());
	if(!result)
	{
		return false;
	}
		
	// Update the CPU usage value in the text object.
	result = m_Text->SetParticleCount(m_ParticleSystem->GetCurrentParticleCount(), m_Direct3D->GetDeviceContext());
	if(!result)
	{
		return false;
	}

	if (!AppToggles.pause)
	{
		// Update the position of the water to simulate motion.	
		m_Water->Frame();

		// Run the frame processing for the particle system.
		m_ParticleSystem->Frame(m_Timer->GetTime(), m_Direct3D->GetDeviceContext());
	}

	// Render the refraction of the scene to a texture.
	result = RenderRefractionToTexture();
	if(!result)
	{
		return false;
	}

	// Render the reflection of the scene to a texture.
	result = RenderReflectionToTexture();
	if(!result)
	{
		return false;
	}

	// Activate/deactivate wireframe mode.
	if (AppToggles.WireframeOn) m_Direct3D->TurnWireframeModeOn();
	else m_Direct3D->TurnWireframeModeOff();

	// Render the scene with or without the blur effect.
	if (AppToggles.BlurOn) RenderBlur();
	else
	{
		// Clear the scene.
		m_Direct3D->BeginScene(SCREEN_BG_R, SCREEN_BG_G, SCREEN_BG_B, SCREEN_BG_A);

		// Render the graphics.
		result = RenderScene();
		if(!result)
		{
			return false;
		}

		// Present the rendered scene to the screen.
		m_Direct3D->EndScene();
	}

	return result;
}

bool ApplicationClass::RenderBlur()
{
	bool result;

	// First render the scene to a render texture.
	result = RenderSceneToTexture();
	if(!result)
	{
		return false;
	}

	// Next down sample the render texture to a smaller sized texture.
	result = DownSampleTexture();
	if(!result)
	{
		return false;
	}

	// Perform a horizontal blur on the down sampled render texture.
	result = RenderHorizontalBlurToTexture();
	if(!result)
	{
		return false;
	}

	// Now perform a vertical blur on the horizontal blur render texture.
	result = RenderVerticalBlurToTexture();
	if(!result)
	{
		return false;
	}

	// Up sample the final blurred render texture to screen size again.
	result = UpSampleTexture();
	if(!result)
	{
		return false;
	}

	// Render the blurred up sampled render texture to the screen.
	result = Render2DTextureScene();
	if(!result)
	{
		return false;
	}
}

bool ApplicationClass::HandleInput(float frameTime)
{
	bool keyDown, result;
	float posX, posY, posZ, rotX, rotY, rotZ;

	// Set the frame time for calculating the updated position.
	m_Position->SetFrameTime(frameTime);

	// Check if the user pressed escape and wants to exit the application.
	if(m_Input->IsEscapePressed() == true)
	{
		return false;
	}

	// Manage the blur mode.
	if (m_Input->IsBPressed()) AppToggles.BlurOn = true;
	else AppToggles.BlurOn = false;

	// Manage the wireframe mode.
	if (m_Input->IsWPressed()) AppToggles.WireframeOn = true;
	else AppToggles.WireframeOn = false;

	// Pause.
	if (m_Input->IsPJustPressed()) AppToggles.pause = !AppToggles.pause;

	// Handle the input.
	keyDown = m_Input->IsLeftPressed();
	m_Position->TurnLeft(keyDown);

	keyDown = m_Input->IsRightPressed();
	m_Position->TurnRight(keyDown);

	keyDown = m_Input->IsUpPressed();
	m_Position->MoveForward(keyDown);

	keyDown = m_Input->IsDownPressed();
	m_Position->MoveBackward(keyDown);

	keyDown = m_Input->IsAPressed();
	m_Position->MoveUpward(keyDown);

	keyDown = m_Input->IsZPressed();
	m_Position->MoveDownward(keyDown);

	keyDown = m_Input->IsPgUpPressed();
	m_Position->LookUpward(keyDown);

	keyDown = m_Input->IsPgDownPressed();
	m_Position->LookDownward(keyDown);
	
	// Check whether the right mouse button is pressed. If so, rotate the field of view.
	if (m_Input->IsMouseRightPressed())
	{
		int mouseX, mouseY;
		m_Input->GetMouseChange(mouseX, mouseY); // Store the change in mouse position.
		m_Position->GetRotation(rotX, rotY, rotZ); // Store the rotation from the position class.
		rotX += (float)mouseY/10; // Increment.
		rotY += (float)mouseX/10; // Increment and convert from int to float.
		m_Position->SetRotation(rotX, rotY, rotZ); // Update with the new rotation.
	}
	else m_Position->GetRotation(rotX, rotY, rotZ); // Otherwise just get the rotation.
	
	m_Position->GetPosition(posX, posY, posZ); // Store the position.
	
	// Set the position of the camera.
	m_Camera->SetPosition(posX, posY, posZ);
	m_Camera->SetRotation(rotX, rotY, rotZ);

	// Update the position values in the text object.
	result = m_Text->SetCameraPosition(posX, posY, posZ, m_Direct3D->GetDeviceContext());
	if(!result)
	{
		return false;
	}

	// Update the rotation values in the text object.
	result = m_Text->SetCameraRotation(rotX, rotY, rotZ, m_Direct3D->GetDeviceContext());
	if(!result)
	{
		return false;
	}

	return true;
}

bool ApplicationClass::RenderRefractionToTexture()
{
	D3DXVECTOR4 clipPlane;
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;
	
	// Setup a clipping plane based on the height of the water to clip everything above it.
	clipPlane = D3DXVECTOR4(0.0f, -1.0f, 0.0f, m_Water->GetHeight() + 0.1f);

	// Set the render target to be the refraction render to texture.
	m_RefractionTexture->SetRenderTarget(m_Direct3D->GetDeviceContext(), m_Direct3D->GetDepthStencilView());

	// Clear the refraction render to texture.
	m_RefractionTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), m_Direct3D->GetDepthStencilView(), 0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	// Put the terrain model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_Terrain->Render(m_Direct3D->GetDeviceContext());

	// Render the terrain using the reflection shader.
	result = m_ReflectionShader->Render(m_Direct3D->GetDeviceContext(), m_Terrain->GetIndexCount(), worldMatrix, viewMatrix,
										projectionMatrix, m_Terrain->GetTexture(), m_Light->GetDirection(), 
										m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), clipPlane);
	if(!result)
	{
		return false;
	}

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	m_Direct3D->SetBackBufferRenderTarget();

	// Reset the viewport back to the original.
	m_Direct3D->ResetViewport();

	return true;
}

bool ApplicationClass::RenderReflectionToTexture()
{
	D3DXMATRIX reflectionViewMatrix, worldMatrix, projectionMatrix;
	bool result;
	D3DXVECTOR4 clipPlane;

	// Setup a clipping plane based on the height of the water to clip everything below it.
	clipPlane = D3DXVECTOR4(0.0f, 1.0f, 0.0f, -m_Water->GetHeight() + 0.1f);

	// Set the render target to be the reflection render to texture.
	m_ReflectionTexture->SetRenderTarget(m_Direct3D->GetDeviceContext(), m_Direct3D->GetDepthStencilView());

	// Clear the reflection render to texture.
	m_ReflectionTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), m_Direct3D->GetDepthStencilView(), 0.0f, 0.0f, 0.0f, 1.0f);

	// Use the camera to render the reflection and create a reflection view matrix.
	m_Camera->RenderReflection(m_Water->GetHeight());

	// Get the camera reflection view matrix instead of the normal view matrix.
	reflectionViewMatrix = m_Camera->GetReflectionViewMatrix();

	// Get the world and projection matrices from the d3d object.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	// Put the wall model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_Terrain->Render(m_Direct3D->GetDeviceContext());

	// Render the terrain using the reflection shader with the reflection view matrix.
	result = m_ReflectionShader->Render(m_Direct3D->GetDeviceContext(), m_Terrain->GetIndexCount(), worldMatrix, reflectionViewMatrix,
										projectionMatrix, m_Terrain->GetTexture(), m_Light->GetDirection(), 
										m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), clipPlane);
	if(!result)
	{
		return false;
	}

	// Get the projection matrix from the d3d object.
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_SunModel->Render(m_Direct3D->GetDeviceContext());

	// Apply the glow shader to the sun, using its world matrix.
	result = m_GlowShader->Render(m_Direct3D->GetDeviceContext(), m_SunModel->GetIndexCount(), sunWorldMatrix, reflectionViewMatrix, projectionMatrix, 
									 m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Light->GetDirection(), m_SunModel->GetTexture());
	if(!result)
	{
		return false;
	}

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	m_Direct3D->SetBackBufferRenderTarget();

	// Reset the viewport back to the original.
	m_Direct3D->ResetViewport();

	return true;
}

bool ApplicationClass::RenderScene()
{
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix, baseWorld, reflectionMatrix;
	bool result;

	// Rotation variable for the rotation matrix.
	static float rotation = 200;
	if (!AppToggles.pause) rotation += (float)D3DX_PI * 0.0013f * 0.25f;
	if(rotation > 360.0f)
	{
		rotation -= 360.0f;
	}	

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);
	m_Direct3D->GetOrthoMatrix(orthoMatrix);

	// Create the translation, scaling and rotation matrices and multiply the world matrix by them.
	D3DXMATRIX matTrans, matRot, scaleMatrix;
	D3DXMatrixRotationZ(&matRot, rotation);	
	D3DXMatrixTranslation(&matTrans, 500.0f, 0.0f, 0.0f);
	D3DXMatrixScaling(&scaleMatrix, 10.0f, 10.0f, 10.0f);
	worldMatrix = scaleMatrix*matTrans*matRot;
	D3DXMatrixTranslation(&matTrans, 256.0f, 0.0f, 256.0f);
	worldMatrix *= matTrans;

	// Save the world matrix to use it for the reflection later.
	sunWorldMatrix = worldMatrix;

	// Get the position of the sun model by using an arbitrary vertex.
	// NOTE: It would have been better if this returned the coordinates of the centre of the sun.
	D3DXVECTOR3 v = m_SunModel->ReturnArbitraryVertexPosition();

	// Multiply it by the world matrix.
	D3DXVec3TransformCoord(&v, &v, &worldMatrix);

	// The light should point from the sun model to the centre of the terrain object.
	D3DXVECTOR3 lightDir = D3DXVECTOR3(256.0f - v.x, 0.0f - v.y, 256.0f - v.z);
	if (v.y < -25.0f) lightDir = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	if (!AppToggles.pause) m_Light->SetDirection(lightDir.x, lightDir.y, lightDir.z);	
		
	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_SunModel->Render(m_Direct3D->GetDeviceContext());

	// Apply the glow shader to the sun.
	result = m_GlowShader->Render(m_Direct3D->GetDeviceContext(), m_SunModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, 
									 m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Light->GetDirection(), m_SunModel->GetTexture());
	if(!result)
	{
		return false;
	}

	// Reset the world matrix.
	m_Direct3D->GetWorldMatrix(worldMatrix);

	// Render the terrain buffers.
	m_Terrain->Render(m_Direct3D->GetDeviceContext());

	// Apply light to the terrain (and texture it).
	result = m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_Terrain->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, 
									 m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Light->GetDirection(), m_Terrain->GetTexture());
	if(!result)
	{
		return false;
	}

	// Reset the world matrix.
	m_Direct3D->GetWorldMatrix(worldMatrix);

	// Get the camera reflection view matrix.
	reflectionMatrix = m_Camera->GetReflectionViewMatrix();

	// Translate to where the water model will be rendered.
	D3DXMatrixTranslation(&worldMatrix, 250.0f, m_Water->GetHeight(), 290.0f); 

	// Put the water model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_Water->Render(m_Direct3D->GetDeviceContext());

	// Render the water model using the water shader.
	result = m_WaterShader->Render(m_Direct3D->GetDeviceContext(), m_Water->GetIndexCount(), worldMatrix, viewMatrix, 
								   projectionMatrix, reflectionMatrix, m_ReflectionTexture->GetShaderResourceView(),
								   m_RefractionTexture->GetShaderResourceView(), m_Water->GetTexture(), 
								   m_Water->GetTranslation(), 0.01f);
	if(!result)
	{
		return false;
	}

	// Reset the world matrix.
	m_Direct3D->GetWorldMatrix(worldMatrix);

	// Turn on alpha blending.
	m_Direct3D->TurnOnAlphaBlending();

	// Translate to where the particle system will be rendered.
	D3DXMatrixTranslation(&worldMatrix, 256.0f, 30.0f, 256.0f); 

	// Put the particle system vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_ParticleSystem->Render(m_Direct3D->GetDeviceContext());

	// Render the system using the particle shader.
	result = m_ParticleShader->Render(m_Direct3D->GetDeviceContext(), m_ParticleSystem->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, 
										m_ParticleSystem->GetTexture());
	if(!result)
	{
		return false;
	}

	// Turn off alpha blending.
	m_Direct3D->TurnOffAlphaBlending();
	
	// Reset the world matrix.
	m_Direct3D->GetWorldMatrix(worldMatrix);

	// Turn off the Z buffer to begin all 2D rendering.
	m_Direct3D->TurnZBufferOff();
		
	// Turn on the alpha blending before rendering the text.
	m_Direct3D->TurnOnAlphaBlending();

	// Render the text user interface elements.
	result = m_Text->Render(m_Direct3D->GetDeviceContext(), m_FontShader, worldMatrix, orthoMatrix);
	if(!result)
	{
		return false;
	}

	// Turn off alpha blending after rendering the text.
	m_Direct3D->TurnOffAlphaBlending();

	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_Direct3D->TurnZBufferOn();
	
	return true;
}

bool ApplicationClass::RenderSceneToTexture()
{
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	bool result;
	
	// Set the render target to be the render to texture.
	m_RenderTexture->SetRenderTarget(m_Direct3D->GetDeviceContext(), m_RenderTexture->GetDepthStencilView());

	// Set the appropriate viewport.
	m_RenderTexture->SetViewport(m_Direct3D->GetDeviceContext());

	// Clear the render to texture.
	m_RenderTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), m_RenderTexture->GetDepthStencilView(), SCREEN_BG_R, SCREEN_BG_G, SCREEN_BG_B, SCREEN_BG_A);
	
	// Now that we have selected the render texture render the whole scene onto it.
	result = RenderScene();
	if(!result)
	{
		return false;
	}
	
	// Reset the render target back to the original back buffer and not the render to texture anymore.
	m_Direct3D->SetBackBufferRenderTarget();

	// Reset the viewport back to the original.
	m_Direct3D->ResetViewport();

	return true;
}

bool ApplicationClass::DownSampleTexture()
{
	D3DXMATRIX worldMatrix, viewMatrix, orthoMatrix;
	bool result;
	
	// Set the render target to be the render to texture.
	m_DownSampleTexure->SetRenderTarget(m_Direct3D->GetDeviceContext(), m_DownSampleTexure->GetDepthStencilView());

	// Set the appropriate viewport.
	m_DownSampleTexure->SetViewport(m_Direct3D->GetDeviceContext());

	// Clear the render to texture.
	m_DownSampleTexure->ClearRenderTarget(m_Direct3D->GetDeviceContext(), m_DownSampleTexure->GetDepthStencilView(), SCREEN_BG_R, SCREEN_BG_G, SCREEN_BG_B, SCREEN_BG_A);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world and view matrices from the camera and d3d objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	
	// Get the ortho matrix from the render to texture since texture has different dimensions being that it is smaller.
	m_DownSampleTexure->GetOrthoMatrix(orthoMatrix);

	// Turn off the Z buffer to begin all 2D rendering.
	m_Direct3D->TurnZBufferOff();

	// Put the small ortho window vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_SmallWindow->Render(m_Direct3D->GetDeviceContext());

	// Render the small ortho window using the texture shader and the render to texture of the scene as the texture resource.
	result = m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_SmallWindow->GetIndexCount(), worldMatrix, m_Camera->GetBaseViewMatrix(), orthoMatrix, 
					 m_RenderTexture->GetShaderResourceView());
	if(!result)
	{
		return false;
	}

	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_Direct3D->TurnZBufferOn();
	
	// Reset the render target back to the original back buffer and not the render to texture anymore.
	m_Direct3D->SetBackBufferRenderTarget();

	// Reset the viewport back to the original.
	m_Direct3D->ResetViewport();

	return true;
}


bool ApplicationClass::RenderHorizontalBlurToTexture()
{
	D3DXMATRIX worldMatrix, viewMatrix, orthoMatrix;
	float screenSizeX;
	bool result;

	// Store the screen width in a float that will be used in the horizontal blur shader.
	screenSizeX = (float)m_HorizontalBlurTexture->GetTextureWidth();
	
	// Set the render target to be the render to texture.
	m_HorizontalBlurTexture->SetRenderTarget(m_Direct3D->GetDeviceContext(), m_HorizontalBlurTexture->GetDepthStencilView());

	// Set the appropriate viewport.
	m_HorizontalBlurTexture->SetViewport(m_Direct3D->GetDeviceContext());

	// Clear the render to texture.
	m_HorizontalBlurTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), m_HorizontalBlurTexture->GetDepthStencilView(), SCREEN_BG_R, SCREEN_BG_G, SCREEN_BG_B, SCREEN_BG_A);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world and view matrices from the camera and d3d objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);

	// Get the ortho matrix from the render to texture since texture has different dimensions.
	m_HorizontalBlurTexture->GetOrthoMatrix(orthoMatrix);

	// Turn off the Z buffer to begin all 2D rendering.
	m_Direct3D->TurnZBufferOff();

	// Put the small ortho window vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_SmallWindow->Render(m_Direct3D->GetDeviceContext());
	
	// Render the small ortho window using the horizontal blur shader and the down sampled render to texture resource.
	result = m_HorizontalBlurShader->Render(m_Direct3D->GetDeviceContext(), m_SmallWindow->GetIndexCount(), worldMatrix, m_Camera->GetBaseViewMatrix(), orthoMatrix, 
											m_DownSampleTexure->GetShaderResourceView(), screenSizeX);
	if(!result)
	{
		return false;
	}

	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_Direct3D->TurnZBufferOn();

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	m_Direct3D->SetBackBufferRenderTarget();

	// Reset the viewport back to the original.
	m_Direct3D->ResetViewport();

	return true;
}

bool ApplicationClass::RenderVerticalBlurToTexture()
{
	D3DXMATRIX worldMatrix, viewMatrix, orthoMatrix;
	float screenSizeY;
	bool result;

	// Store the screen height in a float that will be used in the vertical blur shader.
	screenSizeY = (float)m_VerticalBlurTexture->GetTextureHeight();
	
	// Set the render target to be the render to texture.
	m_VerticalBlurTexture->SetRenderTarget(m_Direct3D->GetDeviceContext(), m_VerticalBlurTexture->GetDepthStencilView());

	// Set the appropriate viewport.
	m_VerticalBlurTexture->SetViewport(m_Direct3D->GetDeviceContext());

	// Clear the render to texture.
	m_VerticalBlurTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), m_VerticalBlurTexture->GetDepthStencilView(), SCREEN_BG_R, SCREEN_BG_G, SCREEN_BG_B, SCREEN_BG_A);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world and view matrices from the camera and d3d objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);

	// Get the ortho matrix from the render to texture since texture has different dimensions.
	m_VerticalBlurTexture->GetOrthoMatrix(orthoMatrix);

	// Turn off the Z buffer to begin all 2D rendering.
	m_Direct3D->TurnZBufferOff();

	// Put the small ortho window vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_SmallWindow->Render(m_Direct3D->GetDeviceContext());
	
	// Render the small ortho window using the vertical blur shader and the horizontal blurred render to texture resource.
	result = m_VerticalBlurShader->Render(m_Direct3D->GetDeviceContext(), m_SmallWindow->GetIndexCount(), worldMatrix, m_Camera->GetBaseViewMatrix(), orthoMatrix, 
										  m_HorizontalBlurTexture->GetShaderResourceView(), screenSizeY);
	if(!result)
	{
		return false;
	}

	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_Direct3D->TurnZBufferOn();

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	m_Direct3D->SetBackBufferRenderTarget();

	// Reset the viewport back to the original.
	m_Direct3D->ResetViewport();

	return true;
}


bool ApplicationClass::UpSampleTexture()
{
	D3DXMATRIX worldMatrix, viewMatrix, orthoMatrix;
	bool result;

	// Set the render target to be the render to texture.
	m_UpSampleTexure->SetRenderTarget(m_Direct3D->GetDeviceContext(), m_UpSampleTexure->GetDepthStencilView());

	// Set the appropriate viewport.
	m_UpSampleTexure->SetViewport(m_Direct3D->GetDeviceContext());

	// Clear the render to texture.
	m_UpSampleTexure->ClearRenderTarget(m_Direct3D->GetDeviceContext(), m_UpSampleTexure->GetDepthStencilView(), SCREEN_BG_R, SCREEN_BG_G, SCREEN_BG_B, SCREEN_BG_A);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world and view matrices from the camera and d3d objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);

	// Get the ortho matrix from the render to texture since texture has different dimensions.
	m_UpSampleTexure->GetOrthoMatrix(orthoMatrix);

	// Turn off the Z buffer to begin all 2D rendering.
	m_Direct3D->TurnZBufferOff();

	// Put the full screen ortho window vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_FullScreenWindow->Render(m_Direct3D->GetDeviceContext());

	// Render the full screen ortho window using the texture shader and the small sized final blurred render to texture resource.
	result = m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_FullScreenWindow->GetIndexCount(), worldMatrix, m_Camera->GetBaseViewMatrix(), orthoMatrix, 
									 m_VerticalBlurTexture->GetShaderResourceView());
	if(!result)
	{
		return false;
	}

	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_Direct3D->TurnZBufferOn();
	
	// Reset the render target back to the original back buffer and not the render to texture anymore.
	m_Direct3D->SetBackBufferRenderTarget();

	// Reset the viewport back to the original.
	m_Direct3D->ResetViewport();

	return true;
}


bool ApplicationClass::Render2DTextureScene()
{
	D3DXMATRIX worldMatrix, viewMatrix, orthoMatrix;
	bool result;

	// Clear the buffers to begin the scene.
	m_Direct3D->BeginScene(SCREEN_BG_R, SCREEN_BG_G, SCREEN_BG_B, SCREEN_BG_A);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and ortho matrices from the camera and d3d objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Direct3D->GetOrthoMatrix(orthoMatrix);

	// Turn off the Z buffer to begin all 2D rendering.
	m_Direct3D->TurnZBufferOff();

	// Put the full screen ortho window vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_FullScreenWindow->Render(m_Direct3D->GetDeviceContext());

	// Render the full screen ortho window using the texture shader and the full screen sized blurred render to texture resource.
	result = m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_FullScreenWindow->GetIndexCount(), worldMatrix, m_Camera->GetBaseViewMatrix(), orthoMatrix, 
									 m_UpSampleTexure->GetShaderResourceView());
	if(!result)
	{
		return false;
	}

	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_Direct3D->TurnZBufferOn();
	
	// Present the rendered scene to the screen.
	m_Direct3D->EndScene();

	return true;
}