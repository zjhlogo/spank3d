/*!
 * \file NineGridStyle.h
 * \date 2-14-2012 15:07:29
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __NINEGRIDSTYLE_H__
#define __NINEGRIDSTYLE_H__

#include "IStyle.h"
#include "PieceInfo.h"

class NineGridStyle : public IStyle
{
public:
	typedef struct NINE_GRID_INFO_tag
	{
		const PieceInfo* pPieceInfo;
		int min_x;
		int min_y;
		int max_x;
		int max_y;
	} NINE_GRID_INFO;

public:
	NineGridStyle();
	~NineGridStyle();

	bool LoadFromXml(wxXmlNode* pNodeNineGridStyle);
	bool SaveToXml(wxXmlNode* pNodeNineGridStyleList);

	bool SetStatePiece(const PieceInfo* pPieceInfo, STYLE_STATE eState);
	bool SetStateMinX(int value, IStyle::STYLE_STATE eState);
	bool SetStateMinY(int value, IStyle::STYLE_STATE eState);
	bool SetStateMaxX(int value, IStyle::STYLE_STATE eState);
	bool SetStateMaxY(int value, IStyle::STYLE_STATE eState);

	const NINE_GRID_INFO* GetStateGridInfo(STYLE_STATE eState) const;

private:
	bool LoadStateInfo(NINE_GRID_INFO& NineGridInfoOut, wxXmlNode* pNodeNineGridStyle, const wxString& strState);
	bool SaveStateInfo(wxXmlNode* pNodeNineGridStyle, const wxString& strState, const NINE_GRID_INFO& NineGridInfo);
	bool IsNormalGrid(const NINE_GRID_INFO& NineGridInfo);
	void BoundingRect(NINE_GRID_INFO& gridInfo);

private:
	NINE_GRID_INFO m_NineGridInfo[SS_NUM];

};
#endif // __NINEGRIDSTYLE_H__
