/*!
 * \file IStyle.cpp
 * \date 9-19-2012 22:36:20
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <ui/style/IStyle.h>

IStyle::IStyle(const tstring& id)
{
	m_strId = id;
}

IStyle::~IStyle()
{
	// TODO: 
}

const tstring& IStyle::GetId() const
{
	return m_strId;
}
