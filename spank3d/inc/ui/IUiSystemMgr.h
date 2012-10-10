/*!
 * \file IUiSystemMgr.h
 * \date 10-10-2012 9:13:43
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __IUISYSTEMMGR_H__
#define __IUISYSTEMMGR_H__

#include "../core/ISingleton.h"

class IUiSystemMgr : public ISingleton
{
public:
	RTTI_DEF(IUiSystemMgr, ISingleton);

	IUiSystemMgr() {};
	virtual ~IUiSystemMgr() {};

	virtual void Update(float dt) = 0;
	virtual void Render() = 0;

};
#endif // __IUISYSTEMMGR_H__
