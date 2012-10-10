/*!
 * \file IObject.h
 * \date 4-17-2012 23:52:45
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __IOBJECT_H__
#define __IOBJECT_H__

#include "EventDispatcher.h"

class IObject : public EventDispatcher
{
public:
	RTTI_DEF(IObject, EventDispatcher);

	IObject();
	virtual ~IObject();

	void SetOk(bool bOk);
	bool IsOk() const;

	int IncRef();
	int DecRef();
	int GetRef() const;

	virtual void Release();

private:
	bool m_bOk;
	int m_nRef;

};
#endif // __IOBJECT_H__
