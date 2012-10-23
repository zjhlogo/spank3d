/*!
 * \file PushButtonParser.cpp
 * \date 10-23-2012 16:07:55
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <ui/parsers/PushButtonParser.h>
#include <ui/controls/PushButton.h>

PushButtonParser::PushButtonParser(const tstring& strClassName /*= _("PushButton")*/)
:IWindowParser(strClassName)
{
	// TODO: 
}

PushButtonParser::~PushButtonParser()
{
	// TODO: 
}

bool PushButtonParser::ParseWindow(TiXmlElement* pXmlWindow, IWindow* pWindow)
{
	if (!IWindowParser::ParseWindow(pXmlWindow, pWindow)) return false;

	PushButton* pPushButton = (PushButton*)pWindow;

	return true;
}

IWindow* PushButtonParser::CreateWindow(IWindow* pParent)
{
	return new PushButton(pParent);
}
