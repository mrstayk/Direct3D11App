/*----------------------------------------------------------------*
 | File name: particlesystemclass.h								  |
 | Author: Martin Staykov										  |
 | References:													  |
 | http://www.abertay.ac.uk/ (University of Abertay Dundee) 	  |
 | http://rastertek.com (No longer available. :( )				  |
 |																  |
 | Version:														  |
 | 1.0															  |
 | 8/2014														  |
 *----------------------------------------------------------------*/

#ifndef _PARTICLESYSTEMCLASS_H_
#define _PARTICLESYSTEMCLASS_H_

// Includes.
#include <d3d11.h>
#include <d3dx10math.h>

// Class includes.
#include "textureclass.h"

// Class name: ParticleSystemClass
class ParticleSystemClass
{
private:
	struct ParticleType
	{
		float positionX, positionY, positionZ;
		float red, green, blue;
		float velocity;
		bool active;
	};

	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR4 color;
	};

public:
	ParticleSystemClass();
	ParticleSystemClass(const ParticleSystemClass&);
	~ParticleSystemClass();

	bool Initialize(ID3D11Device*, WCHAR*);
	void Shutdown();
	bool Frame(float, ID3D11DeviceContext*);
	void Render(ID3D11DeviceContext*);
	int GetIndexCount();
	int GetCurrentParticleCount();

	ID3D11ShaderResourceView* GetTexture();	

private:
	bool LoadTexture(ID3D11Device*, WCHAR*);
	void ReleaseTexture();

	bool InitializeParticleSystem();
	void ShutdownParticleSystem();

	void EmitParticles(float);
	void UpdateParticles(float);
	void KillParticles();

	bool InitializeBuffers(ID3D11Device*);
	bool UpdateBuffers(ID3D11DeviceContext*);
	void RenderBuffers(ID3D11DeviceContext*);
	void ShutdownBuffers();

private:
	float m_particleDeviationX, m_particleDeviationY, m_particleDeviationZ;
	float m_particleVelocity, m_particleVelocityVariation;
	float m_particleSize, m_particlesPerSecond;
	int m_maxParticles;
	int m_currentParticleCount;
	float m_accumulatedTime;
	int m_vertexCount, m_indexCount;

	TextureClass* m_Texture;
	ParticleType* m_particleList;	
	VertexType* m_vertices;
	
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
};

#endif