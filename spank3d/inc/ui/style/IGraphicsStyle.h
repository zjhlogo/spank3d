/*!
 * \file IGraphicsStyle.h
 * \date 9-20-2012 7:28:01
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __IGRAPHICSSTYLE_H__
#define __IGRAPHICSSTYLE_H__

#include "IStyle.h"
#include "../../math/Rect.h"
#include "../../math/Vector2.h"
#include "PieceInfo.h"
#include <vector>
#include <tinyxml-2.6.2/tinyxml.h>

class IGraphicsStyle : public IStyle
{
public:
	RTTI_DEF(IGraphicsStyle, IStyle);

	IGraphicsStyle(const tstring& id);
	virtual ~IGraphicsStyle();

	void SetPaddingLT(const Vector2& paddingLT);
	void SetPaddingLT(float paddingL, float paddingT);
	const Vector2& GetPaddingLT() const;

	void SetPaddingRB(const Vector2& paddingRB);
	void SetPaddingRB(float paddingR, float paddingB);
	const Vector2& GetPaddingRB() const;

	void SetMinSize(const Vector2& minSize);
	void SetMinSize(float width, float height);
	const Vector2& GetMinSize() const;

	void SetMaxSize(const Vector2& maxSize);
	void SetMaxSize(float width, float height);
	const Vector2& GetMaxSize() const;

	void SetBestSize(const Vector2& bestSize);
	void SetBestSize(float width, float height);
	const Vector2& GetBestSize() const;

	virtual bool Render(const Vector2& pos, const Vector2& size, const Rect& clipRect, uint state) = 0;

protected:
	bool LoadFromXml(TiXmlElement* pXmlGraphicsStyle);

private:
	Vector2 m_PaddingLT;
	Vector2 m_PaddingRB;

	Vector2 m_MinSize;
	Vector2 m_MaxSize;
	Vector2 m_BestSize;

};
#endif // __IGRAPHICSSTYLE_H__
