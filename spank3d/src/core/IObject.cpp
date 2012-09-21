/*!
 * \file IObject.cpp
 * \date 4-17-2012 23:52:52
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <core/IObject.h>

IObject::IObject()
{
	m_bOk = false;
	m_nRef = 1;
}

IObject::~IObject()
{
	// TODO: 
}

void IObject::SetOk(bool bOk)
{
	m_bOk = bOk;
}

bool IObject::IsOk() const
{
	return m_bOk;
}

int IObject::IncRef()
{
	return ++m_nRef;
}

int IObject::DecRef()
{
	return --m_nRef;
}

int IObject::GetRef() const
{
	return m_nRef;
}

void IObject::Release()
{
	if (DecRef() <= 0) delete this;
}
