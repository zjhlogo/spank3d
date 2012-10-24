/*!
 * \file IWindowParser.cpp
 * \date 10-23-2012 15:46:25
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <ui/parsers/IWindowParser.h>

IWindowParser::IWindowParser(const tstring& strClassName /*= _("IWindow")*/)
{
	m_strClassName = strClassName;
}

IWindowParser::~IWindowParser()
{
	// TODO: 
}

const tstring& IWindowParser::GetClassName() const
{
	return m_strClassName;
}

bool IWindowParser::CanParse(const tstring& strClassName)
{
	return m_strClassName == strClassName;
}

IWindow* IWindowParser::Parse(TiXmlElement* pXmlWindow, IWindow* pParent)
{
	IWindow* pWindow = CreateWindow(pParent);
	if (!pWindow) return NULL;

	if (!ParseWindow(pXmlWindow, pWindow))
	{
		SAFE_DELETE(pWindow);
		return NULL;
	}

	return pWindow;
}

bool IWindowParser::ParseWindow(TiXmlElement* pXmlWindow, IWindow* pWindow)
{
	tstring strTag = pXmlWindow->Attribute(_("id"));
	pWindow->SetTag(strTag);

	Vector2 pos;
	pXmlWindow->Attribute(_("x"), &pos.x);
	pXmlWindow->Attribute(_("y"), &pos.y);
	pWindow->SetPosition(pos);

	Vector2 size;
	pXmlWindow->Attribute(_("width"), &size.x);
	pXmlWindow->Attribute(_("height"), &size.y);
	pWindow->SetSize(size);

	const tchar* pszBgStyle = pXmlWindow->Attribute(_("bgstyle"));
	pWindow->SetBgStyle(pszBgStyle);

	return true;
}
