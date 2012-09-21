/*!
 * \file Ui.h
 * \date 9-11-2012 20:56:40
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __UI_H__
#define __UI_H__

#include <Spank3d.h>
#include <event/MouseEvent.h>

class Ui : public IApp
{
public:
	RTTI_DEF(Ui, IApp);

	Ui();
	virtual ~Ui();

	virtual bool Initialize();
	virtual void Terminate();

	virtual void Update(float dt);

private:
	void UpdatePosition(float dt);
	void UpdatePatchSize(float dt);

private:
	ITexture* m_pTexSun;
	Vector2 m_sunSpeed;
	Vector2 m_sunPosition;

	NinePatchStyle* m_pNinePatchStyle;
	Vector2 m_patchSpeed;
	Vector2 m_patchPos;
	Vector2 m_patchSize;

};
#endif // __UI_H__
