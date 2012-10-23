/*!
 * \file PushButtonParser.h
 * \date 10-23-2012 16:08:06
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __PUSHBUTTONPARSER_H__
#define __PUSHBUTTONPARSER_H__

#include "IWindowParser.h"

class PushButtonParser : public IWindowParser
{
public:
	RTTI_DEF(PushButtonParser, IWindowParser);

	PushButtonParser(const tstring& strClassName = _("PushButton"));
	virtual ~PushButtonParser();

	virtual bool ParseWindow(TiXmlElement* pXmlWindow, IWindow* pWindow);
	virtual IWindow* CreateWindow(IWindow* pParent);

};
#endif // __PUSHBUTTONPARSER_H__
