/*!
 * \file IRenderInterface.h
 * \date 4-18-2012 21:43:11
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __IRENDERINTERFACE_H__
#define __IRENDERINTERFACE_H__

#include "../core/ISingleton.h"
#include "IShader.h"

class IRenderInterface : public ISingleton
{
public:
	RTTI_DEF(IRenderInterface, ISingleton);

	IRenderInterface() {};
	virtual ~IRenderInterface() {};

	virtual IShader* CreateShader(const tstring& strShaderFile) = 0;

};
#endif // __IRENDERINTERFACE_H__
