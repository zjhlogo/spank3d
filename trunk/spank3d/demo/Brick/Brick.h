/*!
 * \file Brick.h
 * \date 10-25-2012 13:07:07
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __BRICK_H__
#define __BRICK_H__

#include <Spank3d.h>
#include <camera/TargetCameraControl.h>

class Brick : public IApp
{
public:
	RTTI_DEF(Brick, IApp);

	Brick();
	virtual ~Brick();

	virtual bool Initialize();
	virtual void Terminate();

	virtual void Update(float dt);
	virtual void Render();

private:
	IShader* m_pShader;
	IMesh* m_pMesh;

	ICamera* m_pCamera;
	TargetCameraControl* m_pTargetCameraCtrl;

};
#endif // __BRICK_H__
