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
#include <ui/style/BitmapFontStyle.h>

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
	if (!LoadBitmapStyleList(_("BitmapStyleList.xml"))) return false;
	if (!LoadNinePatchStyleList(_("NinePatchStyleList.xml"))) return false;
	if (!LoadHotizontalPatchStyleList(_("HorizontalPatchStyleList.xml"))) return false;
	if (!LoadVerticalPatchStyleList(_("VerticalPatchStyleList.xml"))) return false;
	if (!LoadBitmapFontStyleList(_("BitmapFontStyleList.xml"))) return false;

	return true;
}

void UiResMgr_Impl::Terminate()
{
	for (TM_VERTICAL_PATCH_STYLE::iterator it = m_VerticalPatchStyleMap.begin(); it != m_VerticalPatchStyleMap.end(); ++it)
	{
		VerticalPatchStyle* pVerticalPatchStyle = it->second;
		SAFE_DELETE(pVerticalPatchStyle);
	}
	m_VerticalPatchStyleMap.clear();

	for (TM_HORIZONTAL_PATCH_STYLE::iterator it = m_HorizontalPatchStyleMap.begin(); it != m_HorizontalPatchStyleMap.end(); ++it)
	{
		HorizontalPatchStyle* pHorizontalPatchStyle = it->second;
		SAFE_DELETE(pHorizontalPatchStyle);
	}
	m_HorizontalPatchStyleMap.clear();

	for (TM_NINE_PATCH_STYLE::iterator it = m_NinePatchStyleMap.begin(); it != m_NinePatchStyleMap.end(); ++it)
	{
		NinePatchStyle* pNinePatchStyle = it->second;
		SAFE_DELETE(pNinePatchStyle);
	}
	m_NinePatchStyleMap.clear();

	for (TM_BITMAP_STYLE::iterator it = m_BitmapStyleMap.begin(); it != m_BitmapStyleMap.end(); ++it)
	{
		BitmapStyle* pBitmapStyle = it->second;
		SAFE_DELETE(pBitmapStyle);
	}
	m_BitmapStyleMap.clear();

	for (TM_PIECE_INFO::iterator it = m_PieceInfoMap.begin(); it != m_PieceInfoMap.end(); ++it)
	{
		PieceInfo* pPieceInfo = it->second;
		SAFE_DELETE(pPieceInfo);
	}
	m_PieceInfoMap.clear();
}

PieceInfo* UiResMgr_Impl::FindPieceInfo(const tstring& strId)
{
	TM_PIECE_INFO::iterator itFound = m_PieceInfoMap.find(strId);
	if (itFound == m_PieceInfoMap.end()) return NULL;
	return itFound->second;
}

BitmapStyle* UiResMgr_Impl::FindBitmapStyle(const tstring& strId)
{
	TM_BITMAP_STYLE::iterator itFound = m_BitmapStyleMap.find(strId);
	if (itFound == m_BitmapStyleMap.end()) return NULL;
	return itFound->second;
}

NinePatchStyle* UiResMgr_Impl::FindNinePatchStyle(const tstring& strId)
{
	TM_NINE_PATCH_STYLE::iterator itFound = m_NinePatchStyleMap.find(strId);
	if (itFound == m_NinePatchStyleMap.end()) return NULL;
	return itFound->second;
}

HorizontalPatchStyle* UiResMgr_Impl::FindHorizontalPatchStyle(const tstring& strId)
{
	TM_HORIZONTAL_PATCH_STYLE::iterator itFound = m_HorizontalPatchStyleMap.find(strId);
	if (itFound == m_HorizontalPatchStyleMap.end()) return NULL;
	return itFound->second;
}

IFontStyle* UiResMgr_Impl::FindFontStyle(const tstring& strId)
{
	TM_FONT_STYLE::iterator itFound = m_FontStyleMap.find(strId);
	if (itFound == m_FontStyleMap.end()) return NULL;
	return itFound->second;
}

VerticalPatchStyle* UiResMgr_Impl::FindVerticalPatchStyle(const tstring& strId)
{
	TM_VERTICAL_PATCH_STYLE::iterator itFound = m_VerticalPatchStyleMap.find(strId);
	if (itFound == m_VerticalPatchStyleMap.end()) return NULL;
	return itFound->second;
}

bool UiResMgr_Impl::LoadPieceInfoList(const tstring& strFile)
{
	tstring strXmlData;
	if (!g_pResMgr->ReadStringFile(strXmlData, strFile)) return false;

	TiXmlDocument doc;
	doc.Parse(strXmlData.c_str());
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

bool UiResMgr_Impl::LoadBitmapStyleList(const tstring& strFile)
{
	tstring strXmlData;
	if (!g_pResMgr->ReadStringFile(strXmlData, strFile)) return false;

	TiXmlDocument doc;
	doc.Parse(strXmlData.c_str());
	if (doc.Error()) return false;

	// parse the xml files
	TiXmlElement* pXmlBitmapStyleList = doc.RootElement();
	if (!pXmlBitmapStyleList) return false;

	for (TiXmlElement* pXmlBitmapStyle = pXmlBitmapStyleList->FirstChildElement(_("BitmapStyle")); pXmlBitmapStyle != NULL; pXmlBitmapStyle = pXmlBitmapStyle->NextSiblingElement(_("BitmapStyle")))
	{
		const tchar* pszId = pXmlBitmapStyle->Attribute(_("id"));
		if (!pszId) continue;

		if (m_BitmapStyleMap.find(pszId) != m_BitmapStyleMap.end())
		{
			LOG(_("duplicate bitmap style id %s"), pszId);
			continue;
		}

		BitmapStyle* pBitmapStyle = new BitmapStyle(pszId);
		if (!pBitmapStyle->LoadFromXml(pXmlBitmapStyle))
		{
			SAFE_DELETE(pBitmapStyle);
			continue;
		}

		m_BitmapStyleMap.insert(std::make_pair(pBitmapStyle->GetId(), pBitmapStyle));
	}

	return true;
}

bool UiResMgr_Impl::LoadNinePatchStyleList(const tstring& strFile)
{
	tstring strXmlData;
	if (!g_pResMgr->ReadStringFile(strXmlData, strFile)) return false;

	TiXmlDocument doc;
	doc.Parse(strXmlData.c_str());
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

bool UiResMgr_Impl::LoadHotizontalPatchStyleList(const tstring& strFile)
{
	tstring strXmlData;
	if (!g_pResMgr->ReadStringFile(strXmlData, strFile)) return false;

	TiXmlDocument doc;
	doc.Parse(strXmlData.c_str());
	if (doc.Error()) return false;

	// parse the xml files
	TiXmlElement* pXmlHorizontalPatchStyleList = doc.RootElement();
	if (!pXmlHorizontalPatchStyleList) return false;

	for (TiXmlElement* pXmlHorizontalPatchStyle = pXmlHorizontalPatchStyleList->FirstChildElement(_("HorizontalPatchStyle")); pXmlHorizontalPatchStyle != NULL; pXmlHorizontalPatchStyle = pXmlHorizontalPatchStyle->NextSiblingElement(_("HorizontalPatchStyle")))
	{
		const tchar* pszId = pXmlHorizontalPatchStyle->Attribute(_("id"));
		if (!pszId) continue;

		if (m_HorizontalPatchStyleMap.find(pszId) != m_HorizontalPatchStyleMap.end())
		{
			LOG(_("duplicate horizontal patch style id %s"), pszId);
			continue;
		}

		HorizontalPatchStyle* pHorizontalPatchStyle = new HorizontalPatchStyle(pszId);
		if (!pHorizontalPatchStyle->LoadFromXml(pXmlHorizontalPatchStyle))
		{
			SAFE_DELETE(pHorizontalPatchStyle);
			continue;
		}

		m_HorizontalPatchStyleMap.insert(std::make_pair(pHorizontalPatchStyle->GetId(), pHorizontalPatchStyle));
	}

	return true;
}

bool UiResMgr_Impl::LoadVerticalPatchStyleList(const tstring& strFile)
{
	tstring strXmlData;
	if (!g_pResMgr->ReadStringFile(strXmlData, strFile)) return false;

	TiXmlDocument doc;
	doc.Parse(strXmlData.c_str());
	if (doc.Error()) return false;

	// parse the xml files
	TiXmlElement* pXmlVerticalPatchStyleList = doc.RootElement();
	if (!pXmlVerticalPatchStyleList) return false;

	for (TiXmlElement* pXmlVerticalPatchStyle = pXmlVerticalPatchStyleList->FirstChildElement(_("VerticalPatchStyle")); pXmlVerticalPatchStyle != NULL; pXmlVerticalPatchStyle = pXmlVerticalPatchStyle->NextSiblingElement(_("VerticalPatchStyle")))
	{
		const tchar* pszId = pXmlVerticalPatchStyle->Attribute(_("id"));
		if (!pszId) continue;

		if (m_VerticalPatchStyleMap.find(pszId) != m_VerticalPatchStyleMap.end())
		{
			LOG(_("duplicate vertical patch style id %s"), pszId);
			continue;
		}

		VerticalPatchStyle* pVerticalPatchStyle = new VerticalPatchStyle(pszId);
		if (!pVerticalPatchStyle->LoadFromXml(pXmlVerticalPatchStyle))
		{
			SAFE_DELETE(pVerticalPatchStyle);
			continue;
		}

		m_VerticalPatchStyleMap.insert(std::make_pair(pVerticalPatchStyle->GetId(), pVerticalPatchStyle));
	}

	return true;
}

bool UiResMgr_Impl::LoadBitmapFontStyleList(const tstring& strFile)
{
	tstring strXmlData;
	if (!g_pResMgr->ReadStringFile(strXmlData, strFile)) return false;

	TiXmlDocument doc;
	doc.Parse(strXmlData.c_str());
	if (doc.Error()) return false;

	// parse the xml files
	TiXmlElement* pXmlBitmapFontStyleList = doc.RootElement();
	if (!pXmlBitmapFontStyleList) return false;

	for (TiXmlElement* pXmlBitmapFontStyle = pXmlBitmapFontStyleList->FirstChildElement(_("BitmapFontStyle")); pXmlBitmapFontStyle != NULL; pXmlBitmapFontStyle = pXmlBitmapFontStyle->NextSiblingElement(_("BitmapFontStyle")))
	{
		const tchar* pszId = pXmlBitmapFontStyle->Attribute(_("id"));
		if (!pszId) continue;

		if (m_FontStyleMap.find(pszId) != m_FontStyleMap.end())
		{
			LOG(_("duplicate font style id %s"), pszId);
			continue;
		}

		BitmapFontStyle* pBitmapFontStyle = new BitmapFontStyle(pszId);

		const tchar* pszFile = pXmlBitmapFontStyle->Attribute(_("file"));
		if (!pBitmapFontStyle->LoadFontFile(pszFile))
		{
			SAFE_DELETE(pBitmapFontStyle);
			continue;
		}

		m_FontStyleMap.insert(std::make_pair(pBitmapFontStyle->GetId(), pBitmapFontStyle));
	}

	return true;
}
