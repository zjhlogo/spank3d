/*!
 * \file Rtti.cpp
 * \date 4-17-2012 23:25:32
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <core/Rtti.h>

Rtti::Rtti(const tstring& strClassName, const Rtti* pBaseRtti)
{
	m_strClassName = strClassName;
	m_pBaseRtti = pBaseRtti;
}

Rtti::~Rtti()
{
	// TODO: 
}

const tstring& Rtti::GetClassName() const
{
	return m_strClassName;
}

const Rtti* Rtti::GetBaseRtti() const
{
	return m_pBaseRtti;
}

bool Rtti::IsType(const tstring& strClassName) const
{
	return m_strClassName == strClassName;
}

bool Rtti::IsDerived(const tstring& strClassName) const
{
	const Rtti* pRtti = m_pBaseRtti;

	while (pRtti)
	{
		if (pRtti->GetClassName() == strClassName) return true;
		pRtti = pRtti->GetBaseRtti();
	}

	return false;
}
