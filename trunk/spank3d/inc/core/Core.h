/*!
 * \file Core.h
 * \date 4-14-2012 22:16:20
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __CORE_H__
#define __CORE_H__

#include "ISingleton.h"

class Core : public ISingleton
{
public:
	RTTI_DEF(Core, ISingleton);

	Core();
	virtual ~Core();

	static Core& GetInstance();

	virtual bool Initialize();
	virtual void Terminate();

	void Run();
	void End();

};
#endif // __CORE_H__
