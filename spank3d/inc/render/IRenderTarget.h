/*!
 * \file IRenderTarget.h
 * \date 10-31-2012 13:41:35
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __IRENDERTARGET_H__
#define __IRENDERTARGET_H__

#include "../core/IObject.h"
#include "ITexture.h"

class IRenderTarget : public IObject
{
public:
	RTTI_DEF(IRenderTarget, IObject);

	IRenderTarget() {};
	virtual ~IRenderTarget() {};

	virtual bool SetTargetTexture(ITexture* pTexture) = 0;
	virtual ITexture* GetTargetTexture() = 0;

	virtual bool BeginRender() = 0;
	virtual void EndRender() = 0;

};
#endif // __IRENDERTARGET_H__
