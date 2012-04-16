/*!
 * \file ISingleton.h
 * \date 4-15-2012 14:09:41
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __ISINGLETON_H__
#define __ISINGLETON_H__

#include <type/BaseType.h>

class ISingleton
{
public:
	virtual bool Initialize() = 0;
	virtual void Terminate() = 0;

};
#endif // __ISINGLETON_H__
