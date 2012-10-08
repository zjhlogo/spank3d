/*!
 * \file UiResMgr_Impl.h
 * \date 9-20-2012 23:07:31
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __UIRESMGR_IMPL_H__
#define __UIRESMGR_IMPL_H__

#include <ui/IUiResMgr.h>
#include <map>

class UiResMgr_Impl : public IUiResMgr
{
public:
	typedef std::map<tstring, PieceInfo*> TM_PIECE_INFO;
	typedef std::map<tstring, BitmapStyle*> TM_BITMAP_STYLE;
	typedef std::map<tstring, NinePatchStyle*> TM_NINE_PATCH_STYLE;
	typedef std::map<tstring, HorizontalPatchStyle*> TM_HORIZONTAL_PATCH_STYLE;
	typedef std::map<tstring, VerticalPatchStyle*> TM_VERTICAL_PATCH_STYLE;
	typedef std::map<tstring, IFontStyle*> TM_FONT_STYLE;

public:
	RTTI_DEF(UiResMgr_Impl, IUiResMgr);

	static UiResMgr_Impl& GetInstance();

	virtual bool Initialize();
	virtual void Terminate();

	virtual PieceInfo* FindPieceInfo(const tstring& strId);
	virtual BitmapStyle* FindBitmapStyle(const tstring& strId);
	virtual NinePatchStyle* FindNinePatchStyle(const tstring& strId);
	virtual VerticalPatchStyle* FindVerticalPatchStyle(const tstring& strId);
	virtual HorizontalPatchStyle* FindHorizontalPatchStyle(const tstring& strId);
	virtual IFontStyle* FindFontStyle(const tstring& strId);

private:
	UiResMgr_Impl();
	virtual ~UiResMgr_Impl();

	bool LoadPieceInfoList(const tstring& strFile);
	bool LoadBitmapStyleList(const tstring& strFile);
	bool LoadNinePatchStyleList(const tstring& strFile);
	bool LoadHotizontalPatchStyleList(const tstring& strFile);
	bool LoadVerticalPatchStyleList(const tstring& strFile);
	bool LoadBitmapFontStyleList(const tstring& strFile);

private:
	TM_PIECE_INFO m_PieceInfoMap;
	TM_BITMAP_STYLE m_BitmapStyleMap;
	TM_NINE_PATCH_STYLE m_NinePatchStyleMap;
	TM_HORIZONTAL_PATCH_STYLE m_HorizontalPatchStyleMap;
	TM_VERTICAL_PATCH_STYLE m_VerticalPatchStyleMap;
	TM_FONT_STYLE m_FontStyleMap;

};
#endif // __UIRESMGR_IMPL_H__
