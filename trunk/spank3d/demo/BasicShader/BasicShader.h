/*!
 * \file BasicShader.h
 * \date 11-05-2012 10:58:40
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __BASICSHADER_H__
#define __BASICSHADER_H__

#include <Spank3d.h>
#include <camera/TargetCameraControl.h>

class BasicShader : public IApp
{
public:
	RTTI_DEF(BasicShader, IApp);

	BasicShader();
	virtual ~BasicShader();

	virtual bool Initialize();
	virtual void Terminate();

	virtual void Update(float dt);
	virtual void Render();

private:
	bool OnMouseEvent(MouseEvent& mouseEvent);

private:
	IShader* m_pShader;
	IMesh* m_pMesh;

	ICamera* m_pCamera;
	TargetCameraControl* m_pTargetCameraCtrl;

};
#endif // __BASICSHADER_H__
