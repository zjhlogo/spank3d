/*!
 * \file IResMgr.h
 * \date 4-19-2012 22:54:20
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __IRESMGR_H__
#define __IRESMGR_H__

#include "ISingleton.h"
#include "IMesh.h"

class IResMgr : public ISingleton
{
public:
	RTTI_DEF(IResMgr, ISingleton);

	IResMgr() {};
	virtual ~IResMgr() {};

	virtual IMesh* CreateMesh(const tstring& strFile) = 0;
	virtual void SetDefaultDir(const tstring& strDir) = 0;
	virtual const tstring& GetDefaultDir() = 0;
};

#endif // __IRESMGR_H__
