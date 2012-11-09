/*!
 * \file Picture.cpp
 * \date 11-01-2012 15:31:12
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <ui/controls/Picture.h>
#include <Spank3d.h>

Picture::Picture(IWindow* pParent)
:IWindow(pParent)
{
	m_pTexture = NULL;
}

Picture::~Picture()
{
	// TODO: 
}

void Picture::SetTexture(ITexture* pTexture)
{
	m_pTexture = pTexture;

	// auto size
	Vector2 paddingSize = m_pBgStyle->GetPaddingLT()+m_pBgStyle->GetPaddingRB();
	SetSize(m_pTexture->GetSize()+paddingSize);
}

ITexture* Picture::GetTexture()
{
	return m_pTexture;
}

bool Picture::Render(const Vector2& basePos, const Rect& clipRect, uint state)
{
	if (!IWindow::Render(basePos, clipRect, state)) return true;
	if (!m_pTexture) return true;

	const Vector2& paddingLT = m_pBgStyle->GetPaddingLT();
	const Vector2& paddingRB = m_pBgStyle->GetPaddingRB();
	const Vector2& texSize = m_pTexture->GetSize();

	Vector2 clientSize = m_Size - paddingLT - paddingRB;
	float scaleX = clientSize.x / texSize.x;
	float scaleY = clientSize.y / texSize.y;
	float scale = scaleX < scaleY ? scaleX : scaleY;

	g_pUiRenderer->DrawRect(basePos+m_Position+paddingLT, texSize*scale, 0xFFFFFFFF, clipRect, m_pTexture);

	return true;
}
