/*!
 * \file IApp.h
 * \date 4-14-2012 22:12:26
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __IAPP_H__
#define __IAPP_H__

#include "ISingleton.h"
#include "../ui/style/IFontStyle.h"

class IApp : public ISingleton
{
public:
	RTTI_DEF(IApp, ISingleton);

	IApp();
	virtual ~IApp();

	virtual bool Initialize();
	virtual void Terminate();

	virtual void Update(float dt) = 0;
	virtual void Render() = 0;

	virtual void SystemUpdate(float dt);
	void Run();

private:
	void UpdateFps(float dt);

private:
	IFontStyle* m_pFontStyle;
	uint m_nFPSFrame;
	float m_fFPSTime;
	tstring m_strFPS;

};
#endif // __IAPP_H__
