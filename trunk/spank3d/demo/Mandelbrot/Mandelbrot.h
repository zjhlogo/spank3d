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
#include <ui/controls/Label.h>

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
	bool OnBtnDecIterationsDown(MouseEvent& event);
	bool OnBtnIncIterationsDown(MouseEvent& event);
	void UpdateIterations(int iterations);

	bool OnBtnDecZoomDown(MouseEvent& event);
	bool OnBtnIncZoomDown(MouseEvent& event);
	void UpdateZoom(float zoom);


private:
	IShader* m_pShader;
	Matrix4x4 m_matModelViewProj;

	Label* m_pLblMaxIterations;
	int m_maxIterations;

	Label* m_pLblZoom;
	float m_zoom;

};
#endif // __MANDELBROT_H__
