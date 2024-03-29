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
#include <ui/style/BitmapFontInfo.h>
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
	tstring strXmlData;
	if (!g_pResMgr->ReadStringFile(strXmlData, _("UiRes.xml"))) return false;

	TiXmlDocument doc;
	doc.Parse(strXmlData.c_str());
	if (doc.Error()) return false;

	TiXmlElement* pXmlRoot = doc.RootElement();

	for (TiXmlElement* pXmlPieceInfo = pXmlRoot->FirstChildElement(_("PieceInfo")); pXmlPieceInfo != NULL; pXmlPieceInfo = pXmlPieceInfo->NextSiblingElement(_("PieceInfo")))
	{
		tstring strPath = pXmlPieceInfo->Attribute(_("path"));
		if (!LoadPieceInfoList(strPath)) return false;
	}

	for (TiXmlElement* pXmlBitmapFont = pXmlRoot->FirstChildElement(_("BitmapFont")); pXmlBitmapFont != NULL; pXmlBitmapFont = pXmlBitmapFont->NextSiblingElement(_("BitmapFont")))
	{
		tstring strPath = pXmlBitmapFont->Attribute(_("path"));
		if (!LoadBitmapFontInfo(strPath)) return false;
	}

	for (TiXmlElement* pXmlBitmapStyleList = pXmlRoot->FirstChildElement(_("BitmapStyle")); pXmlBitmapStyleList != NULL; pXmlBitmapStyleList = pXmlBitmapStyleList->NextSiblingElement(_("BitmapStyle")))
	{
		tstring strPath = pXmlBitmapStyleList->Attribute(_("path"));
		if (!LoadBitmapStyleList(strPath)) return false;
	}

	for (TiXmlElement* pXmlNinePatchStyleList = pXmlRoot->FirstChildElement(_("NinePatchStyle")); pXmlNinePatchStyleList != NULL; pXmlNinePatchStyleList = pXmlNinePatchStyleList->NextSiblingElement(_("NinePatchStyle")))
	{
		tstring strPath = pXmlNinePatchStyleList->Attribute(_("path"));
		if (!LoadNinePatchStyleList(strPath)) return false;
	}

	for (TiXmlElement* pXmlHorizontalPatchStyleList = pXmlRoot->FirstChildElement(_("HorizontalPatchStyle")); pXmlHorizontalPatchStyleList != NULL; pXmlHorizontalPatchStyleList = pXmlHorizontalPatchStyleList->NextSiblingElement(_("HorizontalPatchStyle")))
	{
		tstring strPath = pXmlHorizontalPatchStyleList->Attribute(_("path"));
		if (!LoadHorizontalPatchStyleList(strPath)) return false;
	}

	for (TiXmlElement* pXmlVerticalPatchStyleList = pXmlRoot->FirstChildElement(_("VerticalPatchStyle")); pXmlVerticalPatchStyleList != NULL; pXmlVerticalPatchStyleList = pXmlVerticalPatchStyleList->NextSiblingElement(_("VerticalPatchStyle")))
	{
		tstring strPath = pXmlVerticalPatchStyleList->Attribute(_("path"));
		if (!LoadVerticalPatchStyleList(strPath)) return false;
	}

	for (TiXmlElement* pXmlBitmapFontStyleList = pXmlRoot->FirstChildElement(_("BitmapFontStyle")); pXmlBitmapFontStyleList != NULL; pXmlBitmapFontStyleList = pXmlBitmapFontStyleList->NextSiblingElement(_("BitmapFontStyle")))
	{
		tstring strPath = pXmlBitmapFontStyleList->Attribute(_("path"));
		if (!LoadBitmapFontStyleList(strPath)) return false;
	}

	return true;
}

void UiResMgr_Impl::Terminate()
{
	m_GraphicsStyleMap.clear();

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

	for (TM_FONT_INFO::iterator it = m_FontInfoMap.begin(); it != m_FontInfoMap.end(); ++it)
	{
		IFontInfo* pFontInfo = it->second;
		SAFE_DELETE(pFontInfo);
	}
	m_FontInfoMap.clear();

	for (TM_PIECE_INFO::iterator it = m_PieceInfoMap.begin(); it != m_PieceInfoMap.end(); ++it)
	{
		PieceInfo* pPieceInfo = it->second;
		SAFE_DELETE(pPieceInfo);
	}
	m_PieceInfoMap.clear();

	for (TV_TEXTURE::iterator it = m_vUiTextures.begin(); it != m_vUiTextures.end(); ++it)
	{
		ITexture* pTexture = (*it);
		SAFE_RELEASE(pTexture);
	}
	m_vUiTextures.clear();
}

PieceInfo* UiResMgr_Impl::FindPieceInfo(const tstring& strId)
{
	TM_PIECE_INFO::iterator itFound = m_PieceInfoMap.find(strId);
	if (itFound == m_PieceInfoMap.end()) return NULL;
	return itFound->second;
}

IGraphicsStyle* UiResMgr_Impl::FindStyle(const tstring& strId)
{
	TM_GRAPHICS_STYLE::iterator itFound = m_GraphicsStyleMap.find(strId);
	if (itFound == m_GraphicsStyleMap.end()) return NULL;
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

IFontInfo* UiResMgr_Impl::FindFontInfo(const tstring& strId)
{
	TM_FONT_INFO::iterator itFound = m_FontInfoMap.find(strId);
	if (itFound == m_FontInfoMap.end()) return NULL;
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

	const tchar* pszTexture = pXmlPieceInfoList->Attribute(_("texture"));
	if (!pszTexture) return false;

	ITexture* pTexture = g_pResMgr->CreateTexture2D(pszTexture);
	if (!pTexture) return false;
	pTexture->SetFilter(TEXTURE_FILTER::TF_NEAREST);

	m_vUiTextures.push_back(pTexture);

	for (TiXmlElement* pXmlPieceInfo = pXmlPieceInfoList->FirstChildElement(_("PieceInfo")); pXmlPieceInfo != NULL; pXmlPieceInfo = pXmlPieceInfo->NextSiblingElement(_("PieceInfo")))
	{
		const tchar* pszId = pXmlPieceInfo->Attribute(_("id"));
		if (!pszId) continue;

		if (m_PieceInfoMap.find(pszId) != m_PieceInfoMap.end())
		{
			LOG(_T("duplicate piece info id %s"), pszId);
			continue;
		}

		PieceInfo* pPieceInfo = new PieceInfo(pszId);
		if (!pPieceInfo->FromXml(pXmlPieceInfo, pTexture))
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

		if (FindStyle(pszId))
		{
			LOG(_T("duplicate bitmap style id %s"), pszId);
			continue;
		}

		BitmapStyle* pBitmapStyle = new BitmapStyle(pszId);
		if (!pBitmapStyle->FromXml(pXmlBitmapStyle))
		{
			SAFE_DELETE(pBitmapStyle);
			continue;
		}

		m_BitmapStyleMap.insert(std::make_pair(pBitmapStyle->GetId(), pBitmapStyle));
		m_GraphicsStyleMap.insert(std::make_pair(pBitmapStyle->GetId(), pBitmapStyle));
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

		if (FindStyle(pszId))
		{
			LOG(_T("duplicate nine patch style id %s"), pszId);
			continue;
		}

		NinePatchStyle* pNinePatchStyle = new NinePatchStyle(pszId);
		if (!pNinePatchStyle->FromXml(pXmlNinePatchStyle))
		{
			SAFE_DELETE(pNinePatchStyle);
			continue;
		}

		m_NinePatchStyleMap.insert(std::make_pair(pNinePatchStyle->GetId(), pNinePatchStyle));
		m_GraphicsStyleMap.insert(std::make_pair(pNinePatchStyle->GetId(), pNinePatchStyle));
	}

	return true;
}

bool UiResMgr_Impl::LoadHorizontalPatchStyleList(const tstring& strFile)
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

		if (FindStyle(pszId))
		{
			LOG(_T("duplicate horizontal patch style id %s"), pszId);
			continue;
		}

		HorizontalPatchStyle* pHorizontalPatchStyle = new HorizontalPatchStyle(pszId);
		if (!pHorizontalPatchStyle->FromXml(pXmlHorizontalPatchStyle))
		{
			SAFE_DELETE(pHorizontalPatchStyle);
			continue;
		}

		m_HorizontalPatchStyleMap.insert(std::make_pair(pHorizontalPatchStyle->GetId(), pHorizontalPatchStyle));
		m_GraphicsStyleMap.insert(std::make_pair(pHorizontalPatchStyle->GetId(), pHorizontalPatchStyle));
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

		if (FindStyle(pszId))
		{
			LOG(_T("duplicate vertical patch style id %s"), pszId);
			continue;
		}

		VerticalPatchStyle* pVerticalPatchStyle = new VerticalPatchStyle(pszId);
		if (!pVerticalPatchStyle->FromXml(pXmlVerticalPatchStyle))
		{
			SAFE_DELETE(pVerticalPatchStyle);
			continue;
		}

		m_VerticalPatchStyleMap.insert(std::make_pair(pVerticalPatchStyle->GetId(), pVerticalPatchStyle));
		m_GraphicsStyleMap.insert(std::make_pair(pVerticalPatchStyle->GetId(), pVerticalPatchStyle));
	}

	return true;
}

bool UiResMgr_Impl::LoadBitmapFontInfo(const tstring& strFile)
{
	tstring fontInfoId;
	StringUtil::GetFileNameWithoutExt(fontInfoId, strFile);

	if (FindFontInfo(fontInfoId))
	{
		LOG(_T("duplicate bitmap font style id %s"), fontInfoId.c_str());
		return false;
	}

	BitmapFontInfo* pBitmapFontInfo = new BitmapFontInfo(fontInfoId);
	if (!pBitmapFontInfo->LoadFromFile(strFile))
	{
		SAFE_DELETE(pBitmapFontInfo);
		LOG(_T("can not load bitmap font info %s"), strFile.c_str());
		return false;
	}

	m_FontInfoMap.insert(std::make_pair(fontInfoId, pBitmapFontInfo));
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
			LOG(_T("duplicate font style id %s"), pszId);
			continue;
		}

		BitmapFontStyle* pBitmapFontStyle = new BitmapFontStyle(pszId);
		if (!pBitmapFontStyle->FromXml(pXmlBitmapFontStyle))
		{
			SAFE_DELETE(pBitmapFontStyle);
			continue;
		}

		m_FontStyleMap.insert(std::make_pair(pBitmapFontStyle->GetId(), pBitmapFontStyle));
	}

	return true;
}
