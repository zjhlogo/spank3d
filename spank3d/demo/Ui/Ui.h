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

class Ui : public IApp
{
public:
	RTTI_DEF(Ui, IApp);

	Ui();
	virtual ~Ui();

	virtual bool Initialize();
	virtual void Terminate();

	virtual void Update(float dt);
	virtual void Render();

};
#endif // __UI_H__
