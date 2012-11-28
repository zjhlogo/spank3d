/*!
 * \file XmlUtil.h
 * \date 11-27-2012 13:39:13
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __XMLUTIL_H__
#define __XMLUTIL_H__

#include <wx/xml/xml.h>

class XmlUtil
{
public:
	static int GetIntAttribute(wxXmlNode* pXml, const wxString& strName, int defaultValue = 0);
	static float GetFloatAttribute(wxXmlNode* pXml, const wxString& strName, float defaultValue = 0.0f);

	static void SetIntAttribute(wxXmlNode* pXml, const wxString& strName, int value);
	static void SetFloatAttribute(wxXmlNode* pXml, const wxString& strName, float value);

	static wxXmlNode* FindChildNode(wxXmlNode* pNode, const wxString& name);
	static wxXmlNode* FindNextSiblingNode(wxXmlNode* pNode, const wxString& name);

};
#endif // __XMLUTIL_H__
