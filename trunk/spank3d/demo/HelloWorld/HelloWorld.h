/*!
 * \file HelloWorld.h
 * \date 4-14-2012 21:46:03
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __HELLOWORLD_H__
#define __HELLOWORLD_H__

#include <Spank3d.h>
#include <camera/TargetCameraControl.h>

class HelloWorld : public IApp
{
public:
	RTTI_DEF(HelloWorld, IApp);

	HelloWorld();
	virtual ~HelloWorld();

	virtual bool Initialize();
	virtual void Terminate();

	virtual void Update(float dt);
	virtual void Render();

private:
	IShader* m_pShader;
	IMesh* m_pMesh;
	ITexture* m_pTexture;

	ICamera* m_pCamera;
	TargetCameraControl* m_pTargetCameraCtrl;

};
#endif // __HELLOWORLD_H__
