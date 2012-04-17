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
	m_nReference = 1;
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

int IObject::IncReference()
{
	return ++m_nReference;
}

int IObject::DecReference()
{
	return --m_nReference;
}

int IObject::GetReference() const
{
	return m_nReference;
}

void IObject::Release()
{
	if (DecReference() <= 0) delete this;
}
