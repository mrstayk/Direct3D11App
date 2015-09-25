/*----------------------------------------------------------------*
 | File name: orthowindowclass.h								  |
 | Amended by: Martin Staykov									  |
 | References:													  |
 | http://www.abertay.ac.uk/ (University of Abertay Dundee) 	  |
 | http://rastertek.com (No longer available. :( )				  |
 |																  |
 | Version:														  |
 | 1.0															  |
 | 8/2014														  |
 *----------------------------------------------------------------*/

#ifndef _ORTHOWINDOWCLASS_H_
#define _ORTHOWINDOWCLASS_H_

// Includes.
#include <d3d11.h>
#include <d3dx10math.h>

// Class name: OrthoWindowClass
class OrthoWindowClass
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
	    D3DXVECTOR2 texture;
	};

public:
	OrthoWindowClass();
	OrthoWindowClass(const OrthoWindowClass&);
	~OrthoWindowClass();

	bool Initialize(ID3D11Device*, int, int);
	void Shutdown();
	void Render(ID3D11DeviceContext*);
	int GetIndexCount();

private:
	bool InitializeBuffers(ID3D11Device*, int, int);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
};

#endif