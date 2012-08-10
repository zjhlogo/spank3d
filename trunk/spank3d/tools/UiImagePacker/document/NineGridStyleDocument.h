/*!
 * \file NineGridStyleDocument.h
 * \date 2-14-2012 15:24:06
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __NINEGRIDSTYLEDOCUMENT_H__
#define __NINEGRIDSTYLEDOCUMENT_H__

#include "DocumentBase.h"
#include "NineGridStyle.h"
#include <map>
#include <vector>

class NineGridStyleDocument : public DocumentBase
{
public:
	typedef std::map<wxString, NineGridStyle*> TM_NINE_GRID_STYLE;
	typedef std::vector<const NineGridStyle*> TV_NINE_GRID_STYLE;

public:
	virtual ~NineGridStyleDocument();
	static NineGridStyleDocument& GetInstance();

	virtual bool OpenFile(const wxString& strFile);
	virtual bool SaveFile(const wxString& strFile);
	virtual void Reset();

	const NineGridStyle* FindNineGridStyle(const wxString& strId);
	const TM_NINE_GRID_STYLE& GetNineGridStyleMap();
	int EnumNineGridStyles(TV_NINE_GRID_STYLE& vNineGridStyleOut, const PieceInfo* pPieceInfo);

	bool RenameNineGridStyleId(const NineGridStyle* pNineGridStyle, const wxString& strNewId);
	bool SetAutoGenBitmap(const NineGridStyle* pNineGridStyle, bool bAutoGenBitmap);
	bool SetStatePiece(const NineGridStyle* pNineGridStyle, const PieceInfo* pPieceInfo, IStyle::STYLE_STATE eState);
	bool SetStateMinX(const NineGridStyle* pNineGridStyle, int value, IStyle::STYLE_STATE eState);
	bool SetStateMinY(const NineGridStyle* pNineGridStyle, int value, IStyle::STYLE_STATE eState);
	bool SetStateMaxX(const NineGridStyle* pNineGridStyle, int value, IStyle::STYLE_STATE eState);
	bool SetStateMaxY(const NineGridStyle* pNineGridStyle, int value, IStyle::STYLE_STATE eState);

	const NineGridStyle* AddNineGridStyle(const wxString& strId);
	bool RemoveNineGridStyle(const wxString& strId);

protected:
	NineGridStyleDocument();
	NineGridStyle* InternalFindNineGridStyle(const wxString& strId);
	wxString GenerateNewNineGridStyleId(const wxString& strId);

private:
	TM_NINE_GRID_STYLE m_NineGridStyleMap;

};
#endif // __NINEGRIDSTYLEDOCUMENT_H__
