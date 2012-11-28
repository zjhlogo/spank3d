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

	x = 0.0f;
	y = 0.0f;
	width = 0.0f;
	height = 0.0f;
	u = 0.0f;
	v = 0.0f;
	du = 0.0f;
	dv = 0.0f;
	pTexture = NULL;
}

PieceInfo::~PieceInfo()
{
	
}

const tstring& PieceInfo::GetId() const
{
	return strId;
}

bool PieceInfo::FromXml(TiXmlElement* pXmlPieceInfo, ITexture* pTex)
{
	pTexture = pTex;

	if (pXmlPieceInfo->QueryFloatAttribute(_("x"), &x) != TIXML_SUCCESS) return false;
	if (pXmlPieceInfo->QueryFloatAttribute(_("y"), &y) != TIXML_SUCCESS) return false;
	if (pXmlPieceInfo->QueryFloatAttribute(_("width"), &width) != TIXML_SUCCESS) return false;
	if (pXmlPieceInfo->QueryFloatAttribute(_("height"), &height) != TIXML_SUCCESS) return false;

	const Vector2& size = pTexture->GetSize();

	u = x / size.x;
	v = y / size.y;
	du = width / size.x;
	dv = height / size.y;

	return true;
}

TiXmlElement* PieceInfo::ToXml()
{
	TiXmlElement* pXmlPieceInfo = new TiXmlElement(_("PieceInfo"));
	pXmlPieceInfo->SetAttribute(_("id"), strId.c_str());
	pXmlPieceInfo->SetAttribute(_("x"), int(x));
	pXmlPieceInfo->SetAttribute(_("y"), int(y));
	pXmlPieceInfo->SetAttribute(_("width"), int(width));
	pXmlPieceInfo->SetAttribute(_("height"), int(height));
	return pXmlPieceInfo;
}
