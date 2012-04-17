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

class IApp : public ISingleton
{
public:
	RTTI_DEF(IApp, ISingleton);

	IApp();
	virtual ~IApp();

	virtual void Update(float dt) = 0;

	void Run();

};
#endif // __IAPP_H__
