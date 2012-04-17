/*!
 * \file HelloWorld.h
 * \date 4-14-2012 21:46:03
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __HELLOWORLD_H__
#define __HELLOWORLD_H__

#include <Spank3D.h>

class HelloWorld : public IApp
{
public:
	RTTI_DEF(HelloWorld, IApp);

	HelloWorld();
	virtual ~HelloWorld();

	virtual bool Initialize();
	virtual void Terminate();

	virtual void Update(float dt);

private:
	float m_rotTriangle;		// Angle For The Triangle ( NEW )
	float m_rotQuad;			// Angle For The Quad ( NEW )

};
#endif // __HELLOWORLD_H__
