/*!
 * \file PieceInfo.cpp
 * \date 9-20-2012 14:08:58
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <ui/style/PieceInfo.h>
#include <Spank3d.h>

PieceInfo::PieceInfo(const tstring& id)
{
	strId = id;

	x = 0;
	y = 0;
	width = 0;
	height = 0;
	u = 0.0f;
	v = 0.0f;
	du = 0.0f;
	dv = 0.0f;
	pTexture = NULL;
}

PieceInfo::~PieceInfo()
{
	SAFE_RELEASE(pTexture);
}

const tstring& PieceInfo::GetId() const
{
	return strId;
}

bool PieceInfo::LoadFromXml(TiXmlElement* pXmlPieceInfo)
{
	const tchar* pszTexture = pXmlPieceInfo->Attribute(_("texture"));
	if (!pszTexture) return false;

	pTexture = g_pResMgr->CreateTexture(pszTexture);
	if (!pTexture) return false;

	pXmlPieceInfo->Attribute(_("x"), &x);
	pXmlPieceInfo->Attribute(_("y"), &y);
	pXmlPieceInfo->Attribute(_("width"), &width);
	pXmlPieceInfo->Attribute(_("height"), &height);

	const Vector2& size = pTexture->GetSize();

	u = x / size.x;
	v = y / size.y;
	du = width / size.x;
	dv = height / size.y;

	return true;
}
