/*----------------------------------------------------------------*
 | File name: modelclass.h										  |
 | Author: Martin Staykov										  |
 | References:													  |
 | http://www.abertay.ac.uk/ (University of Abertay Dundee) 	  |
 | http://rastertek.com (No longer available. :( )				  |
 |																  |
 | Version:														  |
 | 1.0															  |
 | 8/2014														  |
 *----------------------------------------------------------------*/

#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_

// Includes.
#include <d3d11.h>
#include <d3dx10math.h>
#include <vector>
#include <fstream>
using namespace std;

// Class includes.
#include "textureclass.h"
#include "TokenStream.h"

// Class name: ModelClass
class ModelClass
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
	    D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
	};

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	bool Initialize(ID3D11Device*, char*, WCHAR*, bool isTextFile = false);
	void Shutdown();
	void Render(ID3D11DeviceContext*);
	int GetIndexCount();

	ID3D11ShaderResourceView* GetTexture();

	// This function is used to return a single vertex's position.
	D3DXVECTOR3 ReturnArbitraryVertexPosition();

protected:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, WCHAR*);
	void ReleaseTexture();

	bool LoadModel(char*);
	void ReleaseModel();
	bool LoadTXT(char*);

protected:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;

	TextureClass* m_Texture;
	ModelType* m_model;

	D3DXVECTOR3 arbitraryVertexPos;
};

#endif