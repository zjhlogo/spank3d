/*!
 * \file UiResMgr_Impl.cpp
 * \date 9-20-2012 23:09:10
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "UiResMgr_Impl.h"
#include <Spank3d.h>
#include <tinyxml-2.6.2/tinyxml.h>
#include <util/LogUtil.h>

UiResMgr_Impl::UiResMgr_Impl()
{
	g_pUiResMgr = this;
}

UiResMgr_Impl::~UiResMgr_Impl()
{
	g_pUiResMgr = NULL;
}

UiResMgr_Impl& UiResMgr_Impl::GetInstance()
{
	static UiResMgr_Impl s_UiResMgr_Impl;
	return s_UiResMgr_Impl;
}

bool UiResMgr_Impl::Initialize()
{
	if (!LoadPieceInfoList(_("PieceInfoList.xml"))) return false;
	if (!LoadNinePatchStyleList(_("NinePatchStyleList.xml"))) return false;
	return true;
}

void UiResMgr_Impl::Terminate()
{
	// TODO: 
}

const PieceInfo* UiResMgr_Impl::FindPieceInfo(const tstring& strId)
{
	TM_PIECE_INFO::iterator itFound = m_PieceInfoMap.find(strId);
	if (itFound == m_PieceInfoMap.end()) return NULL;
	return itFound->second;
}

BitmapStyle* UiResMgr_Impl::FindBitmapStyle(const tstring& strId)
{
	// TODO: 
	return NULL;
}

NinePatchStyle* UiResMgr_Impl::FindNinePatchStyle(const tstring& strId)
{
	TM_NINE_PATCH_STYLE::iterator itFound = m_NinePatchStyleMap.find(strId);
	if (itFound == m_NinePatchStyleMap.end()) return NULL;
	return itFound->second;
}

VerticalPatchStyle* UiResMgr_Impl::FindVerticalPatchStyle(const tstring& strId)
{
	// TODO: 
	return NULL;
}

HorizontalPatchStyle* UiResMgr_Impl::FindHorizontalPatchStyle(const tstring& strId)
{
	// TODO: 
	return NULL;
}

bool UiResMgr_Impl::LoadPieceInfoList(const tstring& strFile)
{
	tstring strXmlFile;
	if (!g_pResMgr->ReadStringFile(strXmlFile, strFile)) return false;

	TiXmlDocument doc;
	doc.Parse(strXmlFile.c_str());
	if (doc.Error()) return false;

	// parse the xml files
	TiXmlElement* pXmlPieceInfoList = doc.RootElement();
	if (!pXmlPieceInfoList) return false;

	for (TiXmlElement* pXmlPieceInfo = pXmlPieceInfoList->FirstChildElement(_("PieceInfo")); pXmlPieceInfo != NULL; pXmlPieceInfo = pXmlPieceInfo->NextSiblingElement(_("PieceInfo")))
	{
		const tchar* pszId = pXmlPieceInfo->Attribute(_("id"));
		if (!pszId) continue;

		if (m_PieceInfoMap.find(pszId) != m_PieceInfoMap.end())
		{
			LOG(_("duplicate piece info id %s"), pszId);
			continue;
		}

		PieceInfo* pPieceInfo = new PieceInfo(pszId);
		if (!pPieceInfo->LoadFromXml(pXmlPieceInfo))
		{
			SAFE_DELETE(pPieceInfo);
			continue;
		}

		m_PieceInfoMap.insert(std::make_pair(pPieceInfo->GetId(), pPieceInfo));
	}

	return true;
}

bool UiResMgr_Impl::LoadNinePatchStyleList(const tstring& strFile)
{
	tstring strXmlFile;
	if (!g_pResMgr->ReadStringFile(strXmlFile, strFile)) return false;

	TiXmlDocument doc;
	doc.Parse(strXmlFile.c_str());
	if (doc.Error()) return false;

	// parse the xml files
	TiXmlElement* pXmlNinePatchStyleList = doc.RootElement();
	if (!pXmlNinePatchStyleList) return false;

	for (TiXmlElement* pXmlNinePatchStyle = pXmlNinePatchStyleList->FirstChildElement(_("NinePatchStyle")); pXmlNinePatchStyle != NULL; pXmlNinePatchStyle = pXmlNinePatchStyle->NextSiblingElement(_("NinePatchStyle")))
	{
		const tchar* pszId = pXmlNinePatchStyle->Attribute(_("id"));
		if (!pszId) continue;

		if (m_NinePatchStyleMap.find(pszId) != m_NinePatchStyleMap.end())
		{
			LOG(_("duplicate nine patch style id %s"), pszId);
			continue;
		}

		NinePatchStyle* pNinePatchStyle = new NinePatchStyle(pszId);
		if (!pNinePatchStyle->LoadFromXml(pXmlNinePatchStyle))
		{
			SAFE_DELETE(pNinePatchStyle);
			continue;
		}

		m_NinePatchStyleMap.insert(std::make_pair(pNinePatchStyle->GetId(), pNinePatchStyle));
	}

	return true;
}
