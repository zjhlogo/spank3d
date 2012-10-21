/*!
 * \file IFontInfo.h
 * \date 10-20-2012 9:31:58
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __IFONTINFO_H__
#define __IFONTINFO_H__

#include "../../core/IObject.h"
#include "../../render/ITexture.h"
#include <vector>
#include <map>

class IFontInfo : public IObject
{
public:
	typedef std::vector<ITexture*> TV_TEXTURE;
	typedef std::map<uint, float> TM_UINT_FLOAT;

	typedef struct CHAR_INFO_tag
	{
		int id;
		int x;
		int y;
		int width;
		int height;
		float u;
		float v;
		float du;
		float dv;
		ITexture* pTexture;
		Vector2 offset;
		float advance;
	} CHAR_INFO;
	typedef std::map<int, CHAR_INFO> TM_CHAR_INFO;

public:
	RTTI_DEF(IFontInfo, IObject);

	IFontInfo(const tstring& strId):m_strId(strId) {};
	virtual ~IFontInfo() {};

	const tstring& GetId() const { return m_strId; };
	virtual float GetLineHeight() const = 0;
	virtual const CHAR_INFO* GetCharInfo(int ch) const = 0;
	virtual float GetKerning(int char1, int char2) const = 0;

private:
	tstring m_strId;

};
#endif // __IFONTINFO_H__
