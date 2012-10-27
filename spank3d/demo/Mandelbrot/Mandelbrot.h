/*!
 * \file Mandelbrot.h
 * \date 10-27-2012 15:29:47
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __MANDELBROT_H__
#define __MANDELBROT_H__

#include <Spank3d.h>

class Mandelbrot : public IApp
{
public:
	RTTI_DEF(Mandelbrot, IApp);

	Mandelbrot();
	virtual ~Mandelbrot();

	virtual bool Initialize();
	virtual void Terminate();

	virtual void Update(float dt);
	virtual void Render();

private:
	IShader* m_pShader;
	Matrix4x4 m_matModelViewProj;

};
#endif // __MANDELBROT_H__
