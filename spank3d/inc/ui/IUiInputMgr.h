/*!
 * \file IUiInputMgr.h
 * \date 10-10-2012 9:29:12
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __IUIINPUTMGR_H__
#define __IUIINPUTMGR_H__

#include "../core/ISingleton.h"

class IUiInputMgr : public ISingleton
{
public:
	RTTI_DEF(IUiInputMgr, ISingleton);

	IUiInputMgr() {};
	virtual ~IUiInputMgr() {};

};
#endif // __IUIINPUTMGR_H__
