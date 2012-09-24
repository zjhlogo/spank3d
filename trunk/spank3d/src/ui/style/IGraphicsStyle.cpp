/*!
 * \file IGraphicsStyle.cpp
 * \date 9-20-2012 7:28:07
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <ui/style/IGraphicsStyle.h>

IGraphicsStyle::IGraphicsStyle(const tstring& id)
:IStyle(id)
{
	// TODO: 
}

IGraphicsStyle::~IGraphicsStyle()
{
	// TODO: 
}

void IGraphicsStyle::SetPaddingLT(const Vector2& paddingLT)
{
	SetPaddingLT(paddingLT.x, paddingLT.y);
}

void IGraphicsStyle::SetPaddingLT(float paddingL, float paddingT)
{
	m_PaddingLT.x = paddingL;
	m_PaddingLT.y = paddingT;
}

const Vector2& IGraphicsStyle::GetPaddingLT() const
{
	return m_PaddingLT;
}

void IGraphicsStyle::SetPaddingRB(const Vector2& paddingRB)
{
	SetPaddingRB(paddingRB.x, paddingRB.y);
}

void IGraphicsStyle::SetPaddingRB(float paddingR, float paddingB)
{
	m_PaddingRB.x = paddingR;
	m_PaddingRB.y = paddingB;
}

const Vector2& IGraphicsStyle::GetPaddingRB() const
{
	return m_PaddingRB;
}

void IGraphicsStyle::SetMinSize(const Vector2& minSize)
{
	SetMinSize(minSize.x, minSize.y);
}

void IGraphicsStyle::SetMinSize(float width, float height)
{
	m_MinSize.x = width;
	m_MinSize.y = height;
}

const Vector2& IGraphicsStyle::GetMinSize() const
{
	return m_MinSize;
}

void IGraphicsStyle::SetMaxSize(const Vector2& maxSize)
{
	SetMaxSize(maxSize.x, maxSize.y);
}

void IGraphicsStyle::SetMaxSize(float width, float height)
{
	m_MaxSize.x = width;
	m_MaxSize.y = height;
}

const Vector2& IGraphicsStyle::GetMaxSize() const
{
	return m_MaxSize;
}

void IGraphicsStyle::SetBestSize(const Vector2& bestSize)
{
	SetBestSize(bestSize.x, bestSize.y);
}

void IGraphicsStyle::SetBestSize(float width, float height)
{
	m_BestSize.x = width;
	m_BestSize.y = height;
}

const Vector2& IGraphicsStyle::GetBestSize() const
{
	return m_BestSize;
}

bool IGraphicsStyle::LoadFromXml(TiXmlElement* pXmlGraphicsStyle)
{
	int minW = 0;
	int minH = 0;
	if (!pXmlGraphicsStyle->Attribute(_("minW"), &minW) || !pXmlGraphicsStyle->Attribute(_("minH"), &minH)) return false;
	SetMinSize(float(minW), float(minH));

	int maxW = 0;
	int maxH = 0;
	if (pXmlGraphicsStyle->Attribute(_("maxW"), &maxW) && pXmlGraphicsStyle->Attribute(_("maxH"), &maxH))
	{
		SetMaxSize(float(maxW), float(maxH));
	}
	else
	{
		SetMaxSize(float(INT_MAX), float(INT_MAX));
	}

	int bestW = 0;
	int bestH = 0;
	if (pXmlGraphicsStyle->Attribute(_("bestW"), &bestW) && pXmlGraphicsStyle->Attribute(_("bestH"), &bestH))
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
	pXmlGraphicsStyle->Attribute(_("paddingL"), &paddingL);
	pXmlGraphicsStyle->Attribute(_("paddingT"), &paddingT);
	pXmlGraphicsStyle->Attribute(_("paddingR"), &paddingR);
	pXmlGraphicsStyle->Attribute(_("paddingB"), &paddingB);
	SetPaddingLT(float(paddingL), float(paddingT));
	SetPaddingLT(float(paddingR), float(paddingB));

	return true;
}
