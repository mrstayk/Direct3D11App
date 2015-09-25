/*----------------------------------------------------------------*
 | File name: cameraclass.h										  |
 | Author: Martin Staykov										  |
 | References:													  |
 | http://www.abertay.ac.uk/ (University of Abertay Dundee) 	  |
 | http://rastertek.com (No longer available. :( )				  |
 |																  |
 | Version:														  |
 | 1.0															  |
 | 8/2014														  |
 *----------------------------------------------------------------*/

#ifndef _CAMERACLASS_H_
#define _CAMERACLASS_H_

// Class includes.
#include <d3dx10math.h>

// Class name: CameraClass
class CameraClass
{
public:
	CameraClass();
	CameraClass(const CameraClass&);
	~CameraClass();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);
	void Render();
	void GetViewMatrix(D3DXMATRIX&);
	D3DXMATRIX GetBaseViewMatrix();
	void RenderReflection(float);
	void RenderBaseViewMatrix();
	
	D3DXVECTOR3 GetPosition();
	D3DXVECTOR3 GetRotation();
		
	D3DXMATRIX GetReflectionViewMatrix();

private:
	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;

	D3DXMATRIX m_viewMatrix, m_baseViewMatrix, m_reflectionViewMatrix;
};

#endif