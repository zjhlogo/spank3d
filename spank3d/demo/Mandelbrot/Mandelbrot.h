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
#include <ui/controls/VContainer.h>

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
	void UpdateUi();

	bool OnMouseDown(MouseEvent& event);
	bool OnMouseUp(MouseEvent& event);
	bool OnMouseMove(MouseEvent& event);
	bool OnMouseWheel(MouseEvent& event);

private:
	IShader* m_pShader;
	Matrix4x4 m_matModelViewProj;

	Label* m_pLblMaxIterations;
	int m_maxIterations;

	Label* m_pLblZoom;
	Label* m_pLblCenterPos;
	VContainer* m_pVContainer;

	float m_zoom;
	float m_destZoom;
	float m_zoomSpeed;
	bool m_bZooming;

	bool m_bMouseDown;
	Vector2 m_centerPos;
	Vector2 m_destCenterPos;
	Vector2 m_centerPosSpeed;

};
#endif // __MANDELBROT_H__
