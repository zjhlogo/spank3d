/*!
 * \file RenderInterface_Impl.cpp
 * \date 4-18-2012 21:47:33
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "RenderInterface_Impl.h"
#include "Shader_Impl.h"
#include <util/FileUtil.h>
#include <util/StringUtil.h>
#include <tinyxml-2.6.2/tinyxml.h>
#include <Spank3D.h>

RenderInterface_Impl::RenderInterface_Impl()
{
	g_pRenderInterface = this;
}

RenderInterface_Impl::~RenderInterface_Impl()
{
	g_pRenderInterface = NULL;
}

RenderInterface_Impl& RenderInterface_Impl::GetInstance()
{
	static RenderInterface_Impl s_RenderInterface_Impl;
	return s_RenderInterface_Impl;
}

bool RenderInterface_Impl::Initialize()
{
	// TODO: 
	return true;
}

void RenderInterface_Impl::Terminate()
{
	// TODO: 
}

IShader* RenderInterface_Impl::CreateShader(const tstring& strShaderFile)
{
	std::string strXmlFile;
	if (!FileUtil::ReadFileIntoString(strXmlFile, strShaderFile)) return NULL;

	TiXmlDocument doc;
	doc.Parse(strXmlFile.c_str());
	if (doc.Error()) return NULL;

	// parse the xml files
	TiXmlElement* pElmShader = doc.RootElement();
	if (!pElmShader) return NULL;

	// vertex shader
	const char* pszVertexShader = pElmShader->Attribute("vertex_shader");
	if (!pszVertexShader) return NULL;

	std::string strVertexShaderData;
	if (!FileUtil::ReadFileIntoString(strVertexShaderData, StringUtil::char2tchar(pszVertexShader))) return NULL;

	// fragment shader
	const char* pszFregmentShader = pElmShader->Attribute("fregment_shader");
	if (!pszFregmentShader) return NULL;

	std::string strFragmentShaderData;
	if (!FileUtil::ReadFileIntoString(strFragmentShaderData, StringUtil::char2tchar(pszFregmentShader))) return NULL;

	// vertex attribute
	TiXmlElement* pElmAttrs = pElmShader->FirstChildElement("attributes");
	if (!pElmAttrs) return NULL;

	VertexAttribute::ATTRIBUTE_ITEM attrItems[VertexAttribute::MAX_ATTRIBUTE_ITEMS+1];
	int nAttrIndex = 0;

	TiXmlElement* pElmAttr = pElmAttrs->FirstChildElement("attribute");
	while (pElmAttr)
	{
		int nSize = 0;
		pElmAttr->Attribute("size", &nSize);
		if (nSize <= 0) return NULL;

		const char* pszAttrName = pElmAttr->Attribute("name");
		if (!pszAttrName) return NULL;

		attrItems[nAttrIndex].nSize = nSize;
		attrItems[nAttrIndex].eItemType = VertexAttribute::AIT_FLOAT;
		strncpy_s(attrItems[nAttrIndex].szParamName, pszAttrName, VertexAttribute::MAX_ATTRIBUTE_NAME_LENGTH);

		++nAttrIndex;
		pElmAttr = pElmAttr->NextSiblingElement("attribute");
	}

	if (nAttrIndex <= 0 || nAttrIndex > VertexAttribute::MAX_ATTRIBUTE_ITEMS) return NULL;

	attrItems[nAttrIndex].nSize = 0;
	attrItems[nAttrIndex].eItemType = VertexAttribute::AIT_UNKNOWN;
	attrItems[nAttrIndex].nOffset = 0;
	attrItems[nAttrIndex].szParamName[0] = '\0';

	return InternalCreateShader(strVertexShaderData, strFragmentShaderData, attrItems);
}

IShader* RenderInterface_Impl::InternalCreateShader(const tstring& strVertexShader, const tstring& strFragmentShader, const VertexAttribute::ATTRIBUTE_ITEM* pVertexAttrItem)
{
 	Shader_Impl* pShader = new Shader_Impl(strVertexShader.c_str(), strFragmentShader.c_str(), pVertexAttrItem);
 	if (!pShader || !pShader->IsOk())
 	{
 		SAFE_RELEASE(pShader);
 		return NULL;
 	}
 
 	return pShader;
}
