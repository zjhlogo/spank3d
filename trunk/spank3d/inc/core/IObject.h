/*!
 * \file IObject.h
 * \date 4-17-2012 23:52:45
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __IOBJECT_H__
#define __IOBJECT_H__

#include "../event/IEventDispatcher.h"

class IObject : public IEventDispatcher
{
public:
	RTTI_DEF(IObject, IEventDispatcher);

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
