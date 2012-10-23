/*!
 * \file IWindowParser.h
 * \date 10-23-2012 15:46:20
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __IWINDOWPARSER_H__
#define __IWINDOWPARSER_H__

#include "../../core/IObject.h"
#include "../controls/IWindow.h"
#include <tinyxml-2.6.2/tinyxml.h>

class IWindowParser : public IObject
{
public:
	RTTI_DEF(IWindowParser, IObject);

	IWindowParser(const tstring& strClassName = _("IWindow"));
	virtual ~IWindowParser();

	const tstring& GetClassName() const;
	bool CanParse(const tstring& strClassName);
	IWindow* Parse(TiXmlElement* pXmlWindow, IWindow* pParent);

	virtual bool ParseWindow(TiXmlElement* pXmlWindow, IWindow* pWindow);
	virtual IWindow* CreateWindow(IWindow* pParent) = 0;

private:
	tstring m_strClassName;

};
#endif // __IWINDOWPARSER_H__
