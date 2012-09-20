/*!
 * \file NinePatchStyle.cpp
 * \date 9-20-2012 9:00:26
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <ui/style/NinePatchStyle.h>
#include <ui/UiState.h>
#include <Spank3d.h>

NinePatchStyle::NinePatchStyle(const tstring& id)
:IGraphicsStyle(id)
{
	// TODO: 
}

NinePatchStyle::~NinePatchStyle()
{
	// TODO: 
}

bool NinePatchStyle::Render(const Vector2& pos, const Vector2& size)
{
	// TODO: 
	return true;
}

bool NinePatchStyle::LoadFromXml(TiXmlElement* pXmlNinePatchStyle)
{
	int minW = 0;
	int minH = 0;
	if (!pXmlNinePatchStyle->Attribute(_("minW"), &minW) || !pXmlNinePatchStyle->Attribute(_("minH"), &minH)) return false;
	SetMinSize(float(minW), float(minH));

	int maxW = 0;
	int maxH = 0;
	if (pXmlNinePatchStyle->Attribute(_("maxW"), &maxW) && pXmlNinePatchStyle->Attribute(_("maxH"), &maxH))
	{
		SetMaxSize(float(maxW), float(maxH));
	}
	else
	{
		SetMaxSize(float(INT_MAX), float(INT_MAX));
	}

	int bestW = 0;
	int bestH = 0;
	if (pXmlNinePatchStyle->Attribute(_("bestW"), &bestW) && pXmlNinePatchStyle->Attribute(_("bestH"), &bestH))
	{
		SetBestSize(float(bestW), float(bestH));
	}
	else
	{
		SetBestSize(float(minW), float(minH));
	}

	int paddingL = 0;
	int paddingT = 0;
	int paddingR = 0;
	int paddingB = 0;
	pXmlNinePatchStyle->Attribute(_("paddingL"), &paddingL);
	pXmlNinePatchStyle->Attribute(_("paddingT"), &paddingT);
	pXmlNinePatchStyle->Attribute(_("paddingR"), &paddingR);
	pXmlNinePatchStyle->Attribute(_("paddingB"), &paddingB);
	SetPaddingLT(float(paddingL), float(paddingT));
	SetPaddingLT(float(paddingR), float(paddingB));

	STATE_INFO stateInfo;
	TiXmlElement* pXmlState = pXmlNinePatchStyle->FirstChildElement(_("State"));
	while (pXmlState)
	{
		const char* pszId = pXmlState->Attribute(_("id"));
		const char* pszPieceId = pXmlState->Attribute(_("piece"));

		if (pszId && pszPieceId)
		{
			stateInfo.nState = UiState::GetStateValue(pszId);
			stateInfo.pPieceInfo = g_pUiResMgr->FindPieceInfo(pszPieceId);
			m_vStateInfo.push_back(stateInfo);
		}

		pXmlState = pXmlState->NextSiblingElement(_("State"));
	}

	return true;
}
